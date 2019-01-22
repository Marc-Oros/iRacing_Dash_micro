#include "common.h"
#include "display.h"
#include "font.h"

void setPin(uint8_t pin, uint8_t value)
{
	switch(pin)
	{
		//Pin 2
		case STB:
			if(value)
			{
				GPIOE_PDOR |= 0x00004;
			}else{
				GPIOE_PDOR &= ~0x00004;
			}
			break;
		//Pin 3
		case CLK:
			if(value)
			{
				GPIOE_PDOR |= 0x00008;
			}else{
				GPIOE_PDOR &= ~0x00008;
			}
			break;
		//Pin 4
		case DIO:
			if(value)
			{
				GPIOE_PDOR |= 0x00010;
			}else{
				GPIOE_PDOR &= ~0x00010;
			}
			break;
	}
}

void pinInit()
{
	//Enviem clk a PORTE
	SIM_SCGC5 |= 0x2000;
	//Definim els 3 pins com a GPIO
	PORTE_PCR2 = 0x100;
	PORTE_PCR3 = 0x100;
	PORTE_PCR4 = 0x100;
	//Definim els pins com sortida
	GPIOE_PDDR |= 0x1c;
	//Inicialitzem a HIGH
	setPin(DIO, 1);
	setPin(CLK, 1);
	setPin(STB, 1);
}

void sendByte(uint8_t data)
{
	int i;
	for(i=0; i<8; i++)
	{
		setPin(CLK, 0);
		setPin(DIO, (data & 1) == 1);
		data >>= 1;
		setPin(CLK, 1);
	}
}

void setMode(uint8_t wr_mode, uint8_t addr_mode)
{
	sendByte(0x40 | wr_mode | addr_mode);
}

void sendCmd(uint8_t cmd)
{
	setPin(STB, 0);
	sendByte(cmd);
	setPin(STB, 1);
}

void sendData(uint8_t addr, uint8_t data)
{
	setPin(STB, 0);
	setMode(WRITE_MODE, FIXED_ADDR);
	setPin(STB, 1);
	setPin(STB, 0);
	sendByte(0xC0 | addr);
	sendByte(data);
	setPin(STB, 1);
}

void turnOn(uint8_t brightness)
{
	sendCmd(0x88 | (brightness & 7));
}

void setLED(uint8_t index, uint8_t value)
{
	if (value != 0)
	{
		value = 1;
	}
	sendData((index % 8) * 2 + 1, value);
}

void setLEDs(uint8_t num)
{
	int i;
	for(i = 0; i<num; i++)
	{
		setLED(i, 1);
	}
	for(num; num<8; num++)
	{
		setLED(num, 0);
	}
}

void clearDisplay()
{
	setPin(STB, 0);
	setMode(WRITE_MODE, INCR_ADDR);
	sendByte(0xC0);
	int i;
	for(i=0; i<16; i++)
	{
		sendByte(0x00);
	}
	setPin(STB, 0);
}

void writeChar(uint8_t idx, uint8_t value)
{
	if (idx == 6 && value != 0)
	{
		value |= 128;
	}
	sendData((idx % 8) * 2, value);
}

void clearLEDs()
{
	int i;
	for(i=0; i<8; i++)
	{
		setLED(i, 0);
	}
}

void writeStr(uint8_t idx, char* str)
{
	uint8_t *buf;
	int len = strlen(str);
	buf = malloc(len*sizeof(uint8_t));
	int i = 0;
	uint8_t BCD;
	uint8_t *font_array;
	font_array = malloc(128 * sizeof(uint8_t));
	getFontArray(font_array);
	while(str[i] != '\0')
	{
		BCD = font_array[(int)str[i]];
		buf[i] = BCD;
		i++;
	}
	i = 0;
	while(i < len)
	{
		writeChar(i, buf[i]);
		i++;
	}
	while(i<8)
	{
		writeChar(i, 0x00);
		i++;
	}
	free(buf);
	free(font_array);
}

void initAll(uint8_t bl)
{
	pinInit();
	turnOn(bl);
	clearDisplay();
	clearLEDs();
}

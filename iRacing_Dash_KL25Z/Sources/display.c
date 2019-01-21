#include "common.h"
#include "display.h"
#include "font.h"

void push(node_t* origin, uint8_t value)
{
	if(origin->val == NULL)
	{
		origin->val = value;
		origin->next = NULL;
		origin->prev = NULL;
	}else{
		node_t* last = origin;
		while(last->next != NULL)
			last = last->next;
		node_t* aux;
		aux = malloc(sizeof(node_t));
		aux->val = value;
		aux->prev = last;
		aux->next = NULL;
		last->next = aux;
	}
}

uint8_t pop(node_t* origin)
{	
	if (origin->val == NULL)
	{
		return 0;
	}else{
		uint8_t content;
		node_t* last = origin;
		while(last->next != NULL)
			last = last->next;
		content = last->val;
		if (last == origin)
		{
			origin->val = NULL;
		}else{
			last = last->prev;
			last->next = NULL;
		}
		return content;
	}	
}

void setPin(uint8_t pin, uint8_t value)
{
	switch(pin)
	{
		//Pin 2
		case DIO:
			if(value)
			{
				GPIOE_PDOR |= 0x00004;
			}else{
				GPIOE_PDOR &= ~0x00004;
			}
			break;
		//Pin 3
		case STB:
			if(value)
			{
				GPIOE_PDOR |= 0x00008;
			}else{
				GPIOE_PDOR &= ~0x00008;
			}
			break;
		//Pin 4
		case CLK:
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
		setLED(i, 0);
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
	if (idx == 6)
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
	node_t byte_list;
	byte_list.val = NULL;
	byte_list.next = NULL;
	byte_list.prev = NULL;
	int i = 0;
	uint8_t BCD;
	uint8_t font_array[128];
	getFontArray(font_array);
	while(str[i] != '\0')
	{
		if(str[i] == '.')
		{
			push(&byte_list, pop(&byte_list) | 128);
		}else{
			BCD = font_array[(int)str[i]];
			push(&byte_list, BCD);
		}
		i++;
	}
	int exit = 0;
	while(!exit)
	{
		writeChar(idx, byte_list.val);
		if(byte_list.next != NULL)
		{
			byte_list = *(byte_list.next);
			idx++;
		}else{
			exit = 1;
		}
	}
}

void initAll(uint8_t bl)
{
	pinInit();
	turnOn(bl);
	clearDisplay();
	clearLEDs();
}

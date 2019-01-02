#include "common.h"
#include "Cpu.h"
#include "Events.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "USB0.h"
#include "MCUC1.h"
#include "CLS1.h"
#include "TMOUT1.h"
#include "WAIT1.h"
#include "CS1.h"
#include "CS2.h"
#include "UTIL1.h"
#include "XF1.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


static void CDC_Init(void) {
	static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
	while(CDC1_App_Task(cdc_buffer, sizeof(cdc_buffer))==ERR_BUSOFF) {
      //Enumeración de dispositivo
	  WAIT1_Waitms(10);
	}  
}

void delay(unsigned int length_ms)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;  // Make sure clock is enabled
    LPTMR0_CSR = 0;                     // Reset LPTMR settings         
    LPTMR0_CMR = length_ms;             // Set compare value (in ms)

    // Use 1kHz LPO with no prescaler
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;
    
    // Start the timer and wait for it to reach the compare value
    LPTMR0_CSR = LPTMR_CSR_TEN_MASK;
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK))
        ;
    
    LPTMR0_CSR = 0;                     // Turn off timer
}

void initLED()
{
	SIM_SCGC5 |= 0x1400;
	PORTB_PCR18 = 0x100;
	PORTB_PCR19 = 0x100;
	PORTD_PCR1 = 0x100;
	GPIOB_PDOR |= 0xC0000;
	GPIOD_PDOR |= 0x00002;
	GPIOD_PDDR |= 0x00002;
	GPIOB_PDDR |= 0xC0000;
}

void ledsOff()
{
	GPIOB_PDOR |= 0xC0000;
	GPIOD_PDOR |= 0x00002;
}

void getPack(uint8_t *buf)
{
	int i;
	if(CDC1_GetCharsInRxBuf() >= 13)
	{
		for(i=0; i<13; i++)
		{
			CDC1_RecvChar(buf+i);
		}
	}
}

int main(void)
{
	//Inicializaciones
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	uint8_t buf[32];
	int led = 255;
	int speed;
	int rpm;
	int gear;
	int blink = 0;
	PE_low_level_init();  
	CDC_Init();
	initLED();
	buf[13]='\0';
	for(;;){
		getPack(buf);
		sscanf(buf, "%d %d %d %d", &led, &speed, &rpm, &gear);
		gear--;
		switch (led)
		{
			case 0:
				ledsOff();
				ledBlue();
				WAIT1_Waitms(10);
				break;
			case 1:
				ledsOff();
				ledGreen();
				WAIT1_Waitms(10);
				break;
			case 2:
				ledsOff();
				blink = (blink + 1) % BLINK_LEN;
				if(blink >= BLINK_LEN/2)
				{
				ledRed();
				}
				WAIT1_Waitms(10);
				break;
			default:
				ledsOff();
				blink = (blink + 1) % CONN_BLINK_LEN;
				if(blink >= CONN_BLINK_LEN/2)
				{
				ledBlue();
				}
				WAIT1_Waitms(10);
				break;
		}		
	}
	
}

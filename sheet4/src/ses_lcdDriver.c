/*
 * ses_lcdDriver.c
 *
 * Created: 5/14/2014 11:43:59 AM
 *  Author: Tobias
 */ 


#include "ses_common.h"
#include "ses_lcdDriver.h"

void lcdDriver_init() 
{
	DDRD |= ((1 << DISPLAY_CS_PIN) | (1 << DISPLAY_WR_PIN) | (1 << DISPLAY_RD_PIN) | (1 << DISPLAY_A0_PIN) );
	DISPLAY_WR_PORT |= (1 << DISPLAY_WR_PIN);
	DISPLAY_RD_PORT |= (1 << DISPLAY_RD_PIN);
	
}

/****************************************************
\brief This function writes a byte to the LCD buffer
	   The steps are:
	   
	   1) Select what kind of data is put onto parallel data port
	      A0 controls if data is 'command'(low) or 'data'
		  Here we want to write 'data' so A0 is set to high.
		  
		  Also the pin RD must be set to low and pin WR to high in order 
		  to write data.

****************************************************/
void lcdDriver_writeData(uint8_t data) 
{
	DISPLAY_WR_PORT |= (1 << DISPLAY_A0_PIN);
	DISPLAY_WR_PORT &= ~(1 << DISPLAY_RD_PIN);
	DISPLAY_WR_PORT |= (1 << DISPLAY_WR_PIN);
	
	// write
	DISPLAY_DATA_PORT = data;
	
	
	// wait 160 ns
	asm("NOP");
	asm("NOP");
	asm("NOP");
	

	// finalize operation
	DISPLAY_WR_PORT &= ~(1 << DISPLAY_WR_PIN);
	DISPLAY_WR_PORT |= (1 << DISPLAY_WR_PIN);
	
}

void lcdDriver_writeCommand(uint8_t data)
{
	DISPLAY_WR_PORT &= ~(1 << DISPLAY_A0_PIN);
	DISPLAY_WR_PORT &= ~(1 << DISPLAY_RD_PIN);
	DISPLAY_WR_PORT |= (1 << DISPLAY_WR_PIN);
	
	// write
	DISPLAY_DATA_PORT = data;
	
	
	// wait 160 ns
	asm("NOP");
	asm("NOP");
	asm("NOP");
		

	
	// finalize operation
	DISPLAY_WR_PORT &= ~(1 << DISPLAY_WR_PIN);
	DISPLAY_WR_PORT |= (1 << DISPLAY_WR_PIN);
}


void lcdDriver_selectController(uint8_t sel) 
{
	if(sel > 0) {
		DISPLAY_CS_PORT |= (1 << DISPLAY_CS_PIN);
	}
	else {
		DISPLAY_CS_PORT &= ~(1 << DISPLAY_CS_PIN);
	}
	
	
}

/***********************************************************************
 \brief This function reads a status from the data port
		To do this
		
		1) The data/command register must be set to low (command)
		2) The read enabled flag must be set 
		3) The write enabled flag must be unset
		4) It takes min. 180 ns to read the value from the LCD
		   One cylces takes 62.5ns on the 16MHz CPU: 1/(16*10^-6)
		 
                                   
/************************************************************************/
uint8_t lcdDriver_readStatus()
{
	DISPLAY_RD_PORT &= ~(1 << DISPLAY_A0_PIN);
	DISPLAY_RD_PORT |=  (1 << DISPLAY_RD_PIN);
	DISPLAY_RD_PORT &= ~(1 << DISPLAY_WR_PIN);
	
	// wait 180 ns
	asm("nop");
	asm("nop");
	asm("nop");
	
	// finalize operation
	DISPLAY_RD_PORT &= ~(1 << DISPLAY_RD_PIN);
	DISPLAY_RD_PORT |=  (1 << DISPLAY_RD_PIN);

	return DISPLAY_DATA_PORT;
}
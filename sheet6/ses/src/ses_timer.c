/* INCLUDES ******************************************************************/

#include "ses_timer.h"


/*FUNCTION DEFINITION ********************************************************/
pTimerCallback timer2_callback = NULL;

/***********************************************************************
 \brief Initializes Timer2 

 The timer is used to invoke an interrupt every 1 ms. To configure the timer
 we need to follow these steps:
 
 
   1) The power for the timer2 module is turned on by deactivating the
	  power reduction for this module. The Power Reduction Register (PRR0)
      controls the Sleep Modes by turning off the clock to individual
	  peripherals. By writing a ZERO to the Power Reduction Timer 2 (PRTIM2) Bit
	  the module is powered on.
	  
   2) The timer can be used in different modes. We want to check the counter value
      against a value in the Output Timer Control Register (OCR2A) register. 
	  A Counter is configured by Timer/Counter Control Register (TCCR2A and TCCR2B). Here we set 
	  the WGM21-Bit to 1 to enable the Clear Timer on Compare Match (CTC) Mode. This 
	  mode automatically clears the counter value and starts from the bottom again if 
	  the top value is reached.
	 
   3) Next we set the top value in the Output Timer Control Register (OCR2A). In our case we
      set it to 250 because the formula for the time between interrupts is:
	  
	  t = (TOP_VALUE * PRESCALER) / CYCLES
	  
	  Since we have t=1ms, cycles=16.000.000 and prescaler=64 (see below) we can solve to top value.
	  
   4) Set the prescaler. Without a prescaler and a top value the timer 2 would interrupt
      every 16us when the highest counter value 0xFF is incremented by 1. 
	  This is too fast for us, so we slow the process down by the factor
	  1 ms / 16 us = 62.5
	  
	  However there are only pre-defined prescaler of 2, 4, 8, 16, 32, 64, 128 and 256.
	  We settle to the nearest value of 64 and use the formula in 3) and the top value 
	  to get the right interrupt interval of 1 ms. 
	  
	  A prescaler of 64 is also set in the Timer/Counter Control Register (TCCRB) by writing
	  a one to the Clock Select Bit 2 (CS22).
	  
   5) Last we need to enable interrupts. This is done by writing to the Timer/Counter Interrupt Flag Register (TIFR2).
      Since we want to compare the Output Timer Control Register (OCR2A) we need to set
	  the Output Compare Flag 2 A (OCF2A).
	  
	  Of course the interrupt flag must also be set in the Timer Interrupt Mask (TIMSK2) register. 
	  The interrupt flag is called Timer/Counter2 Output Compare Match B Interrupt Enable (OCIE2A)
	  
                                                                     
************************************************************************/
void timer2_init(pTimerCallback cb) {
	timer2_callback = cb; 
	
	// Power on Timer 2
	PRR0 &=  ~(1 << PRTIM2); 
	
	// Clear Timer on Compare Match (CTC) Mode
	TCCR2A |= (1 << WGM21); 
	
	// Set Top Value
	OCR2A = 0xFA; // 250

	// Set the Prescaler to 64
	TCCR2B |= (1 << CS22);
	
	// Enable interrupts
	TIMSK2	|= (1 << OCIE2A);
	TIFR2	|= (1 << OCF2A);
	
}

ISR(TIMER2_COMPA_vect) {
	if( NULL != timer2_callback) {
		timer2_callback();
	}
	
}
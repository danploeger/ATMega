/*
 * ses_buzzer.c
 *
 *  Created on: Jun 24, 2014
 *      Author: happy
 */


#include "ses_buzzer.h"
#include "ses_common.h"

void initBuzzer(void) {

	// Power on Timer 1
	PRR0 &=  ~(1 << PRTIM1);

	// Clear Timer on Compare Match (CTC) Mode
	TCCR1B |= (1 << WGM12);

	// Toggle OCnA/OCnB/OCnC on Compare	Match.
	TCCR1A |= (1 << COM1B0);


	/*
	 * The buzzer should buzz with a frequency of 2 kHz (loud noise)
	 *
	 * 16.000.000 (System fequency) / 64 (prescaler) = 250.000
	 * 250.000 / 125 (Overflow) = 2000 Hz
	 *
	 */


	// Set Top Value
	OCR1AH = 0x00;
	OCR1AL = 0x7d; // 125

	// Set the Prescaler to 64
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS11);

	// Enable interrupts
	TIMSK1	|= (1 << OCIE1A);
	TIFR1	|= (1 << OCF1A);

}

void startAlarm(void) {
	// unmask interrupt
	TIMSK1	|= (1 << OCIE1A);
}


void stopAlarm(void) {
	TIMSK1 &= ~(1 << OCIE1A);
}


ISR(TIMER1_COMPA_vect) {
	/* bit is automatically toggled
	 * due to the above configuration of timer register:
	 *
	 * 	TCCR1A |= (1 << COM1B0);
	 */

}

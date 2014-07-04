/*
 * ses_buzzer.c
 *
 *  Created on: Jun 24, 2014
 *      Author: happy
 */


#include "ses_buzzer.h"
#include "ses_common.h"

/**
 *
 * The buzzer should buzz with a frequency of 2 kHz (loud noise)
 *
 * 16.000.000 (System fequency) / 64 (prescaler) = 250.000
 * 250.000 / 125 (Overflow) = 2000 Hz
 *
 */
void initBuzzer(void) {
	// Power on Timer 0
	PRR0 &=  ~(1 << PRTIM0);
	DDR(PORTG)&= ~(1 << PIN5);

	// Configure Timer Mode
	TCCR0A |= (1 << COM0B0); // Toggle OC0B on Compare Match
	TCCR0A |= (1 << COM0A0); // Toggle OC0B on Compare Match
	TCCR0A |= (1 << WGM01);	 // CTC-Mode Clear Timer on Compare Match

	TCCR0B = PRESCALER_440HZ;

	// Set Top Value
	OCR0A = TOP_VALUE_440HZ;
	OCR0B = TOP_VALUE_440HZ;

}

void startAlarm(void) {
	DDR(PORTG)  |= (1 << PIN5);
}

void stopAlarm(void) {
	DDR(PORTG)&= ~(1 << PIN5);
}


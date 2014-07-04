/*
 * ses_buzzer.c
 *
 *  Created on: Jun 24, 2014
 *      Author: happy
 */


#include "ses_buzzer.h"
#include "ses_common.h"
#include "ses_led.h"

/**
 *
 * The buzzer should buzz with a frequency of 2 kHz (loud noise)
 *
 * 16.000.000 (System fequency) / 64 (prescaler) = 250.000
 * 250.000 / 125 (Overflow) = 2000 Hz
 *
 */
void initBuzzer(void) {
	leds_init();
	leds_off();


	// Power on Timer 0
	PRR0 &=  ~(1 << PRTIM0);


	// Configure Timer Mode
	TCCR0A |= (1 << COM0B0); // Toggle OC0B on Compare Match
	TCCR0A |= (1 << COM0A0); // Toggle OC0B on Compare Match
	TCCR0A |= (1 << WGM01);	 // CTC-Mode Clear Timer on Compare Match

	DDR(PORTG)  |= (1 << 5);


	// Set Top Value
	OCR0A = 71;
	OCR0B = 71;

	// Set the Prescaler
	//TCCR0B |= (1 << CS02);



}

void startAlarm(void) {
	TCCR0B |= (1 << CS02 ); // sets prescaler to 64
}


void stopAlarm(void) {
	TCCR0B &= 248; // set prescaler to 0
}


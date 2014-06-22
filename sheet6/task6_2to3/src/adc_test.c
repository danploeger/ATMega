/*
 * adc_test.c
 *
 *  Created on: Jun 17, 2014
 *      Author: tobias
 */


#include <stdlib.h>
#include "ses_led.h"
#include "ses_adc.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_scheduler.h"


const uint8_t THRESHOLD = 50;

void joystickCallback (void) {
	checkSensorChannel(ADC_JOYSTICK_CH);
}

void tempCallback (void) {
	checkSensorChannel(ADC_TEMP_CH);
}

void lightCallback (void) {
	checkSensorChannel(ADC_LIGHT_CH);
}

void checkSensorChannel(uint8_t channel) {

	uint16_t rawData=0;

	/* Read data from sensor */
	rawData = adc_read(channel);

	/* Read and output data */
	if( ADC_INVALID_CHANNEL == rawData ) {
		lcd_setCursor(0,3);
		printf("Invalid Channel     ");
		return;
	}

	switch (channel) {
	case ADC_MIC_NEG_CH:
		/* unused */
		break;
	case ADC_MIC_POS_CH:
		/*  unused */
		break;
	case ADC_TEMP_CH:
		/* Print temperature on second row */
		lcd_setCursor(0,1);
		printf("Temperature: %d *E-2 C     ", adc_convertTemp(rawData));
		break;
	case ADC_LIGHT_CH:
		/*  Print brightness on third row */
		lcd_setCursor(0,2);
		printf("Brightness: %d     ", rawData);
		break;
	case ADC_JOYSTICK_CH:
		joystickProcessing(rawData);
		break;
	case ADC_NUM:
		lcd_setCursor(0,3);
		printf("#ADC channels: %d     ", rawData);
		break;
	default:
		break;
	}
}

void joystickProcessing (uint16_t rawData) {
	/* Check the Direction of the joystick and print to first lcd row */
	lcd_setCursor(0,0);

	if( (rawData -200) < THRESHOLD ) {			/* RIGHT */
		printf("Joystick: right     ");
	}
	else if ( (rawData - 400) < THRESHOLD ) {	/* UP */
		printf("Joystick: up     ");
	}
	else if ( (rawData - 600) < THRESHOLD ) {	/* LEFT */
		printf("Joystick: left     ");
	}
	else if ( (rawData - 800) < THRESHOLD ) {	/* DOWN */
		printf("Joystick: down     ");
	}
	else if ( (rawData - 1000) < THRESHOLD ) {	/* MIDDLE */
		printf("Joystick: middle     ");
	}
	else {
		printf("Joystick: invalid     ");
	}
}


/* TEST FUNCTION *************************************************************/
int main(int argc, char **argv) {

	lcd_init();
	stdout = lcdout;
	adc_init();
	leds_init();

	led_redOff();
	led_yellowOff();
	led_greenOff();

	// Start scheduler
	scheduler_init();


	// IMPORTANT: Globally enable Interrupts
	sei();

	// Add tasks
	if (scheduler_add(joystickCallback, 100, 100)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}
	if (scheduler_add(tempCallback, 300, 1500)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}
	if (scheduler_add(lightCallback, 200, 200)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}

	// MAIN LOOP
	while (1) {
		scheduler_run();
	}


	return 0;
}

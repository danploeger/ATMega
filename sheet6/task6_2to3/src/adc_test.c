/*
 * adc_test.c
 *
 *  Created on: Jun 17, 2014
 *      Author: tobias
 */


#include <stdlib.h>
#include "ses_adc.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_scheduler.h"


const unit16_t THRESHOLD 50;

void checkJoystick(void) {

	uint16_t rawData=0;

	// read data from joystick sensor
	rawData = adc_read(ADC_JOYSTICK_CH);

	if( ADC_INVALID_CHANNEL == rawData ) {
		printf("Invalid Channel");
		return;
	}

	// Check the Direction and print to lcd
	lcd_setCursor(0,0);
	if( (rawData - 200) < THRESHOLD )		/* RIGHT */
	{
		print("RIGHT");
	}
	else if ( rawData - 400) < THRESHOLD)	/* UP */
	{
		printf("UP");
	}
	else if ( rawData - 600) < THRESHOLD) 	/* LEFT */
	{
		printf("LEFT");
	}
	else if ( rawData - 800) < THRESHOLD) 	/* DOWN */
	{
		printf("DOWN");
	}
	else if( rawData - 1000) < THRESHOLD) 	/* NO DIRECTION */
	{
		printf("MIDDLE");
	}
	else {
		printf("DIRECTION ERROR");
	}

}


/* TEST FUNCTION *************************************************************/
int main(int argc, char **argv) {

	adc_init();
	lcd_init();
	stdout = lcdout;

	// Start scheduler
	scheduler_init();


	// IMPORTANT: Globally enable Interrupts
	sei();

	// Add tasks
	if (scheduler_add(checkJoystick, 100, 100)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}

	// MAIN LOOP
	while (1) {
		scheduler_run();
	}


	return 0;
}

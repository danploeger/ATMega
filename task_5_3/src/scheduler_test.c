
#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include <stdlib.h>


uint16_t stopWatchTimerInMs = 20000;

/* FUNCTION DEFINITION *******************************************************/


void stopWatch(void) {
	if(stopWatchTimerInMs > 0) {
		lcd_setCursor(0, 0);
		printf("%d\n", stopWatchTimerInMs);
		stopWatchTimerInMs--;

	}
}


/**
 * Checks if the joystick button is still pressed. After 1 ms the bouncing must have stopped and
 * we should be able to read a active high signal from the pin
 */
void checkJoystickButton() {
	if ( PORTB & (1 << BUTTON_JOYSTICK_PIN) ) {
		led_yellowToggle();
	}

}

void checkRotaryButton() {
	if (PORTB & (1 << BUTTON_ROTARY_PIN)) {
		// start watch
		scheduler_add( stopWatch, stopWatchTimerInMs, 0);
	}
	else {
		// stop watch
		scheduler_remove( stopWatch );
	}
}


void joystickCallback() {
	// add tasks if not already scheduled
	if( NULL == scheduler_find( checkJoystickButton ))
	{
		if( scheduler_add(checkJoystickButton, 1, 0) ) {
			led_redOn(); // error
			exit(1); // no free slots
		}
	}

}

void rotaryCallback() {
	// add tasks if not already scheduled
	if( NULL == scheduler_find( checkRotaryButton ))
	{
		if( scheduler_add(checkRotaryButton, 1, 0) ) {
			led_redOn(); // error
			exit(1); // no free slots
		}
	}
}


/* TEST FUNCTION *************************************************************/

int main (void) {
	leds_init();
	leds_off();
	lcd_init();
	stdout=lcdout;

	button_init();
	button_setJoystickButtonCallback(joystickCallback);
	button_setRotaryButtonCallback(rotaryCallback);


	// IMPORTANT: Globally enable Interrupts
	sei();


	// Add tasks
	if (scheduler_add(led_greenToggle, 500, 500)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}






	// Start scheduler
	scheduler_init();

	// MAIN LOOP
	while(1) {
		scheduler_run();
	}


	return true;
}

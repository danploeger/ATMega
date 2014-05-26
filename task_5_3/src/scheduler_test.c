
#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_button.h"
#include <stdlib.h>


/* FUNCTION DEFINITION *******************************************************/

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
	if (PORTB & (1<<BUTTON_JOYSTICK_PIN)) {
		led_yellowToggle();
	}
}


void joystickCallback() {
	// add tasks if not already scheduled
	if( NULL == scheduler_find( checkJoystickButton ))
	{
		if( scheduler_add(checkJoystickButton, 1, 0) ) {
			exit(1); // no free slots
		}
	}

}

void rotaryCallback() {
	// add tasks if not already scheduled
	if( NULL == scheduler_find( checkRotaryButton ))
	{
		if( scheduler_add(checkRotaryButton, 1, 0) ) {
			exit(1); // no free slots
		}
	}
}


/* TEST FUNCTION *************************************************************/

int main (void) {
	leds_init();
	button_init();
	button_setJoystickButtonCallback(joystickCallback);
	button_setRotaryButtonCallback(rotaryCallback);


	// IMPORTANT: Globally enable Interrupts
	sei();


	// Add tasks
	if (scheduler_add(led_greenToggle, 500, 500)) {
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

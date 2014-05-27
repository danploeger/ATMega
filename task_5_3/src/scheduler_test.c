
#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include <stdlib.h>
#include <util/atomic.h>


volatile uint16_t stopWatchTimerInMs = 20000;

volatile bool joystickTaskScheduled = false;
volatile bool rotaryTaskScheduled = false;
/* FUNCTION DEFINITION *******************************************************/


void stopWatch(void) {
	if(stopWatchTimerInMs > 0) {
		lcd_setCursor(3, 0);
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
		joystickTaskScheduled = false;
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

	lcd_setCursor(0,3);
	printf("joystick");
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(joystickTaskScheduled) {
			return;
		}
		joystickTaskScheduled = true;
	}


	if( scheduler_add(checkJoystickButton, 1, 0) ) {
		led_redOn();
		exit(1); // no free slots
	}

}

void rotaryCallback() {

	lcd_setCursor(10 ,3);
	printf("rotary");
	if( scheduler_add(checkRotaryButton, 1, 0) ) {
		led_redOn(); // error
		exit(1); // no free slots
	}

}


/* TEST FUNCTION *************************************************************/

int main (void) {
	leds_init();
	leds_off();
	lcd_init();
	stdout=lcdout;


	// Start scheduler
	scheduler_init();
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







	// MAIN LOOP
	while(1) {
		scheduler_run();
	}


	return true;
}

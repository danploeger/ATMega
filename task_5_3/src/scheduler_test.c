
#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include <stdlib.h>
#include <util/atomic.h>


volatile uint16_t stopWatchTimerInMs = 20000;

/* FUNCTION DEFINITION *******************************************************/


void stopWatch(void) {
	if(stopWatchTimerInMs > 0) {
		lcd_setCursor(0, 3);
		printf("%d\n", stopWatchTimerInMs);
		stopWatchTimerInMs--;
	} else {
		printf("%d\n", stopWatchTimerInMs);
		// stop watch
		scheduler_remove(stopWatch);
	}
}

void joystickCallback() {
	led_yellowToggle();
}

void rotaryCallback() {
	if ((PIN(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) == 0) {
		if (scheduler_find(stopWatch()) == NULL) {
			// start watch
			scheduler_add(stopWatch, 1, 1);
		} else {
			// stop watch
			scheduler_remove(stopWatch());
		}
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

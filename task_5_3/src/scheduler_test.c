#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include <stdlib.h>
#include <util/atomic.h>

bool watchStarted = false;
volatile uint16_t stopWatchTimerInSec = 20;

/* FUNCTION DEFINITION *******************************************************/

void startWatch(void) {

	if (stopWatchTimerInSec > 0) {
		lcd_setCursor(0, 3);
		printf("%d\n", stopWatchTimerInSec);
		stopWatchTimerInSec--;
	}

}

void joystickCallback() {
	led_yellowToggle();
}

void rotaryCallback() {

	if (!watchStarted) {
		watchStarted = true;
		scheduler_add(startWatch, 1000, 1000);
	} else {
		watchStarted = false;
		scheduler_remove(startWatch);
	}
}

/* TEST FUNCTION *************************************************************/

int main(void) {
	leds_init();
	leds_off();
	lcd_init();
	stdout = lcdout;

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
	while (1) {
		scheduler_run();
	}

	return true;
}

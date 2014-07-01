#include "ses_scheduler.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include <stdlib.h>
#include <util/atomic.h>

bool watchStarted = false;
volatile uint16_t stopWatchTimerInMs = 20000;

/* FUNCTION DEFINITION *******************************************************/

void startWatch(void) {

	lcd_setCursor(0, 3);
	printf("%02d.%03d\n", stopWatchTimerInMs/1000, stopWatchTimerInMs%1000);

	if (stopWatchTimerInMs > 0) {
		stopWatchTimerInMs--;
	}
}

void startClock(void) {
	uint32_t time = scheduler_getSystemTime();
	lcd_setCursor(0, 0);
	printf("Time (ms): %lu", time);

	struct type_time timeStruct;
	scheduler_getTime(&timeStruct);
	lcd_setCursor(0, 1);
	printf("Time: %02d:%02d:%02d:%03d", timeStruct.hour, timeStruct.minute, timeStruct.second, timeStruct.milli);
}

void joystickCallback() {
	led_yellowToggle();
}

void rotaryCallback() {

	if (!watchStarted) {
		watchStarted = true;
		scheduler_add(startWatch, 1, 1);
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

	// Add tasks
	if (scheduler_add(startClock, 100, 100)) {
		led_redOn(); // red means error ;-)
		exit(1); // no free slots
	}

	// MAIN LOOP
	while (1) {
		scheduler_run();
	}

	return true;
}

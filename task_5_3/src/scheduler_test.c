
#include "ses_scheduler.h"
#include "ses_led.h"
#include <stdlib.h>


/* FUNCTION DEFINITION *******************************************************/

void joystickCallback() {
	if (scheduler_add(led_yellowToggle(), 1, 1)) {
		exit(1);
	}
}

/* TEST FUNCTION *************************************************************/

int main (void) {

	// init
	led_greenInit();
	led_yellowInit();
	button_init();

	// globally enable interrupts!!!!
	sei();
	button_setJoystickButtonCallback(joystickCallback);

	// add tasks
	if (scheduler_add(led_greenToggle, 500, 500)) {
		exit(1);
	}


	scheduler_init();


	while(1) {
		scheduler_run();
	}


	return true;
}

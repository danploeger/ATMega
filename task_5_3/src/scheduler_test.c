
#include "ses_scheduler.h"
#include "ses_led.h"
#include <stdlib.h>

int main (void) {

	// init
	led_greenInit();

	// add tasks
	scheduler_add(led_greenToggle, 500, 500);


	scheduler_init();


	while(1) {
		scheduler_run();
	}


	return true;
}

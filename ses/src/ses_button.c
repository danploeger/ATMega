/* INCLUDES ******************************************************************/


#include "ses_led.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_button.h"
#include "ses_scheduler.h"
#include <util/atomic.h>

/* PRIVATE VARIABLES *********************************************************/

pButtonCallback joystickCallback;
pButtonCallback rotaryCallback;
volatile bool debounceTaskPending = false;

/* FUNCTION DEFINITION *******************************************************/

void button_init() {
	DDRB  &=~((1 << BUTTON_ROTARY_PIN) | (1 << BUTTON_JOYSTICK_PIN));
	PORTB |= ((1 << BUTTON_ROTARY_PIN) | (1 << BUTTON_JOYSTICK_PIN));
	
	// To activate the pin change interrupt at all write a 1 
	// to the corresponding pin in the PCICR register
	PCICR |= 0x01;
	
	// Enable triggering an interrupt if a button is pressed
	PCMSK0 |= ((1 << BUTTON_ROTARY_PIN) | (1 << BUTTON_JOYSTICK_PIN));
	
}

void button_setRotaryButtonCallback(pButtonCallback callback) {
	rotaryCallback = callback;
}

void button_setJoystickButtonCallback(pButtonCallback callback) {
	joystickCallback = callback;
}

/**
 * This function schedules a task that will check the button state
 * after 2 ms. After this time the bouncing must have stopped and
 * we can tell if the button was being pressed for sure.
 * To avoid that the task is scheduled multiple times a global variable
 * 'debounceTaskPending' is used. It must be enclosed within an atomic block
 * so the concurring tasks are not reading/writing this at the same time
 * (boolean write is not necessarily atomic)
 */

void debounceButton() {
	if ((PIN(BUTTON_ROTARY_PORT) & (1<<BUTTON_ROTARY_PIN)) == 0) {
		if (rotaryCallback != NULL) {
			rotaryCallback();
		}
	}
	if ((PIN(BUTTON_JOYSTICK_PORT) & (1<<BUTTON_JOYSTICK_PIN)) == 0) {
		if (joystickCallback != NULL) {
			joystickCallback();
		}
	}
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		debounceTaskPending = false;
	}
}

// interrupt service routine for pin change interrupt 7:0
ISR(PCINT0_vect)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(debounceTaskPending) {
			return;
		}
		debounceTaskPending = true;
	}

	if(scheduler_add(debounceButton, 2, 0)) {
			led_redOn();
			exit(1); // no free slots
	}
}

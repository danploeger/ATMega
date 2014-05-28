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
volatile bool joystickTaskScheduled = false;
volatile bool rotaryTaskScheduled = false;

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
 * after 1 ms. After this time the bouncing must have stopped and
 * we can tell if the button was being pressed for sure.
 * To avoid that the task is scheduled multiple times a global variable
 * 'rotaryTaskScheduled' is used. It must be enclosed within an atomic block
 * so the concurring tasks are not reading/writing this at the same time
 */
void debounceRotaryButton() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(rotaryTaskScheduled) {
			return;
		}
		rotaryTaskScheduled = true;
	}

	if(scheduler_add(rotaryCallback, 2, 0)) {
			led_redOn();
			exit(1); // no free slots
	}
	rotaryTaskScheduled = false;
}

void debounceJoystickButton() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if(joystickTaskScheduled) {
			return;
		}
		joystickTaskScheduled = true;
	}

	if(scheduler_add(joystickCallback, 2, 0)) {
			led_redOn();
			exit(1); // no free slots
	}
	joystickTaskScheduled = false;
}


// interrupt service routine for pin change interrupt 7:0
ISR(PCINT0_vect)
{
	if ((PIN(BUTTON_ROTARY_PORT) & (1<<BUTTON_ROTARY_PIN)) == 0) {
		if (rotaryCallback != NULL) {
			debounceRotaryButton();
		}
	}
	if ((PIN(BUTTON_JOYSTICK_PORT) & (1<<BUTTON_JOYSTICK_PIN)) == 0) {
		if (joystickCallback != NULL) {
			debounceJoystickButton();
		}
	}


}

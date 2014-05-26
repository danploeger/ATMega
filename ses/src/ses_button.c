/* INCLUDES ******************************************************************/

#include "ses_button.h"
#include "ses_led.h"

/* PRIVATE VARIABLES *********************************************************/

pButtonCallback joystickCallback;
pButtonCallback rotaryCallback;

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

// interrupt service routine for pin change interrupt 7:0
ISR(PCINT0_vect)
{
	if (PORTB & (1<<BUTTON_ROTARY_PIN)) {
		if (rotaryCallback != NULL) {
			rotaryCallback();
		}
	}
	if (PORTB & (1<<BUTTON_JOYSTICK_PIN)) {
		if (joystickCallback != NULL) {
			joystickCallback();
		}
	}
}

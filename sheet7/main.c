/*
 * main.c
 *
 *  Created on: Jul 1, 2014
 *      Author: tobias
 */

#include "ses_buzzer.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_lcdDriver.h"
#include "ses_lcd.h"
#include <stdlib.h>



typedef struct Fsm Fsm;
typedef struct Event Event;
typedef void (*State) (Fsm *, const Event *);

struct Fsm {
	State state;		// the current state
	uint8_t alarmSet;	// is alarm enabled?
	uint32_t timeSet;	// temporary buffer for time
};

struct Event {
	uint8_t signal;		// multi purpose signal
};

/* dispatches events to state machine, called in application */
inline static void fsm_dispatch(Fsm *fsm, const Event* event) {
	fsm->state(fsm, event);
}

/* sets and calls initial state of state machine */
inline static void fsm_init(Fsm *fsm, State init) {
	fsm->state = init;
	fsm_dispatch(fsm, NULL);
}

void joystickCallback() {
	Event e;
	e->signal = JOYSTICK_SIG;
	fsm_dispatch((Fsm*) & keyboard, (const Event*) &e);
}

void rotaryCallback() {
	Event e;
	e->signal = ROTARY_SIG;
	fsm_dispatch((Fsm*) & keyboard, (const Event*) &e);
}

/* all sub-initializations of hardware and fsm initial state */
void alarmClock_init(Fsm *me, Event *e) {
	/** initialize hardware **/
	/* initialize LEDs */
	leds_init();
	leds_off();
	/* initialize display */
	lcd_init();
	stdout = lcdout;
	/* start scheduler */
	scheduler_init();
	/* initialize buttons */
	button_init();
	button_setJoystickButtonCallback(joystickCallback);
	button_setRotaryButtonCallback(rotaryCallback);
	/* Globally enable Interrupts */
	sei();

	/* initialize fsm */
	// TODO set timeStruct
	e->signal = EVENT_NONE;
	me->state = (State) alarmClock_setClockHour;
}

int main(void) {
	Fsm fsm;
	fsm_init((Fsm*) &fsm, (void*)alarmClock_init);
	while (1) {
		scheduler_run();
	}
	return 0;
}

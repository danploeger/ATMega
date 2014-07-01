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

typedef void (State*) (Fsm*, const Event *);

struct Fsm {
	State state;		// the current state
	uint8_t state;		// is alarm enabled?
	uint32_t timeSet;	// temporary buffer for time

};

struct Event {
	uint8_t signal		// multi purpose signal
};

/* dispatches events to state machin, called in application */
inline static void fsm_dispatch(Fsm *fsm, const Event* event);

/* sets and calls initial state of state machine */
inline static void fsm_init(Fsm *fsm, State init);


void alarmClock_init(Fsm *me, Event *e) {
	// initialize hardware

	e->signal = EVENT_NONE;
	me->signal = (State) alarmClock_YOUR_NEXT_STATE;
}


int main(void) {




	return 0;
}

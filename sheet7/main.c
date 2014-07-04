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
#include "ses_scheduler.h"
#include <stdlib.h>


#define HOUR_IN_MS 3600000
#define MIN_IN_MS    60000


/* typedefs */
typedef struct Fsm Fsm;
typedef struct Event Event;
typedef void (*State) (Fsm *, const Event *);

static Fsm fsm;




/* signals */
enum { JOYSTICK_SIG, ROTARY_SIG, ALARM_SIG, TIMEOUT_SIG, EVENT_NONE};


/* forward declarations */
void alarmClock_setClockHour(Fsm *, const Event *);
void alarmClock_setClockMinute(Fsm *, const Event *);
void alarmClock_clockRun(Fsm *, const Event *);
void alarmClock_setAlarmHour(Fsm *, const Event *);
void alarmClock_setAlarmMinute(Fsm *, const Event *);
void alarmClock_ringAlarm(Fsm *, const Event *);



struct Fsm {
	State state;			// the current state
	uint8_t alarmSet;		// is alarm enabled?
	uint32_t systemTime;	// temporary buffer for time
	uint32_t alarmTime;		// temporary buffer for alarm
};

struct Event {
	uint8_t signal;		// multi purpose signal
} event;

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
	e.signal = JOYSTICK_SIG;
	fsm_dispatch(&fsm, &e);
}

void rotaryCallback() {
	Event e;
	e.signal = ROTARY_SIG;
	fsm_dispatch(&fsm, &e);
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


void alarmClock_setClockHour(Fsm *me, const Event *e) {
	switch (e->signal) {
		case ROTARY_SIG:
			me->systemTime += HOUR_IN_MS;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_setClockMinute;
			break;
		default:
			break;
	}
}

void alarmClock_setClockMinute(Fsm *me, const Event *e) {
	switch (e->signal) {
		case ROTARY_SIG:
			me->systemTime += MIN_IN_MS;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

void alarmClock_clockRun(Fsm *me, const Event *e) {

	switch (e->signal) {
		case ALARM_SIG:
			me->state = alarmClock_ringAlarm;
			break;
		case ROTARY_SIG:
			// toggle alarm
			me->alarmSet = ~me->alarmSet;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_setAlarmHour;
			break;
		default:
			break;
	}

}

void alarmClock_setAlarmHour(Fsm *me, const Event *e) {
	switch (e->signal) {
		case ROTARY_SIG:
			me->alarmTime += HOUR_IN_MS;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_setAlarmHour;
			break;
		default:
			break;
	}
}

void alarmClock_setAlarmMinute(Fsm *me, const Event *e) {
	switch (e->signal) {
		case ROTARY_SIG:
			me->alarmTime += MIN_IN_MS;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

void alarmClock_ringAlarm(Fsm *me, const Event *e) {
	switch (e->signal) {
		case TIMEOUT_SIG:
			me->state = alarmClock_clockRun;
			break;
		case ROTARY_SIG:
			me->state = alarmClock_clockRun;
			break;
		case JOYSTICK_SIG:
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

int main(void) {
	fsm_init((Fsm*) &fsm, (void*)alarmClock_init);
	while (1) {
		scheduler_run();
	}
	return 0;
}

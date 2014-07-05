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
#include <math.h>


#define HOUR_IN_MS 3600000
#define MIN_IN_MS    60000
#define SEC_IN_MS     1000

/* TYPEDEFS AND PRIVATE DECLARATIONS *****************************************/
typedef struct Fsm Fsm;
typedef struct Event Event;
typedef void (*State) (Fsm *, const Event *);

static Fsm fsm_alarmClock;

/* SIGNALS *******************************************************************/
enum { JOYSTICK_SIG, ROTARY_SIG, ALARM_SIG, TIMEOUT_SIG, EVENT_NONE};

/* FORWARD DECLARATIONS ******************************************************/
void alarmClock_setClockHour(Fsm *, const Event *);
void alarmClock_setClockMinute(Fsm *, const Event *);
void alarmClock_clockRun(Fsm *, const Event *);
void alarmClock_setAlarmHour(Fsm *, const Event *);
void alarmClock_setAlarmMinute(Fsm *, const Event *);
void alarmClock_ringAlarm(Fsm *, const Event *);
void displayTime(uint8_t, bool, bool);

/* STRUCT DEFINITIONS ********************************************************/
struct Fsm {
	State state;			// the current state
	bool alarmSet;			// is alarm enabled?
	uint32_t systemTime;	// temporary buffer for time
	uint32_t alarmTime;		// temporary buffer for alarm
};

struct Event {
	uint8_t signal;		// multi purpose signal
} event;

/* FUNCTION DEFINITION *******************************************************/

/* dispatches events to state machine, called in application */
inline static void fsm_dispatch(Fsm *fsm, const Event* event) {
	fsm->state(fsm, event);
}

/* sets and calls initial state of state machine */
inline static void fsm_init(Fsm *fsm, State init) {
	fsm->state = init;
	fsm_dispatch(fsm, NULL);
}

/* called by ISR */
void joystickCallback() {
	Event e;
	e.signal = JOYSTICK_SIG;
	fsm_dispatch(&fsm_alarmClock, &e);
}

/* called by ISR */
void rotaryCallback() {
	Event e;
	e.signal = ROTARY_SIG;
	fsm_dispatch(&fsm_alarmClock, &e);
}

/* increments time every second */
void updateSystemTime (void) {
	/* increment system time by one second */
	fsm_alarmClock.systemTime += SEC_IN_MS;

	led_greenToggle();
	displayTime(1, 0, true);

	/* wrap around of time after 24 hours */
	if (fsm_alarmClock.systemTime >= 24 * HOUR_IN_MS) {
		fsm_alarmClock.systemTime = 0;
	}
	/* trigger alarm */
	if ( abs(fsm_alarmClock.alarmTime-fsm_alarmClock.systemTime)<1000 && fsm_alarmClock.alarmSet && (fsm_alarmClock.state == alarmClock_clockRun)) {
		Event e;
		e.signal = ALARM_SIG;
		fsm_dispatch(&fsm_alarmClock, &e);
	}
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
	/* initialize buzzer */
	initBuzzer();

	/* start scheduler */
	scheduler_init();
	/* initialize buttons */
	button_init();
	button_setJoystickButtonCallback(joystickCallback);
	button_setRotaryButtonCallback(rotaryCallback);

	/* initialize fsm */
	e->signal = EVENT_NONE;
	me->state = alarmClock_setClockHour;
	me->systemTime = 0;
	me->alarmTime = 0;
	me->alarmSet = false;
	fsm_dispatch(me, e);
}

void alarmClock_setClockHour(Fsm *me, const Event *e) {
	lcd_setCursor(0,0);
	printf("RB=Hour,JB=Cont");

	switch (e->signal) {
		case ROTARY_SIG:
			me->systemTime += HOUR_IN_MS;
			displayTime(1, 0, false);
			break;
		case JOYSTICK_SIG:
			lcd_clear();
			me->state = alarmClock_setClockMinute;
			break;
		default:
			break;
	}
}

void alarmClock_setClockMinute(Fsm *me, const Event *e) {
	lcd_setCursor(0,0);
	printf("RB=Hour,JB=Cont");
	switch (e->signal) {
		case ROTARY_SIG:
			me->systemTime += MIN_IN_MS;
			displayTime(1, 0, false);
			break;
		case JOYSTICK_SIG:
			if (scheduler_add(updateSystemTime, SEC_IN_MS, SEC_IN_MS)) {
				led_redOn(); // red means error ;-)
				exit(1); // no free slots
			}
			lcd_clear();
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

void alarmClock_clockRun(Fsm *me, const Event *e) {

	switch (e->signal) {
		case ALARM_SIG:
			startAlarm();
			scheduler_add(led_redToggle, 100, 100);
			me->state = alarmClock_ringAlarm;
			break;
		case ROTARY_SIG:
			// toggle alarm
			if(me->alarmSet) {
				me->alarmSet = 0;
			}
			else {
				me->alarmSet = 1;
			}

			me->alarmSet ? led_yellowOn() : led_yellowOff();
			break;
		case JOYSTICK_SIG:
			lcd_clear();
			me->state = alarmClock_setAlarmHour;
			break;
		default:
			break;
	}
}

void alarmClock_setAlarmHour(Fsm *me, const Event *e) {
	lcd_setCursor(0,0);
	printf("RB=Hour,JB=Cont");
	switch (e->signal) {
		case ROTARY_SIG:
			me->alarmTime += HOUR_IN_MS;
			displayTime(2, 1, false);
			break;
		case JOYSTICK_SIG:
			lcd_clear();
			me->state = alarmClock_setAlarmMinute;
			break;
		default:
			break;
	}
}

void alarmClock_setAlarmMinute(Fsm *me, const Event *e) {
	lcd_setCursor(0,0);
	printf("RB=Min,JB=Cont");
	switch (e->signal) {
		case ROTARY_SIG:
			me->alarmTime += MIN_IN_MS;
			displayTime(2, 1, false);
			break;
		case JOYSTICK_SIG:
			lcd_clear();
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

void alarmClock_ringAlarm(Fsm *me, const Event *e) {
	switch (e->signal) {
		case TIMEOUT_SIG:
		case ROTARY_SIG:
		case JOYSTICK_SIG:
			stopAlarm();
			scheduler_remove(led_redToggle);
			me->state = alarmClock_clockRun;
			break;
		default:
			break;
	}
}

void displayTime(uint8_t line, bool alarmTime, bool showSeconds) {
	time_t myTime;

	if(alarmTime)
		scheduler_setSystemTime(fsm_alarmClock.alarmTime);
	else
		scheduler_setSystemTime(fsm_alarmClock.systemTime);

	scheduler_getTime(&myTime);
	lcd_setCursor(0,line);

	if(showSeconds)
		printf("%.2d:%.2d:%.2d", myTime.hour, myTime.minute, myTime.second);
	else
		printf("%.2d:%.2d", myTime.hour, myTime.minute);

}


int main(void) {
	/* globally enable Interrupts */
	sei();
	/* initialize fsm */
	fsm_init((Fsm*) &fsm_alarmClock, (void*)alarmClock_init);
	while (1) {
		scheduler_run();
	}
	return 0;
}

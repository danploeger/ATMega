/*INCLUDES *******************************************************************/
#include "ses_timer.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include "ses_scheduler.h"
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>



/* PRIVATE VARIABLES *********************************************************/
volatile taskDescriptor_t tasks[SCHEDULER_ENTRIES];
volatile static uint32_t systemTime = 0;


/*FUNCTION DEFINITION ********************************************************/

uint32_t scheduler_getSystemTime() {
	uint32_t returnTime = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		returnTime = systemTime;
	}
	return returnTime;
}

void scheduler_setSystemTime(uint32_t newTime) {
	systemTime = newTime; // TODO: atomic
}

void scheduler_getTime(time_t *time) {
	uint32_t totalMillis = systemTime;
	uint32_t totalSeconds = totalMillis / 1000;
	uint32_t totalMinutes = (totalSeconds / 60);
	uint16_t totalHours = totalSeconds / 3600;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		time->milli = totalMillis % 1000;
		time->second = totalSeconds % 60;
		time->minute = totalMinutes % 60;
		time->hour = totalHours % 24;
	}
}

void scheduler_setTime(const time_t *time) {
	uint8_t totalHours = time->hour;
	uint16_t totalMinutes = totalHours * 60 + time->minute;
	uint32_t totalSeconds = totalMinutes * 60 + time->second;
	systemTime = totalSeconds * 1000 + time->milli;
}

void scheduler_update(void) {

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		systemTime++;
	}

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				if(tasks[i].task != NULL && tasks[i].expire > 0) {
					tasks[i].expire--;
				}
			}
	}
}

void scheduler_init() {
	systemTime = 0;

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		tasks[i].task = NULL;
	}

	timer2_init(scheduler_update);

}

void scheduler_run() {


	for (int i=0; i < SCHEDULER_ENTRIES; i++) {

		task_t taskRunner = NULL;


		/* Expire Time is read (and written) here */
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			taskDescriptor_t taskDescr = tasks[i];

			if(taskDescr.task != NULL) {

				/* Execute expired tasks */
				if(0 == taskDescr.expire) {

					/* Remember Task that is going to be executed */
					taskRunner = taskDescr.task;


					/* Delete non-periodic tasks after execution */
					if(0 == taskDescr.period) {
						tasks[i].task = NULL;
						#ifdef __DEBUG__
							lcd_setCursor(i, 0);
							printf("_");
						#endif
					}
					/* Re-Schedule periodic tasks */
					else {
						tasks[i].expire = tasks[i].period;
					}
				}
			}
		}


		/* RUN TASK */
		if(taskRunner!=NULL) {
			taskRunner();

		}


	}

}

bool scheduler_add(task_t task, uint16_t expire, uint16_t period) {

	/* create new task */
	taskDescriptor_t aTask = {task, expire, period};


	/* search a free slot */
	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (tasks[i].task == NULL) {
				// found a free slot
				tasks[i] = aTask;

				return false;
			}
		}
	}


	// scheduler is full
	return true;


}

void scheduler_remove(task_t task) {

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if(task==tasks[i].task) {
				tasks[i].task = NULL;
			}
		}
	}
}

taskDescriptor_t* scheduler_find(task_t task) {
	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if(task==tasks[i].task) {
				return &tasks[i];
			}
		}
	}
	return NULL;
}

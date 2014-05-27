/*INCLUDES *******************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "ses_lcd.h"
#include "ses_lcdDriver.h"
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>



/* PRIVATE VARIABLES *********************************************************/
volatile taskDescriptor_t tasks[SCHEDULER_ENTRIES];


/*FUNCTION DEFINITION ********************************************************/

/**
 * This function decreases the expire time of all tasks
 * The expire time is decreased in an atomic block because the
 * scheduler also accesses this variable
 */
void scheduler_update(void) {

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				if(tasks[i].task != NULL && tasks[i].expire > 0) {
					tasks[i].expire--;
				}
			}
	}
}

void scheduler_init() {

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		tasks[i].task = NULL;
	}

	timer2_init(scheduler_update);

}

void scheduler_run() {

	for (int i=0; i < SCHEDULER_ENTRIES; i++) {

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			taskDescriptor_t taskDescr = tasks[i];

			if(taskDescr.task != NULL) {

				if(0 == taskDescr.expire) {
					taskDescr.task();

					if(0 == taskDescr.period) {
						tasks[i].task = NULL;
						lcd_setCursor(i, 1);
						printf("%d", i);
					}
					else {
						tasks[i].expire = tasks[i].period;
					}
				}
			}
		}


	}

}

/**
 * Iterates the global array 'tasks' until a free slot is found.
 * It adds the new tasks into this slot.
 *
 * If no free slot is found the function returns true
 */
bool scheduler_add(task_t task, uint16_t expire, uint16_t period) {

	// create new task
	taskDescriptor_t aTask = {task, expire, period};

	stdout=lcdout;



	// search a free slot
	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (tasks[i].task == NULL) {
				// found a free slot
				tasks[i] = aTask;

				lcd_setCursor(i, 0);
				printf("%d", i);
				return false;
			}
		}
	}


	// scheduler is full
	return true;


}

/**
 * Iterates the global array 'tasks' to find the right task.
 * If task is found it is set to NULL
 */
void scheduler_remove(task_t task) {

	for(int i=0; i < SCHEDULER_ENTRIES; i++) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if(task==tasks[i].task) {
				tasks[i].task = NULL;
			}
		}
	}
}

/**
 * Returns a pointer to a task descriptor or NULL if task is not within schedulers tasks
 */
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

/*INCLUDES *******************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>

/* TYPES *********************************************************************/
typedef struct {
	task_t task;
	uint16_t expire;
	uint16_t period;
} taskDescriptor_t;

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
				if(tasks[i].task != NULL) {
					tasks[i].expire--;
				}
			}
	}
}

void scheduler_init() {

	timer2_init(scheduler_update);

}

void scheduler_run() {

	for (int i=0; i < SCHEDULER_ENTRIES; i++) {
		taskDescriptor_t taskDescr = tasks[i];

		if(taskDescr.task != NULL) {

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				if(0 == taskDescr.expire) {
					taskDescr.task();

					if(0 >= taskDescr.period) {
						tasks[i].task = NULL;
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



	// search a free slot
	for(int i=0; i < SCHEDULER_ENTRIES; i++) {

		if (tasks[i].task == NULL) {
			// found a free slot
			tasks[i] = aTask;
			return false;
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
		if(task==tasks[i].task) {
			tasks[i].task = NULL;
		}
	}
}
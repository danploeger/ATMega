#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"

/* DEFINES & MACROS **********************************************************/

#define SCHEDULER_ENTRIES		10


/* TYPES ********************************************************************/

/** Type of function pointer for tasks.
 */
typedef void (*task_t)(void);


typedef struct {
	task_t task;
	uint16_t expire;
	uint16_t period;
} taskDescriptor_t;

/**
 * Readable form of time storage as struct.
 */
struct type_time {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t milli;
};

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes the task scheduler.
 */
void scheduler_init();

/**
 * Runs scheduler in an infinite loop.
 */
void scheduler_run();

/**
 * Adds a new task to the scheduler. A scheduled task is executed once after the given
 * time period, if period==0. A periodic tasks execution is repeated depending on its period.
 *
 * @param task      valid pointer to task
 * @param expire    time counter
 * @param period    time period
 * @return          FALSE = no error, TRUE = scheduler full/error
 */
bool scheduler_add(task_t task, uint16_t expire, uint16_t period);

/**
 * Removes a timer task from the scheduler.
 *
 * @param task	pointer to timer
 * */
void scheduler_remove(task_t task);

/**
 * Returns a task descriptor or NULL if task is not within scheduler tasks
 */
taskDescriptor_t* scheduler_find(task_t task);

#endif /* SCHEDULER_H_ */

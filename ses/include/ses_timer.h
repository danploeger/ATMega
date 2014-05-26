#ifndef SES_TIMER_H_
#define SES_TIMER_H_

/*INCLUDES *******************************************************************/

#include "ses_common.h"


/*PROTOTYPES *****************************************************************/


/**type of function pointer used as timer callback
 */
typedef void (*pTimerCallback)(void);


/**Initializes hardware timer 2 of MCU.
 * Timer callback is executed each millisecond.
 *
 * @param cb	valid pointer to callback function
 */
void timer2_init(pTimerCallback cb);



#endif /* SES_TIMER_H_ */

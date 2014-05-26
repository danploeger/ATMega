#ifndef SES_BUTTON_H_
#define SES_BUTTON_H_

/* INCLUDES ******************************************************************/

#include "ses_common.h"

/* DEFINES & MACROS **********************************************************/

// button wiring on SES board
#define BUTTON_ROTARY_PORT		PORTB
#define BUTTON_ROTARY_PIN		6
#define BUTTON_JOYSTICK_PORT	PORTB
#define BUTTON_JOYSTICK_PIN		7

/* TYPES *********************************************************************/

/**type of function pointer used as button callback
 */
typedef void (*pButtonCallback)();

/* FUNCTION PROTOTYPES *******************************************************/

/**initializes rotary encoder and joystick button
 */
void button_init(void);


/**sets callback for rotary encoder button pressed event
 *
 * @param callback	a valid pointer to callback function or NULL to 
 *			to deactivate the callback
 */
void button_setRotaryButtonCallback(pButtonCallback callback);


/**sets callback for joystick button pressed event
 *
 * @param callback 	a valid pointer to callback function or NULL to 
 *			to deactivate the callback
 */
void button_setJoystickButtonCallback(pButtonCallback callback);

#endif /* SES_BUTTON_H_ */

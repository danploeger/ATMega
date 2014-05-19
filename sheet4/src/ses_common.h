#ifndef SES_COMMON_H_
#define SES_COMMON_H_

/* INCLUDES ******************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* DEFINES & MACROS **********************************************************/

/**pin address of port x
 *
 * example: PING == PIN(PORTG)
 */
#define PIN(x) (*(&(x) - 2))

/**address of data direction register of port x
 *
 * example: DDRG == DDR(PORTG)
 */
#define DDR(x) (*(&(x) - 1))

#endif /* SES_COMMON_H_ */

#ifndef SES_LCDDRIVER_H_
#define SES_LCDDRIVER_H_



/*INCLUDES-------------------------------------------------------------------*/
#include <stdint.h>


/*MACROS---------------------------------------------------------------------*/

#define DISPLAY_DATA_PORT 	PORTE

#define DISPLAY_CS_PORT		PORTD
#define DISPLAY_CS_PIN		4

#define DISPLAY_A0_PORT		PORTD
#define DISPLAY_A0_PIN		6

#define DISPLAY_RD_PORT		PORTD
#define DISPLAY_RD_PIN		7

#define DISPLAY_WR_PORT		PORTD
#define DISPLAY_WR_PIN		5



/*PROTOTYPES-----------------------------------------------------------------*/

/**
 * Initializes display
 * \li Inits Port
 * \li Resets Display
 * \li Activates Display
 * \li ...
 */
void lcdDriver_init();


/**
 * Writes a byte into the RAM of the selected LCD controller
 *
 * @param data	a byte to write
 */
void lcdDriver_writeData(uint8_t data);

/**
 * Writes a command to the selected LCD controller
 * @param	a byte to write
 */
void lcdDriver_writeCommand(uint8_t data);

/**
 * Selects one controller of the LCD
 * @param sel	selected controller (either 0 or 1)
 */
void lcdDriver_selectController(uint8_t sel);

/**
 * Reads status byte from  selected LCD controller
 */
uint8_t lcdDriver_readStatus();


#endif /* SES_LCDDRIVER_H_ */

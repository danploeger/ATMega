#ifndef TWI_EEPROM_H_
#define TWI_EEPROM_H_

/*INCLUDES-------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>


/*PROTOTYPES-----------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif



/**
 * Initializes EEPROM.
 */
void ext_eeprom_init();


/**
 * Reads data from EEPROM. 
 *
 * @addr 	byte address
 * @data	pointer to buffer
 * @length  	number of bytes to read 
 * @return 	number of read bytes, in case of an error a value less than length
 * 		is returned.
 */
uint16_t ext_eeprom_read(uint32_t addr, uint8_t *data, uint16_t length);


/**
 * Writes data to EEPROM. 
 *
 * @addr 	byte address
 * @data	pointer to buffer
 * @length  	number of bytes to write
 * @return 	number of written bytes, in case of an error a value less than length
 * 		is returned.
 */
uint16_t ext_eeprom_write(uint32_t addr, const uint8_t *data, uint16_t length);


#ifdef __cplusplus
}
#endif

#endif /* TWI_EEPROM_H_ */

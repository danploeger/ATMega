#ifndef SES_ADC_H
#define SES_ADC_H

/*INCLUDES *******************************************************************/

#include <inttypes.h>
#include <avr/io.h>
#include "ses_common.h"

/* DEFINES & MACROS **********************************************************/

/* to signal that the given channel was invalid */
#define ADC_INVALID_CHANNEL    0xFFFF

/* sensor ports and pins */
#define ADC_MIC_NEG_PORT        PORTF
#define ADC_MIC_NEG_PIN             0
#define ADC_MIC_POS_PORT        PORTF
#define ADC_MIC_POS_PIN             1
#define ADC_TEMP_PORT           PORTF
#define ADC_TEMP_PIN                2
#define ADC_LIGHT_PORT          PORTF
#define ADC_LIGHT_PIN               4
#define ADC_JOYSTICK_PORT       PORTF
#define ADC_JOYSTICK_PIN            5

/* reference voltage and prescaling */
#define ADC_VREF_SRC             0x3  	/* use internal 1.6V Voltage Reference */
#define ADC_PRESCALE             0x6  	/* ADC clock prescaler: 250 kHz with division factor 64
										   (clock must be 330kHz or less for 10 bit resolution) */


/* setup adc channels */
enum ADChannels{
  ADC_MIC_NEG_CH=0,                     /* ADC0 */
  ADC_MIC_POS_CH,                       /* ADC1 */
  ADC_TEMP_CH,                          /* ADC2 */
  ADC_RESERVED1_CH,                     /* ADC3 */
  ADC_LIGHT_CH,                         /* ADC4 */
  ADC_JOYSTICK_CH,                      /* ADC5 */
  ADC_RESERVED2_CH,                     /* ADC6 */
  ADC_RESERVED3_CH,                     /* ADC7 */
  ADC_NUM                               /* number of ADC channels*/
};

/* TYPES ********************************************************************/


/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initialization of the Analog-to-Digital Converter (ADC). Configuration of temperature, light,
 * microphone and joystick registers. The reference voltage is set to 1.6V and the ADC clock speed
 * is set to 250 kHz. Auto triggering is disabled and ADC interrupt is enabled.
 */
void adc_init(void);

/**
 * Deactivates the ADC and enter power reduction mode. For reactivation please use adc_init()
 */
void adc_disable(void);

/**
 * Reads one sensor output from enum ADChannels. adc_read() is atomic, therefore only one sensor
 * can be read at the same time. Further read commands will be delayed until the conversion is done.
 */
uint16_t adc_read(uint8_t adc_channel);

/**
 * Converts the temperature sensor raw value into °C values. Does only work in the range of 0°C to 50°C.
 *
 */
int16_t adc_convertTemp(uint16_t val);


#endif /* SES_ADC_H */

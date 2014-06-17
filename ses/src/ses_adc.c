/* INCLUDES ******************************************************************/
#include "ses_adc.h"
#include "ses_common.h"
#include <util/atomic.h>

/* DEFINES & MACROS **********************************************************/

/* PRIVATE VARIABLES *********************************************************/

/* FUNCTION DEFINITION *******************************************************/

void adc_init(void) {
	// Step 1: configuration of data direction registers and
	// deactivation of pull-up resistors
	DDRF  &=~(1 << ADC_TEMP_PIN);
	DDRF  &=~(1 << ADC_LIGHT_PIN);
	DDRF  &=~(1 << ADC_MIC_POS_PIN);
	DDRF  &=~(1 << ADC_MIC_NEG_PIN);
	DDRF  &=~(1 << ADC_JOYSTICK_PIN);

	PORTF |= (1 << ADC_TEMP_PIN);
	PORTF |= (1 << ADC_LIGHT_PIN);
	PORTF |= (1 << ADC_MIC_POS_PIN);
	PORTF |= (1 << ADC_MIC_NEG_PIN);
	PORTF |= (1 << ADC_JOYSTICK_PIN);

	// Step 2: disabling power on ADC
	PRR0 &=  ~(1 << PRADC);

	// Step 3: reference voltage set
	ADMUX |= (ADC_VREF_SRC << REFS0);

	// Step 4: A/D conversion right adjust result
	ADMUX &= ~(1 << ADLAR);

	// Step 5: writing of ADC Control and Status Register A
	ADCSRA = ((1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (1 << ADIF) | (1 << ADIE) | (ADC_PRESCALE << ADPS0));
	// TODO: SREG I-bit?
}

uint16_t adc_read(uint8_t adc_channel) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

	}
}


void adc_disable(void) {

}


int16_t adc_convertTemp(uint16_t val) {

}


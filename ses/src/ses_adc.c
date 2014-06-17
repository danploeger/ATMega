/* INCLUDES ******************************************************************/
#include "ses_adc.h"
#include "ses_common.h"
#include <util/atomic.h>
#include <avr/sleep.h>

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
}

uint16_t adc_read(uint8_t adc_channel) {
	set_sleep_mode(SLEEP_MODE_ADC);

	// select adc channel
	ADCSRB &= ~(1 << MUX5);
	switch (adc_channel) {
	case ADC_MIC_NEG_CH:
		ADMUX &= ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		ADMUX |= PIN(1 << ADC_MIC_NEG_PORT); // TODO: ?
		break;
	case ADC_MIC_POS_CH:
		break;
	case ADC_TEMP_CH:
		break;
	case ADC_RESERVED1_CH:
		break;
	case ADC_LIGHT_CH:
		break;
	case ADC_JOYSTICK_CH:
		break;
	case ADC_RESERVED2_CH:
		break;
	case ADC_RESERVED3_CH:
		break;
	case ADC_NUM:
		break;
	default:
		return ADC_INVALID_CHANNEL;
	}

	// start conversion
	sleep_enable();
	do {
		sleep_cpu();
	} while ((ADCSRA & (1 << ADSC)) != 0);
	sleep_disable();

	return ADC;
}


void adc_disable(void) {
	ADCSRA &= ~(1 << ADEN); // TODO: if-Bedingung (atomic)
}


int16_t adc_convertTemp(uint16_t val) {

}

ISR(ADC_vect)
{

}

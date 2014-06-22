/* INCLUDES ******************************************************************/
#include "ses_adc.h"
#include "ses_common.h"
#include <util/atomic.h>
#include <avr/sleep.h>

/* DEFINES & MACROS **********************************************************/

/* PRIVATE VARIABLES *********************************************************/

/* Temperature
 */
const uint16_t RvTempMap[20][2] = { {2711, 27315}, {2597, 27414}, {2483, 27519}, {2369, 27628}, {2255, 27745}, {2141, 27868}, {2027, 28000}, {1913, 28140}, {1799, 28290}, {1685, 28453}, {1571, 28628}, {1457, 28819}, {1343, 29029}, {1229, 29261}, {1115, 29520}, {1001, 29812}, {887, 30146}, {773, 30536}, {659, 31001}, {545, 31573}, {431, 32309} };
const uint8_t  SCALE_FACTOR = 100;
const uint8_t  STEP_WIDTH = 114;

/* FUNCTION DEFINITION *******************************************************/

void adc_init(void) {
	/* Step 1: configuration of data direction registers and
	 * deactivation of pull-up resistors */
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

	/* Step 2: disable power reduction mode on ADC */
	PRR0 &=  ~(1 << PRADC);

	/* Step 3: reference voltage set */
	ADMUX |= (ADC_VREF_SRC << REFS0);

	/* Step 4: A/D conversion right adjust result */
	ADMUX &= ~(1 << ADLAR);

	/* Step 5: write ADC Control and Status Register A */
	ADCSRA = ((1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (1 << ADIF) | (1 << ADIE) | (ADC_PRESCALE << ADPS0));

	/* Ensure start-up waiting time of 20 us */
	_delay_us(20);
}

uint16_t adc_read(uint8_t adc_channel) {
	set_sleep_mode(SLEEP_MODE_ADC);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		/* select adc channel */
		ADCSRB &= ~(1 << MUX5);
		ADMUX &= ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		switch (adc_channel) {
		case ADC_MIC_NEG_CH:
			ADMUX |= ADC_MIC_NEG_CH;
			break;
		case ADC_MIC_POS_CH:
			ADMUX |= ADC_MIC_POS_CH;
			break;
		case ADC_TEMP_CH:
			ADMUX |= ADC_TEMP_CH;
			break;
		case ADC_RESERVED1_CH:
			ADMUX |= ADC_RESERVED1_CH;
			break;
		case ADC_LIGHT_CH:
			ADMUX |= ADC_LIGHT_CH;
			break;
		case ADC_JOYSTICK_CH:
			ADMUX |= ADC_JOYSTICK_CH;
			break;
		case ADC_RESERVED2_CH:
			ADMUX |= ADC_RESERVED2_CH;
			break;
		case ADC_RESERVED3_CH:
			ADMUX |= ADC_RESERVED3_CH;
			break;
		case ADC_NUM:
			return ADC_NUM;
		default:
			return ADC_INVALID_CHANNEL;
		}

		/* start conversion */
		sleep_enable();
		do {
			sleep_cpu(); /* go to sleep (and trigger ADC conversion) */
		} while ((ADCSRA & (1 << ADSC)) != 0); /* validate that the interrupt is triggered by ADC */
		sleep_disable();

		return ADC;
	}
}


void adc_disable(void) {
	ADCSRA &= ~(1 << ADEN);
}


int16_t adc_convertTemp(uint16_t val) {

	uint8_t index=-1;
	index = val * SCALE_FACTOR / STEP_WIDTH;

	return RvTempMap[index][1];
}

ISR(ADC_vect)
{
	/**
	 * Interrupt service routine kept empty because further processing
	 * of converted sensor data already happens in adc_read().
	 */
}

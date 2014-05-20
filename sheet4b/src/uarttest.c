/*INCLUDES-------------------------------------------------------------------*/
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include "ses_led.h"
#include "ses_uart.h"


/*PROTOTYPES-----------------------------------------------------------------*/

/**
 * Tests Universal Asynchronous Receiver Transmitter (UART).
 */


void leds_init(void) {
	led_redInit();
	led_yellowInit();
	led_greenInit();

	led_greenOff();
	led_yellowOff();
	led_redOff();
}

int main(void) {

	uart_init(57600);
	leds_init();
	sei();		// enable interrupts globally
	uint8_t counter=0;
	char input;

	while(1) {
		input = uart_getc();

		switch (input) {
			case '1': led_greenToggle();
			break;
			case '2': led_yellowToggle();
			break;
			case '3': led_redToggle();
			break;
			default :
			break;
		}
	}
	return 0;
}

/* INCLUDES ******************************************************************/

#include "ses_led.h"

/* FUNCTION DEFINITION *******************************************************/

void leds_init(void) {
	led_redInit();
	led_yellowInit();
	led_greenInit();
}

void leds_off(void) {
	led_redOff();
	led_yellowOff();
	led_greenOff();
}

void led_redInit(void) {
	DDR(LED_RED_PORT) |= (1<<LED_RED_PIN);
}

void led_redToggle(void) {
	LED_RED_PORT ^= (1<<LED_RED_PIN);
}

void led_redOn(void) {
	LED_RED_PORT &= ~(1<<LED_RED_PIN);
}

void led_redOff(void) {
	LED_RED_PORT |= (1<<LED_RED_PIN);
}

void led_yellowInit(void) {
	DDR(LED_YELLOW_PORT) |= (1<<LED_YELLOW_PIN);
}

void led_yellowToggle(void) {
	LED_YELLOW_PORT ^= (1<<LED_YELLOW_PIN);
}

void led_yellowOn(void) {
	LED_YELLOW_PORT &= ~(1<<LED_YELLOW_PIN);
}

void led_yellowOff(void) {
	LED_YELLOW_PORT |= (1<<LED_YELLOW_PIN);
}

void led_greenInit(void) {
	DDR(LED_GREEN_PORT) |= (1<<LED_GREEN_PIN);
}

void led_greenToggle(void) {
	LED_GREEN_PORT ^= (1<<LED_GREEN_PIN);
}

void led_greenOn(void) {
	LED_GREEN_PORT &= ~(1<<LED_GREEN_PIN);
}

void led_greenOff(void) {
	LED_GREEN_PORT |= (1<<LED_GREEN_PIN);
}

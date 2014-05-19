/*INCLUDES-------------------------------------------------------------------*/

#include <stdio.h>
#include <avr/io.h>
#include "ses_lcd.h"


/*PROTOTYPES-----------------------------------------------------------------*/

/**
 * Tests LCD.
 */
int main(void) {
	lcd_init();
	stdout=lcdout; // if you want to use printf instead of fprintf

	lcd_setCursor(0,0);
    fprintf(lcdout,"   Software For      Embedded  Systems\n");
    lcd_setCursor(7,3);
    printf("%d\n",2012);

	while(1) {}
	return 0;
}

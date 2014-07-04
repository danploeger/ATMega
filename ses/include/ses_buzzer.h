/*
 * ses_buzzer.h
 *
 *  Created on: Jun 24, 2014
 *      Author: happy
 */

#ifndef SES_BUZZER_H_
#define SES_BUZZER_H_

#define PRESCALER_440HZ 0x04
#define TOP_VALUE_440HZ 70


void initBuzzer(void);
void startAlarm(void);
void stopAlarm(void);

#endif /* SES_BUZZER_H_ */

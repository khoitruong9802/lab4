/*
 * software_timer.h
 *
 *  Created on: Nov 11, 2022
 *      Author: DELL
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern int timer0_flag;

void set_timer0(int duration);
void timer_run();

#endif /* INC_SOFTWARE_TIMER_H_ */

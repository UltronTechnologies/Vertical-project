/*
 * drv_thread_entry.h
 *
 *  Created on: 21 Aug 2023
 *      Author: khurr
 */

#ifndef DRV_THREAD_ENTRY_H_
#define DRV_THREAD_ENTRY_H_

void LiftDown();
void LiftUp();
void LiftStop();
void stoppinglift(int num);
void stoppinglift_down(int number);
void stoppinglift_up(int number);
int callfloor1();
int callfloor_1();
int callfloor2();
int callfloor_2();
int callfloor3();
int callfloor_3();
int callfloor4();
int callfloor_4();
void dooropen(uint8_t code, uint8_t shift);
void dooroperate(uint8_t code, uint8_t shift);
#endif /* DRV_THREAD_ENTRY_H_ */

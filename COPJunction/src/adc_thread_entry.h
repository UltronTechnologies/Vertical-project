/*
 * adc_thread_entry.h
 *
 *  Created on: 18 Sep 2023
 *      Author: khurr
 */

#ifndef ADC_THREAD_ENTRY_H_
#define ADC_THREAD_ENTRY_H_
#include <stdint.h>
/*****************************************************************************/
/* Global                                                                    */
/*****************************************************************************/
extern uint16_t usMuxResults1[72];
void adc_thread_entry(void);

#endif /* ADC_THREAD_ENTRY_H_ */

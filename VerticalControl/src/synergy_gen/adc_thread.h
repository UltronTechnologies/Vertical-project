/* generated thread header file - do not edit */
#ifndef ADC_THREAD_H_
#define ADC_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void adc_thread_entry(void);
                #else
extern void adc_thread_entry(void);
#endif
#include "r_adc.h"
#include "r_adc_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc1;
#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;
#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ADC_THREAD_H_ */

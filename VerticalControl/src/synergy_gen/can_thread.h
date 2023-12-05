/* generated thread header file - do not edit */
#ifndef CAN_THREAD_H_
#define CAN_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void can_thread_entry(void);
                #else
extern void can_thread_entry(uint8_t floor);
#endif
#include "r_can.h"
#include "r_can_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/** CAN on CAN Instance. */
extern const can_instance_t g_can0;
#ifndef CanCallback
void CanCallback(can_callback_args_t *p_args);
#endif
#define CAN_NO_OF_MAILBOXES_g_can0 (32)
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* CAN_THREAD_H_ */

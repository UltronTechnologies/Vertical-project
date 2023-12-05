/* generated thread header file - do not edit */
#ifndef DRV_THREAD_H_
#define DRV_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void drv_thread_entry(void);
                #else
extern void drv_thread_entry(void);
#endif
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_i2c.h"
#include "r_i2c_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
extern const i2c_cfg_t g_i2c0_cfg;
/** I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c0;
#ifndef NULL
void NULL(i2c_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg g_i2c0_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c0_ctrl;
#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == g_transfer0)
    #define g_i2c0_P_TRANSFER_TX (NULL)
#else
#define g_i2c0_P_TRANSFER_TX (&g_transfer0)
#endif
#if (SYNERGY_NOT_DEFINED == g_transfer1)
    #define g_i2c0_P_TRANSFER_RX (NULL)
#else
#define g_i2c0_P_TRANSFER_RX (&g_transfer1)
#endif
#undef SYNERGY_NOT_DEFINED
#define g_i2c0_P_EXTEND (&g_i2c0_cfg_extend)
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* DRV_THREAD_H_ */

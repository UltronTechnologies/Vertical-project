/* generated thread source file - do not edit */
#include "drv_thread.h"

TX_THREAD drv_thread;
void drv_thread_create(void);
static void drv_thread_func(ULONG thread_input);
static uint8_t drv_thread_stack[2048] BSP_PLACE_IN_SECTION_V2(".stack.drv_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer1) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_SCI0_RXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_SCI0_RXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer1_ctrl;
transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info,
  .activation_source = ELC_EVENT_SCI0_RXI,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer1,
  .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dtc };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer0) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_SCI0_TXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_SCI0_TXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info,
  .activation_source = ELC_EVENT_SCI0_TXI,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer0,
  .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
#if !defined(SSP_SUPPRESS_ISR_g_i2c0) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_i2c_rxi_isr, SCI, RXI, 0);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_i2c0) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_i2c_txi_isr, SCI, TXI, 0);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_i2c0) && !defined(SSP_SUPPRESS_ISR_SCI0)
SSP_VECTOR_DEFINE_CHAN(sci_i2c_tei_isr, SCI, TEI, 0);
#endif
sci_i2c_instance_ctrl_t g_i2c0_ctrl;

/** I2C extended configuration */
const sci_i2c_extended_cfg g_i2c0_cfg_extend =
{ .bitrate_modulation = true };

const i2c_cfg_t g_i2c0_cfg =
{ .channel = 0,
  .rate = I2C_RATE_STANDARD,
  .slave = 0x00,
  .addr_mode = I2C_ADDR_MODE_7BIT,
  .sda_delay = 300,
#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == g_transfer0)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer0,
#endif
#if (SYNERGY_NOT_DEFINED == g_transfer1)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer1,
#endif
#undef SYNERGY_NOT_DEFINED
  .p_callback = NULL,
  .p_context = (void*) &g_i2c0,
  .rxi_ipl = (12),
  .txi_ipl = (12),
  .tei_ipl = (12),
  .eri_ipl = BSP_IRQ_DISABLED,
  .p_extend = &g_i2c0_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c0 =
{ .p_ctrl = &g_i2c0_ctrl, .p_cfg = &g_i2c0_cfg, .p_api = &g_i2c_master_on_sci };
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void drv_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&drv_thread, (CHAR*) "Drive Thread", drv_thread_func, (ULONG) NULL, &drv_thread_stack, 2048,
                            1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&drv_thread, 0);
    }
}

static void drv_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    drv_thread_entry ();
}

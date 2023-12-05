/* generated thread source file - do not edit */
#include "lcd_thread.h"

TX_THREAD lcd_thread;
void lcd_thread_create(void);
static void lcd_thread_func(ULONG thread_input);
static uint8_t lcd_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.lcd_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void lcd_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&lcd_thread, (CHAR*) "LCD Thread", lcd_thread_func, (ULONG) NULL, &lcd_thread_stack, 1024,
                            2, 2, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&lcd_thread, 0);
    }
}

static void lcd_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    lcd_thread_entry (0);
}

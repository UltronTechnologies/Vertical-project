#include "status_led.h"

/* Status LED entry function */
void status_led_entry(void)
{
    while (1)
    {
        g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_08, IOPORT_LEVEL_HIGH);

        tx_thread_sleep(25);

        g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_08, IOPORT_LEVEL_LOW);

        tx_thread_sleep(25);

    }
}

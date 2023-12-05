/***********************************************************************************************************************
* File Name    : blinky_thread_entry.c
* Description  : This is a very simple example application that blinks all the LEDs on a board.
***********************************************************************************************************************/

#include <status_thread.h>

/* @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the the threadx sleep function.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
void status_thread_entry(void)
{

    while (1)
    {
        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_11, IOPORT_LEVEL_HIGH);

        tx_thread_sleep(25);

        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_11, IOPORT_LEVEL_LOW);

        tx_thread_sleep(25);
    }
}

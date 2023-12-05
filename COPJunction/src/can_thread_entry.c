#include "can_thread.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lcd_thread_entry.h"
#include "adc_thread.h"
#include "string.h"
#include "can_thread_entry.h"
#include "adc_thread_entry.h"

static uint8_t ucTxComplete=0, ucRxComplete=0x55;
static volatile ssp_err_t error;
static struct st_can_frame txFrame;
static struct st_can_frame rxFrame;
static volatile uint32_t ulErrors=0;
static can_info_t Info;
static uint32_t ch0=0;
uint8_t tx_data[8], rx_data[8];
int previous;

extern uint16_t usMuxResults[72];


/* Can Thread entry function */
void can_thread_entry(uint8_t floor1)
{

    memset(&txFrame, 0x00, sizeof(txFrame));
    txFrame.id = 0x7DF;
    txFrame.type = CAN_FRAME_TYPE_DATA;
    txFrame.data_length_code = 2;
    txFrame.data[0x00] = floor1;
    txFrame.data[0x01] = 0x8;

    memset(&rxFrame, 0x00, sizeof(rxFrame));
    rxFrame.id = 0x7DF;
    rxFrame.type = CAN_FRAME_TYPE_DATA;
    rxFrame.data_length_code = 2;

    error = g_can0.p_api->open(g_can0.p_ctrl, g_can0.p_cfg);

    error = g_can0.p_api->infoGet(g_can0.p_ctrl, &Info);

    int n=0;
    while (n<15)
    {
        g_can0.p_api->write(g_can0.p_ctrl, 1, &txFrame);
        if(ucTxComplete)
        {
            ucTxComplete = 0;
            //error = g_can0.p_api->write(g_can0.p_ctrl, 1, &txFrame);
        }
        can_rx();
        n++;
    }
}


/*Function to perform CAN reception*/
void can_rx()
{

    // Message received. Increment rx_count.
    //rx_count++;
    if(ucRxComplete)
    {
        ucRxComplete = 0;
        g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_07, IOPORT_LEVEL_HIGH);

        error = g_can0.p_api->read(g_can0.p_ctrl, 0, &rxFrame);
        tx_thread_sleep (25);
        // Store received frame data
        memcpy(&rx_data, &rxFrame.data, sizeof(rx_data));

        if(previous != rx_data[0])
        {
            lcd_thread_entry(rx_data[0]);
        }
        previous =rx_data[0];

        if(rx_data[0] == 8)
        {
            g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_HIGH); //Cabin Light
            g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //Courtesy Light
        }
        else if(rx_data[0]==9)
        {
            g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_LOW);
            g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_LOW);
        }

    }
}


void CanCallback(can_callback_args_t *p_args)
{
    if(p_args->channel == 0x0000)
    {
        ch0++;
        if(p_args->event == CAN_EVENT_TX_COMPLETE)
        {
            ucTxComplete = 0x55;
        }
        else if(p_args->event == CAN_EVENT_RX_COMPLETE)
        {
            ucRxComplete = 0x55;
        }
        else
        {
            ulErrors++;
        }
    }
}


#include "can_thread.h"

static volatile uint8_t ucTxComplete=0x50, ucRxComplete=0;
static volatile ssp_err_t error;
static struct st_can_frame txFrame;
static struct st_can_frame rxFrame;

static can_info_t Info;
static can_command_t Cmd;

static volatile uint32_t ch0=0, canErrors=0;
uint8_t tx_data[8], rx_data[8];

/* Can Thread entry function */
void can_thread_entry(uint8_t floor)
{
    memset(&txFrame, 0x00, sizeof(txFrame));
    txFrame.id = 0x7DF;
    txFrame.type = CAN_FRAME_TYPE_DATA;
    txFrame.data_length_code = 2;
    txFrame.data[0x00] = floor;
    txFrame.data[0x01] = 0x5;

    memset(&rxFrame, 0x00, sizeof(rxFrame));
    rxFrame.id = 0x7DF;
    rxFrame.type = CAN_FRAME_TYPE_DATA;
    rxFrame.data_length_code = 2;

    error = g_can0.p_api->open(g_can0.p_ctrl, g_can0.p_cfg);

    error = g_can0.p_api->infoGet(g_can0.p_ctrl, &Info);

    int n=0;
    while (n<15)
    {

        if(ucTxComplete)
        {
            ucTxComplete = 0;
            error = g_can0.p_api->write(g_can0.p_ctrl, 0, &txFrame);
        }

        if(ucRxComplete)
        {
            ucRxComplete = 0;
        }
        n++;
        //tx_thread_sleep (250);
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
            error = g_can0.p_api->read(g_can0.p_ctrl, 1, &rxFrame);
            ucRxComplete = 0x55;
            tx_thread_sleep (25);
            //Store received frame data
            memcpy(&rx_data, &rxFrame.data, sizeof(rx_data));
        }
        else
        {
            //Error
            canErrors++;
            //g_can0.p_api->control()
        }
    }
}

/* EOF */

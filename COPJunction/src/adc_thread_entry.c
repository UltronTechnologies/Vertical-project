#include "adc_thread.h"

/*****************************************************************************/
/* Static                                                                    */
/*****************************************************************************/
static volatile ssp_err_t error;
//static adc_register_t scanChannelsA[5]={ADC_REG_CHANNEL_0,ADC_REG_CHANNEL_1,ADC_REG_CHANNEL_2,ADC_REG_CHANNEL_3,ADC_REG_CHANNEL_7};
//static adc_register_t scanChannelsB[4]={ADC_REG_CHANNEL_0,ADC_REG_CHANNEL_1,ADC_REG_CHANNEL_2,ADC_REG_CHANNEL_7};
static uint16_t usADCResult1[9];

/*****************************************************************************/
/* Global                                                                    */
/*****************************************************************************/
uint16_t usMuxResults1[72];

/*****************************************************************************/
/* Forwards                                                                  */
/*****************************************************************************/
void setMux(uint8_t value);
void scanADC(void);



/*****************************************************************************/
/* adc_thread_entry                                                          */
/*****************************************************************************/
void adc_thread_entry(void)
{
    uint8_t index;

    while (1)
    {
        for(index=0;index<8;index++)
        {
            /* Select the Multiplexor */
            setMux(index);

            /* Wait */
            tx_thread_sleep(1);

            /* Scan all 9 ADCs */
            scanADC();

            /* Store in the global array */
            memcpy(&usMuxResults1[9*index], &usADCResult1[0], 18);
        }
    }
    //if(usMuxResults1[64] >= 1600)
    //{
        //can_thread_entry(1);
    //}

}

/*****************************************************************************/
/* scanADC                                                                   */
/*****************************************************************************/
void scanADC(void)
{
    error = g_adc0.p_api->open(g_adc0.p_ctrl, g_adc0.p_cfg);
    error = g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);

    error = g_adc0.p_api->scanStart(g_adc0.p_ctrl); //start the scan

    do
    {
        tx_thread_sleep (1);
        error = g_adc0.p_api->scanStatusGet(g_adc0.p_ctrl);
    } while (error != SSP_SUCCESS); //impatiently wait for scan to finish

    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_0, &usADCResult1[0]); // A
    g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_1, &usADCResult1[1]); // B

    error = g_adc0.p_api->scanStop(g_adc0.p_ctrl); //stop the scan

    error = g_adc0.p_api->close(g_adc0.p_ctrl);

}

/*****************************************************************************/
/* setMux                                                                    */
/*****************************************************************************/
void setMux(uint8_t value)
{
    switch(value)
    {
        default:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_LOW); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_LOW); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_LOW); // C
            break;
        case 1:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_HIGH); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_LOW); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_LOW); // C
            break;
        case 2:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_LOW); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_HIGH); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_LOW); // C
            break;
        case 3:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_HIGH); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_HIGH); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_LOW); // C
            break;
        case 4:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_LOW); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_LOW); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_HIGH); // C
            break;
        case 5:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_HIGH); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_LOW); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_HIGH); // C
            break;
        case 6:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_LOW); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_HIGH); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_HIGH); // C
            break;
        case 7:
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_11, IOPORT_LEVEL_HIGH); // A
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_12, IOPORT_LEVEL_HIGH); // B
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_13, IOPORT_LEVEL_HIGH); // C
            break;
    }
}
/* EOF */

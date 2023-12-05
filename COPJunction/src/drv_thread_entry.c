#include "drv_thread.h"
#include "can_thread_entry.h"
#include "adc_thread_entry.h"


/*****************************************************************************/
/* Externals                                                                 */
/*****************************************************************************/
extern uint16_t usMuxResults1[72];

extern void can_thread_entry(uint8_t floor1);

#define FLOOR1  64
#define FLOOR2  28
#define FLOOR3   1
#define FLOOR4  10
#define FLOOR5  19

#define ADC_MINIMUM_IN      1600


/* Drive Thread entry function */
void drv_thread_entry(void)
{

    while (1)
    {
       if(usMuxResults1[FLOOR1] >= ADC_MINIMUM_IN)
       {
           int n=0;
           while(n<=1)
           {
               can_thread_entry(1);
               n++;
           }
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_HIGH); //Cabin Light
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //Courtesy Light
       }

       else if(usMuxResults1[FLOOR2] >= ADC_MINIMUM_IN)
       {
           int n=0;
           while(n<=1)
           {
               can_thread_entry(2);
               n++;
           }
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_HIGH);
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH);
       }

       else if(usMuxResults1[FLOOR3] >= ADC_MINIMUM_IN)
       {
           int n=0;
           while(n<=1)
           {
               can_thread_entry(3);
               n++;
           }
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_HIGH);
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH);
       }

       else if(usMuxResults1[FLOOR4] >= ADC_MINIMUM_IN)
       {
           int n=0;
           while(n<=1)
           {
               can_thread_entry(4);
               n++;
           }
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_05, IOPORT_LEVEL_HIGH);
           //g_ioport.p_api->pinWrite(IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH);
       }

       else
       {
           int n=0;
           while(n<=1)
           {
               can_thread_entry(0);
               n++;
           }
       }

        tx_thread_sleep (1);
    }
}

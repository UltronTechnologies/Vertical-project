#include "drv_thread.h"
#include "drv_thread_entry.h"
#include "can_thread_entry.h"
#include "adc_thread_entry.h"


/*****************************************************************************/
/* Defs                                                                      */
/*****************************************************************************/
#define REG_INPUT           0x00
#define REG_OUTPUT          0x01
#define REG_INVERSION       0x02
#define REG_DIRECTION       0x03

#define SLAVE_OUT1          0x20 //0x40
#define SLAVE_OUT2          0x21 //0x42

#define INTER_RELAY_DELAY   50
#define SLEEP_DELAY         2

#define DEBOUNCE_COUNT      5

#define ADC_MINIMUM_IN      1600
#define INTER_RELAY_DELAY1
/*****************************************************************************/
/* Static                                                                    */
/*****************************************************************************/
static volatile ssp_err_t error;
static uint8_t uc_Buffer[10], ucRunUp=0;

/*****************************************************************************/
/* Externals                                                                 */
/*****************************************************************************/
extern uint16_t usMuxResults[72];

extern void can_thread_entry(uint8_t floor);

/*****************************************************************************/
/* Global                                                                    */
/*****************************************************************************/
uint8_t uc_Outputs[2] = {0x40, 0x00};
uint8_t ucRun=0;
uint8_t ucDebounceUpHi=0, ucDebounceUpLo=0, ucDebounceDownHi=0, ucDebounceDownLo=0;
uint8_t previous_floor=0, current_floor=0;
static int call=1;
static int pendant_cop_f1=0;
static int pendant_cop_f2=0;
static int pendant_cop_f3=0;
static int pendant_cop_f4=0;
static int door_reopen_cop_f1=0;
static int door_reopen_cop_f2=0;
static int call_1=0;
static int number=0;
static int number1=0;
static int coming_up=0;
static int coming_down=0;
static int counter=0;
static int counter1=0;
static int counter2=0;
static int counter3=0;
static int arr[2]={0};
static int array[2]={0};
static int arr1[2]={0};
static int array1[2]={0};
/****************************************************************************/
/* forwards                                                                 */
/****************************************************************************/
void i2cWriteOutputBuffer(uint8_t slave, uint8_t reg, uint8_t dbyte);

/****************************************************************************/
/* drv_thread_entry                                                         */
/****************************************************************************/
void drv_thread_entry(void)
{

    i2cWriteOutputBuffer(SLAVE_OUT1, REG_OUTPUT, uc_Outputs[0]);
    i2cWriteOutputBuffer(SLAVE_OUT2, REG_OUTPUT, uc_Outputs[1]);

    i2cWriteOutputBuffer(SLAVE_OUT1, REG_DIRECTION, 0x00);
    i2cWriteOutputBuffer(SLAVE_OUT2, REG_DIRECTION, 0x00);

    while (1)
    {


        previous_floor=current_floor;
        i2cWriteOutputBuffer(SLAVE_OUT1, REG_OUTPUT, uc_Outputs[0]);
        i2cWriteOutputBuffer(SLAVE_OUT2, REG_OUTPUT, uc_Outputs[1]);
        tx_thread_sleep(2);

        //Button Down
        if(usMuxResults[3] >= ADC_MINIMUM_IN)
        {
            pendant_cop_f1=0;
            pendant_cop_f2=0;
            pendant_cop_f3=0;
            pendant_cop_f4=0;
            can_thread_entry(8);
            callfloor0();
        }

        //Button Up
        if(usMuxResults[12] >= ADC_MINIMUM_IN)
        {
            pendant_cop_f1=0;
            pendant_cop_f2=0;
            pendant_cop_f3=0;
            pendant_cop_f4=0;
            can_thread_entry(8);
            callfloor01();
        }

        // Call Floor 1
       if((usMuxResults[37] >= ADC_MINIMUM_IN) && (usMuxResults[29] <= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN))
       {
           can_thread_entry(8);
           call_1=0;
           callfloor1();
       }

        // Call Floor 2
        if((usMuxResults[63] >= ADC_MINIMUM_IN) && (usMuxResults[2] <= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN))
        {
            can_thread_entry(8);
            if((coming_down==1) && (call==4))
            {
                coming_down=0;
                coming_up=1;

            }
            else if((coming_down==1) && (call==3))
            {
                coming_down=0;
                coming_up=1;

            }
            else if((coming_up==1) && (call==1))
            {
                coming_up=0;
                coming_down=1;
            }
            call_1=0;
            callfloor2();
        }

        // Call Floor 3
        if((usMuxResults[0] >= ADC_MINIMUM_IN) && (usMuxResults[11] <= ADC_MINIMUM_IN) && (usMuxResults[56] <= ADC_MINIMUM_IN))
        {
            can_thread_entry(8);
            if((coming_up==1) && (call==2))
            {
                coming_up=0;
                coming_down=1;
            }
            else if((coming_down==1) && (call==4))
            {
                coming_down=0;
                coming_up=1;
            }
            call_1=0;
            callfloor3();
        }

        // Call Floor 4
        if((usMuxResults[41] >= ADC_MINIMUM_IN) && (usMuxResults[20] <= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN))
        {
            can_thread_entry(8);
            call_1=0;
            callfloor4();
        }


        /****************************************************************************/
        /* STOPPING LIFT                                                            */
        /****************************************************************************/

        if ((usMuxResults[17] >= ADC_MINIMUM_IN) || (usMuxResults[43] >= ADC_MINIMUM_IN))
        {
            if ((rx_data[0] == 4) || (rx_data[0] == 3) || (rx_data[0] == 2) || (rx_data[0] == 1) || ((rx_data[0] == 0) && (call_1==41) && (pendant_cop_f4==1)) || ((rx_data[0] == 0) && (call_1==31) && (pendant_cop_f3==1)) || ((rx_data[0] == 0) && (call_1==21) && (pendant_cop_f2==1)) || ((rx_data[0] == 0) && (call_1==11) && (pendant_cop_f1==1)))
            {
                stoppinglift_1(call_1);
            }
            else if ((number==5) && (ucRun==2))
            {
                stoppinglift_down(number);
            }
            else if((number1==6) && (ucRun==1))
            {
                stoppinglift_up(number1);
            }
            else
            {
                stoppinglift(call);
            }
        }


        /****************************************************************************/
        /* REOPEN DOORS                                                             */
        /****************************************************************************/

        //REOPEN DOOR 4
        if ((usMuxResults[41] >= ADC_MINIMUM_IN) && (usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
        {
            dooropen(0x80,7);
            dooroperate(0x80,7);
            int n=0;
            while(n<=1)
            {
                can_thread_entry(4);
                n++;
            }

        }
        if ((usMuxResults[41] <= ADC_MINIMUM_IN) && (usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
        {
            if ((counter ==1) || (counter1==1))
            {
                counter=0;
                counter1=0;
            }
        }

        //REOPEN DOOR 3
        if((usMuxResults[0] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN))
        {
            int n=0;
            while(n<=1)
            {
                can_thread_entry(3);
                n++;
            }
            dooropen(0x20,5);
            dooroperate(0x20,5);
        }
        if((usMuxResults[0] <= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN))
        {
            if ((counter ==1) || (counter1==1))
            {
                counter=0;
                counter1=0;
            }
        }

        //REOPEN DOOR 2
        if ((usMuxResults[63] >= ADC_MINIMUM_IN) && (usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))
        {
            int n=0;
            while(n<=1)
            {
                can_thread_entry(2);
                n++;
            }
            dooropen(0x8,3);
            dooroperate(0x8,3);
        }
        if ((usMuxResults[63] <= ADC_MINIMUM_IN) && (usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))
        {
            if ((counter ==1) || (counter1==1))
            {
                counter=0;
                counter1=0;
            }
        }


        //REOPEN DOOR 2 From COP
        if((rx_data[0] == 2) && (usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))
        {
            door_reopen_cop_f2=1;
            int n=0;
            while(n<=1)
            {
                can_thread_entry(2);
                n++;
            }
            dooropen1(0x8,3);
            dooroperate1(0x8,3);
        }
        if ((rx_data[0] == 0) && (usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))
        {
            if ((counter2 ==1) || (counter3==1))
            {
                counter2=0;
                counter3=0;
                door_reopen_cop_f2=0;
            }
        }



        //REOPEN DOOR 1
        if((usMuxResults[37] >= ADC_MINIMUM_IN) && (usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))
        {
            int n=0;
            while(n<=1)
            {
                can_thread_entry(1);
                n++;
            }
            dooropen(0x2, 1);
            dooroperate(0x2, 1);
        }
        if((usMuxResults[37] <= ADC_MINIMUM_IN) && (usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))
        {
            if ((counter ==1) || (counter1==1))
            {
                counter=0;
                counter1=0;
            }
        }


        //REOPEN DOOR 1 From COP
        if((rx_data[0] == 1) && (usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))
        {
            door_reopen_cop_f1=1;
            int n=0;
            while(n<=1)
            {
                can_thread_entry(1);
                n++;
            }
            dooropen1(0x2, 1);
            dooroperate1(0x2, 1);
        }
        if((rx_data[0] == 0) && (usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))
        {
            if ((counter2 ==1) || (counter3==1))
            {
                counter2=0;
                counter3=0;
                door_reopen_cop_f1=0;
            }
        }

        /****************************************************************************/
        /* FLOOR CONTROLS FROM COP                                                  */
        /****************************************************************************/

        // Call Floor 4 from COP
        if((rx_data[0] == 4) && (usMuxResults[20] <= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN))
        {
            call=0;
            callfloor_4();
            rx_data[0]=0;
        }

        // Call Floor 3 from COP
        if((rx_data[0] == 3)&& (usMuxResults[56] <= ADC_MINIMUM_IN) && (usMuxResults[11] <= ADC_MINIMUM_IN))
        {
            if((coming_up==1) && (call_1==21))
            {
                coming_up=0;
                coming_down=1;
            }
            else if((coming_down==1) && (call_1==41))
            {
                coming_down=0;
                coming_up=1;
            }
            call=0;
            callfloor_3();
            rx_data[0]=0;
        }

        // Call Floor 2 from COP
        if((rx_data[0] == 2) && (usMuxResults[2] <= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN))
        {
            if((coming_down==1) && (call_1==41))
            {
                coming_down=0;
                coming_up=1;
            }
            else if((coming_down==1) && (call_1==31))
            {
                coming_down=0;
                coming_up=1;
            }
            else if((coming_up==1) && (call_1==11))
            {
                coming_up=0;
                coming_down=1;
            }
            call=0;
            callfloor_2();
            rx_data[0]=0;
        }

        // Call Floor 1 from COP
        if((rx_data[0] == 1) && (usMuxResults[29] <= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN))
        {
            call=0;
            callfloor_1();
            rx_data[0]=0;
        }

        /*************************************************************************************************************************/

    }
}


/****************************************************************************/
/* i2cWriteOutputBuffer                                                     */
/****************************************************************************/
void i2cWriteOutputBuffer(uint8_t slave, uint8_t reg, uint8_t dbyte)
{
    uc_Buffer[0] = reg;
    uc_Buffer[1] = dbyte;

    error = g_i2c0.p_api->open(g_i2c0.p_ctrl, g_i2c0.p_cfg);

    error = g_i2c0.p_api->slaveAddressSet(g_i2c0.p_ctrl, slave, I2C_ADDR_MODE_7BIT);

    // short delay before starting to read from slave device
    R_BSP_SoftwareDelay (2, BSP_DELAY_UNITS_MILLISECONDS);

    error = g_i2c0.p_api->write(g_i2c0.p_ctrl, &uc_Buffer[0], 2, false);

    error = g_i2c0.p_api->close(g_i2c0.p_ctrl);
}


void LiftDown()
{
    switch(ucRunUp)
    {
        case 0:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_02, IOPORT_LEVEL_HIGH); // Power Relay 1
            ucRunUp++;
            break;
        case 1:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 2:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_00, IOPORT_LEVEL_HIGH); // Power Relay 2
            ucRunUp++;
            break;
        case 3:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 4:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_HIGH); // Power Contactor 1
            ucRunUp++;
            break;
        case 5:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 6:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_11, IOPORT_LEVEL_HIGH); // Down Relay 1
            ucRunUp++;
            break;
        case 7:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 8:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_13, IOPORT_LEVEL_HIGH); // Down Relay 2
            ucRunUp++;
            break;
        case 9:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 10:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_01, IOPORT_LEVEL_HIGH); // Down Contactor
            ucRunUp++;
            break;
        case 11:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 12:
            tx_thread_sleep(SLEEP_DELAY);
            break;
    }
}


void LiftUp()
{
    switch(ucRunUp)
    {
        case 0:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_02, IOPORT_LEVEL_HIGH); // Power Relay 1
            ucRunUp++;
            break;
        case 1:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 2:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_00, IOPORT_LEVEL_HIGH); // Power Relay 2
            ucRunUp++;
            break;
        case 3:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 4:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_HIGH); // Power Contactor 1
            ucRunUp++;
            break;
        case 5:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 6:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_05, IOPORT_LEVEL_HIGH); // Up Relay 1
            ucRunUp++;
            break;
        case 7:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 8:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_03, IOPORT_LEVEL_HIGH); // Up Relay 2
            ucRunUp++;
            break;
        case 9:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 10:
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_10, IOPORT_LEVEL_HIGH); // Up Pressure 1
            ucRunUp++;
            break;
        case 11:
            tx_thread_sleep(INTER_RELAY_DELAY);
            ucRunUp++;
            break;
        case 12:
            tx_thread_sleep(SLEEP_DELAY);
            break;
    }

}

void LiftStop()
{
    ucRunUp=0;
    //ucRunDown=0;
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_02, IOPORT_LEVEL_LOW); // Power Relay 1
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_00, IOPORT_LEVEL_LOW); // Power Relay 2
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_LOW); // Power Contactor 1
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_05, IOPORT_LEVEL_LOW); // Up Relay 1
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_03, IOPORT_LEVEL_LOW); // Up Relay 2
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_10, IOPORT_LEVEL_LOW); // Up Pressure 1
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_11, IOPORT_LEVEL_LOW); // Down Relay 1
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_13, IOPORT_LEVEL_LOW); // Down Relay 2
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_01, IOPORT_LEVEL_LOW);
    ucRun=0;
}


int callfloor0()
{
    number=5;
    ucRun=0;
    if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN)) // FLOOR1_LEVELDOWN_CPU
    {
        if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((number==5))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
    }

    else if(ucRun == 2)
    {
        LiftDown();
    }
    return number;
}


int callfloor1()
{
    number=0;
    number1=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;
    call=1;
    ucRun=0;
    if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN)) // FLOOR1_LEVELDOWN_CPU
    {
        if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call==4) || (call==3) || (call==2) || (call==1) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[29] <= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
    }
    return call;
}


int callfloor_1()
{
    number=0;
    number1=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;
    call_1=11;
    ucRun=0;
    if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN)) // FLOOR1_LEVELDOWN_CPU
    {
        if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] <= ADC_MINIMUM_IN))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call_1==41) || (call_1==31) || (call_1==21) || (call_1==11) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[29] <= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
        pendant_cop_f1=1;
    }
    return call_1;
}



int callfloor2()
{
    number=0;
    number1=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;
    call=2;
    current_floor=1;
    ucRun=0;
    if((coming_down==1) || (call==1) || (call==2) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN)))
    {
        if((coming_down==1) || (call==1) || (call==2) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call==4) || (call==3) || (call==1) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] <= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
    }
    return call;
}


int callfloor_2()
{
    pendant_cop_f2=1;
    number=0;
    number1=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;
    call_1=21;
    current_floor=1;
    ucRun=0;
    if((coming_down==1) || (call_1==11) || (call_1==21) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN)))
    {
        if((coming_down==1) || (call_1==11) || (call_1==21) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call_1==41) || (call_1==31) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] <= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
    }
    return call_1;
}




int callfloor3()
{
    number=0;
    number1=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;
    call=3;
    ucRun=0;
    if((coming_down==1) || (call==1) || (call==2) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] <= ADC_MINIMUM_IN)))
    {
        if((coming_down==1) || (call==1) || (call==2) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call==4) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] <= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }

    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;

    }
    return call;
}


int callfloor_3()
{
    pendant_cop_f3=1;
    number=0;
    number1=0;
    call_1=31;
    ucRun=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;

    if((coming_down==1) || (call_1==11) || (call_1==21) ||((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] <= ADC_MINIMUM_IN)))
    {
        if((coming_down==1) || (call_1==11) || (call_1==21) ||((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[56] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((coming_up==1) || (call_1==41) ||((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) || ((usMuxResults[11] <= ADC_MINIMUM_IN) && (usMuxResults[56] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }


    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;

    }
    return call_1;
}



int callfloor4()
{
    number=0;
    number1=0;
    call=4;
    ucRun=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;

    if((call==1) ||(call==2) || (call==3) || (call==4) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN)))
    {
        if((call==1) ||(call==2) || (call==3) || (call==4) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if(((usMuxResults[41] >= ADC_MINIMUM_IN) && (usMuxResults[20] <= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }
    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
    }
    return call;
}


int callfloor_4()
{
    pendant_cop_f4=1;
    number=0;
    number1=0;
    call_1=41;
    ucRun=0;
    counter=0;
    counter1=0;
    counter2=0;
    counter3=0;

    if((call_1==11) ||(call_1==21) || (call_1==31) || (call_1==41) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN)))
    {
        if((call_1==11) ||(call_1==21) || (call_1==31) || (call_1==41) || ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN)) || ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN)) || ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN)) || ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] <= ADC_MINIMUM_IN)))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((usMuxResults[20] <= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }
    if(ucRun == 1)
    {
        LiftUp();
        coming_down=1;
    }

    else if(ucRun == 2)
    {
        LiftDown();
        coming_up=1;
    }
    return call_1;
}

int callfloor01()
{
    number1=6;
    ucRun=0;

    if((number1==6))
    {
        if((number1==6))
        {
            ucDebounceUpLo = 0;
            if(ucDebounceUpHi >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun |= 0x01;
            }
            else
            {
                ucDebounceUpHi++;
            }
        }
        else
        {
            ucDebounceUpHi = 0;
            if(ucDebounceUpLo >= DEBOUNCE_COUNT)
            {
                //Up
                ucRun &= (uint8_t)(~0x01 & 0xFF);
            }
            else
            {
                ucDebounceUpLo++;
            }
        }
    }
    else
    {
        if((usMuxResults[20] <= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
        {
            ucDebounceDownLo = 0x00;
            if(ucDebounceDownHi >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun |= 0x02;
            }
            else
            {
                ucDebounceDownHi++;
            }
        }
        else
        {
            ucDebounceDownHi = 0;
            if(ucDebounceDownLo >= DEBOUNCE_COUNT)
            {
                //Down
                ucRun &= (uint8_t)(~0x02 & 0xFF);
            }
            else
            {
                ucDebounceDownLo++;
            }
        }
    }
    if(ucRun == 1)
    {
        LiftUp();
    }

    else if(ucRun == 2)
    {
        LiftDown();
    }
    return number1;
}


void stoppinglift_down(int number)
{
    if (((number==5) && (usMuxResults[3] <= ADC_MINIMUM_IN)))
    {
        LiftStop();
        can_thread_entry(9);
    }
}

void stoppinglift_up(int number1)
{
    if (((number1==6) && (usMuxResults[12] <= ADC_MINIMUM_IN)))
    {
        LiftStop();
        can_thread_entry(9);
    }
}

// Stops lift
void stoppinglift(int num)
{
    if (((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN) && (num==1) && (usMuxResults[37] >= ADC_MINIMUM_IN)) || ((num==1) && (usMuxResults[37] <= ADC_MINIMUM_IN)))
    {
        if (((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN) && (usMuxResults[37] >= ADC_MINIMUM_IN)) || ((usMuxResults[17] >= ADC_MINIMUM_IN)&& (usMuxResults[37] >= ADC_MINIMUM_IN)) || ((usMuxResults[43] >= ADC_MINIMUM_IN)&& (usMuxResults[37] >= ADC_MINIMUM_IN)) || ((usMuxResults[37] <= ADC_MINIMUM_IN)))
        {
            if((number!=5) && (number1!=6))
            {
                LiftStop();
                can_thread_entry(9);
            }
            if((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(1);
                    n++;
                }
                can_thread_entry(9);
            }
        }
    }
    else if (((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN) && (num==2) && (usMuxResults[63] >= ADC_MINIMUM_IN)) || ((num==2) && (usMuxResults[63] <= ADC_MINIMUM_IN)))
    {
        if (((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN) && (usMuxResults[63] >= ADC_MINIMUM_IN)) || ((usMuxResults[17] >= ADC_MINIMUM_IN) && (usMuxResults[63] >= ADC_MINIMUM_IN)) || ((usMuxResults[43] >= ADC_MINIMUM_IN)&& (usMuxResults[63] >= ADC_MINIMUM_IN)) || ((usMuxResults[63] <= ADC_MINIMUM_IN)))
        {
            if((number!=5) && (number1!=6))
            {
                LiftStop();
                can_thread_entry(9);
            }
            if ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(2);
                    n++;
                }
                can_thread_entry(9);
            }
        }
    }
    else if (((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN)&& (num==3) && (usMuxResults[0] >= ADC_MINIMUM_IN)) || ((num==3) && (usMuxResults[0] <= ADC_MINIMUM_IN)))
    {
        if (((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN) && (usMuxResults[0] >= ADC_MINIMUM_IN)) || ((usMuxResults[17] >= ADC_MINIMUM_IN)&& (usMuxResults[0] >= ADC_MINIMUM_IN)) || ((usMuxResults[43] >= ADC_MINIMUM_IN)&& (usMuxResults[0] >= ADC_MINIMUM_IN)) || ((usMuxResults[0] <= ADC_MINIMUM_IN)))
        {
            if((number!=5) && (number1!=6))
            {
                LiftStop();
                can_thread_entry(9);
            }

            if((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(3);
                    n++;
                }
                can_thread_entry(9);
            }
        }
    }
    else if (((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN) && (num==4)&& (usMuxResults[41] >= ADC_MINIMUM_IN)) || ((num==4) && (usMuxResults[41] <= ADC_MINIMUM_IN)))
    {
        if (((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN) && (usMuxResults[41] >= ADC_MINIMUM_IN)) || ((usMuxResults[17] >= ADC_MINIMUM_IN)&& (usMuxResults[41] >= ADC_MINIMUM_IN)) || ((usMuxResults[43] >= ADC_MINIMUM_IN)&& (usMuxResults[41] >= ADC_MINIMUM_IN)) || ((usMuxResults[41] <= ADC_MINIMUM_IN)))
        {
            if((number!=5) && (number1!=6))
            {
                LiftStop();
                can_thread_entry(9);
            }
            if((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(4);
                    n++;
                }
                can_thread_entry(9);
            }
        }
    }
}


// Stops lift
void stoppinglift_1(int num)
{

    if (((num==11) && ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))&& (rx_data[0] == 1)) ||  ((rx_data[0] == 0) && (num==11)&& (pendant_cop_f1==1)))
    {
        if (((num==11) && ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN))&& (rx_data[0] == 1)) ||  ((rx_data[0] == 0) && (num==11) && (pendant_cop_f1==1)))
        {
            LiftStop();
            //can_thread_entry(9);
            if ((usMuxResults[29] >= ADC_MINIMUM_IN) && (usMuxResults[47] >= ADC_MINIMUM_IN) && (ucRun==1))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(1);
                    n++;
                }
                dooropen1(0x2, 1);
                dooroperate1(0x2, 1);
            }
        }
    }
    else if (((num==21) && ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))&& (rx_data[0] == 2)) ||  ((rx_data[0] == 0) && (num==21) && (pendant_cop_f2==1)))
    {
        if (((num==21) && ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN))&& (rx_data[0] == 2)) ||  ((rx_data[0] == 0) && (num==21) && (pendant_cop_f2==1)))
        {

            LiftStop();
            //can_thread_entry(9);
            if ((usMuxResults[2] >= ADC_MINIMUM_IN) && (usMuxResults[65] >= ADC_MINIMUM_IN) && (ucRun==1))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(2);
                    n++;
                }
                dooropen1(0x8,3);
                dooroperate1(0x8,3);
            }
        }
    }
    else if (((num==31) && ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN))&& (rx_data[0] == 3)) ||  ((rx_data[0] == 0) && (num==31) && (pendant_cop_f3==1)))
    {
        if (((num==31) && ((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN))&& (rx_data[0] == 3)) ||  ((rx_data[0] == 0) && (num==31) && (pendant_cop_f3==1)))
        {
            LiftStop();
            //can_thread_entry(9);
            if((usMuxResults[56] >= ADC_MINIMUM_IN) && (usMuxResults[11] >= ADC_MINIMUM_IN) )
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(3);
                    n++;
                }
                dooropen1(0x20,5);
                dooroperate1(0x20,5);
            }
        }
    }
    else if (((num==41) && ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) && (rx_data[0] == 4)) ||  ((rx_data[0] == 0) && (num==41) && (pendant_cop_f4==1)))
    {
        if (((num==41) && ((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN)) && (rx_data[0] == 4)) ||  ((rx_data[0] == 0) && (num==41) && (pendant_cop_f4==1)))
        {
            LiftStop();
            //can_thread_entry(9);
            if((usMuxResults[20] >= ADC_MINIMUM_IN) && (usMuxResults[38] >= ADC_MINIMUM_IN))
            {
                coming_up=0;
                coming_down=0;
                int n=0;
                while(n<=1)
                {
                    can_thread_entry(4);
                    n++;
                }
                dooropen1(0x80,7);
                dooroperate1(0x80,7);
            }
        }
    }
}


void dooropen(uint8_t code, uint8_t shift)
{
    switch(arr[0])
    {
        case 0:
            if((counter==0))
            {
                arr[0]++;
            }
                break;
        case 1:
            if(((call==4) && (counter==0)) || ((counter==0) && (counter2==1)))
            {
                uc_Outputs[0] |= code;
                arr[0]++;
                counter++;
            }
            else if((call==3) && (counter==0))
            {
                uc_Outputs[0] |= code;
                arr[0]++;
                counter++;
            }
            else if(((call==2) && (counter==0)))
            {
                uc_Outputs[0] |= code;
                arr[0]++;
                counter++;
            }
            else if(((call==1) && (counter==0)) || ((pendant_cop_f1==1) && (counter==0)))
            {
                uc_Outputs[0] |= code;
                arr[0]++;
                counter++;
            }

            break;
        case 2:
            if((call==4) || ((counter==1) && (counter2==1)))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr[0]++;
                //R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
            }
            else if((call==3))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr[0]++;
            }
            else if((call==2))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr[0]++;
            }
            else if((call==1) || (pendant_cop_f1==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr[0]++;
            }
            break;
        case 3:
            arr[0]=0;
            break;

    }
}


void dooropen1(uint8_t code, uint8_t shift)
{
    switch(arr1[0])
    {
        case 0:
            if((counter2==0))
            {
                arr1[0]++;
            }
                break;
        case 1:
            if((call_1==41) && (counter2==0))
            {
                uc_Outputs[0] |= code;
                arr1[0]++;
                counter2++;
            }
            else if((call_1==31) && (counter2==0))
            {
                uc_Outputs[0] |= code;
                arr1[0]++;
                counter2++;
            }
            else if(((call_1==21) && (counter2==0)) || ((counter2==0) && (door_reopen_cop_f2==1)))
            {
                uc_Outputs[0] |= code;
                arr1[0]++;
                counter2++;
            }
            else if(((call_1==11) && (counter2==0)) || ((counter2==0) && (door_reopen_cop_f1==1)))
            {
                uc_Outputs[0] |= code;
                arr1[0]++;
                counter2++;
            }

            break;
        case 2:
            if((call_1==41))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr1[0]++;
                //R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
            }
            else if((call_1==31))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr1[0]++;
            }
            else if((call_1==21) || (door_reopen_cop_f2==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr1[0]++;
            }
            else if((call_1==11) || (door_reopen_cop_f1==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[0] &= (uint8_t)(~(1 << shift));
                arr1[0]++;
            }
            break;
        case 3:
            arr1[0]=0;
            break;

    }
}


void dooroperate(uint8_t code, uint8_t shift)
{
    switch(arr[1])
    {
        case 0:
            if ((counter1==0))
            {
                arr[1]++;
            }
            break;
        case 1:
            if(((call==4) && (counter1==0)) || ((counter1==0) && (counter3==1)))
            {
                //R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr[1]++;
                counter1++;

            }
            else if((call==3) && (counter1==0))
            {
                //R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr[1]++;
                counter1++;
            }
            else if(((call==2) && (counter1==0)))
            {
                //R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr[1]++;
                counter1++;
            }
            else if(((call==1) && (counter1==0)) || ((pendant_cop_f1==1) && (counter1==0)))
            {
                //R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr[1]++;
                counter1++;
            }
            break;
        case 2:
            if((call==4) || ((counter1==1) && (counter3==1)))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr[1]++;
            }
            else if(call==3)
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr[1]++;
            }
            else if((call==2))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr[1]++;
            }
            else if((call==1) || (pendant_cop_f1==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr[1]++;
            }
            break;
        case 3:
            arr[1]=0;
            break;
    }
}



void dooroperate1(uint8_t code, uint8_t shift)
{
    switch(arr1[1])
    {
        case 0:
            if ((counter3==0))
            {
                arr1[1]++;
            }
            break;
        case 1:
            if((call_1==41) && (counter3==0))
            {
                //R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr1[1]++;
                counter3++;

            }
            else if((call_1==31) && (counter3==0))
            {
                //R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] |= code;
                arr1[1]++;
                counter3++;
            }
            else if(((call_1==21) && (counter3==0)) || ((counter3==0) && (door_reopen_cop_f2==1)))
            {
                uc_Outputs[1] |= code;
                arr1[1]++;
                counter3++;
            }
            else if(((call_1==11) && (counter3==0)) || ((counter3==0) && (door_reopen_cop_f1==1)))
            {
                uc_Outputs[1] |= code;
                arr1[1]++;
                counter3++;
            }
            break;
        case 2:
            if(call_1==41)
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr1[1]++;
            }
            else if(call_1==31)
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr1[1]++;
            }
            else if((call_1==21) || (door_reopen_cop_f2==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr1[1]++;
            }
            else if((call_1==11) || (door_reopen_cop_f1==1))
            {
                R_BSP_SoftwareDelay(2,BSP_DELAY_UNITS_SECONDS);
                uc_Outputs[1] &= (uint8_t)(~(1 << shift));
                arr1[1]++;
            }
            break;
        case 3:
            arr1[1]=0;
            break;
    }
}

/* EOF */

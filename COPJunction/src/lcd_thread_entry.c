#include "lcd_thread.h"

static volatile ssp_err_t error;
static uint8_t i2cdata[10];
static volatile uint16_t data, ptr;
static volatile uint16_t address=63, delay=125;
static volatile uint32_t ulAborted=0, ulSent=0;

/* Forwards */
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_Transfer(uint8_t chr);
void LCD_Command(uint8_t chr);
void LCD_Data(uint8_t chr);
void disp_char(uint8_t line, uint8_t column, char *dp);
void Disp_XY(uint8_t posx, uint8_t posy);
void DispOneChar(uint8_t x, uint8_t y, uint8_t Wdata);

/* LCD Thread entry function */
void lcd_thread_entry(uint8_t floor)
{
    tx_thread_sleep(5);
    LCD_Command (0x30);
    tx_thread_sleep(5);
    LCD_Command (0x30);
    tx_thread_sleep(5);
    LCD_Command (0x30);
    tx_thread_sleep(5);
    LCD_Command (0x20);
    tx_thread_sleep(5);
    LCD_Command(0x28); // Function set
    tx_thread_sleep(5);

    LCD_Command(0x0c); // Display ON/OFF
    tx_thread_sleep(5);
    LCD_Command(0x01); // Clear display
    tx_thread_sleep(10);
    LCD_Command(0x06); // set display mode
    tx_thread_sleep(5);

    if (floor == 1)
    {
        disp_char(1, 1, "Floor 1           ");
        tx_thread_sleep(25);
    }

    else if (floor == 2)
    {
        disp_char(1, 1, "Floor 2           ");
        tx_thread_sleep(25);
    }
    else if (floor == 3)
    {
        disp_char(1, 1, "Floor 3           ");
        tx_thread_sleep(25);
    }
    else if (floor == 4)
    {
        disp_char(1, 1, "Floor 4           ");
        tx_thread_sleep(25);
    }
    else
    {
        disp_char(1, 1, "Vertical Lift      ");
        tx_thread_sleep(25);
    }
}

void IIC_Start(void)
{
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_HIGH); // SDA=1;
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_HIGH); //SCL=1;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_LOW); // SDA=0;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_LOW); //SCL=0;
    R_BSP_SoftwareDelay(10, 1);
}
void IIC_Stop(void)
{
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_LOW); // SDA=0;
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_HIGH); //SCL=1;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_HIGH); // SDA=1;
}
void IIC_Ack(void)
{
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_LOW); // SDA=0;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_HIGH); //SCL=1;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_LOW); //SCL=0;
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_HIGH); // SDA=1;
}
void IIC_Transfer(uint8_t chr)
{
    uint8_t j;

    data = chr;
    ptr = 0x80;
    for(j=0;j<8;j++)
    {
        g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_LOW); //SCL=0;
        if((data & ptr) != 0x00)
        {
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_HIGH); // SDA=1;
        }
        else
        {
            g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_LOW); // SDA=0;
        }
        //data <<= 1;
        ptr >>= 1;
        R_BSP_SoftwareDelay(10, 1);
        g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_HIGH); //SCL=1;
        R_BSP_SoftwareDelay(10, 1);
    }
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_LOW); //SCL=0;
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_01, IOPORT_LEVEL_LOW); // SDA=0;
    R_BSP_SoftwareDelay(10, 1);
    g_ioport.p_api->pinWrite(IOPORT_PORT_01_PIN_00, IOPORT_LEVEL_HIGH); //SCL=1;
   // R_BSP_SoftwareDelay(10, 1);
}
void LCD_Command(uint8_t chr)
{
    IIC_Start();
    IIC_Transfer(0x7C);
    IIC_Ack();
    IIC_Transfer(0x00);
    IIC_Ack();
    IIC_Transfer(chr);
    IIC_Ack();
    IIC_Stop();
}
void LCD_Data(uint8_t chr)
{
    IIC_Start();
    IIC_Transfer(0x7C); // DC=0; RW=0   选择SLAVE ADDRESS
    IIC_Ack();
    IIC_Transfer(0x40);
    IIC_Ack();
    IIC_Transfer(chr);
    IIC_Ack();
    IIC_Stop();
}
void disp_char(uint8_t line, uint8_t column, char *dp)
{
    uint8_t i;

    LCD_Command(0x80+((line-1)*0x40) +(column-1));  // 0X80:First row  address  0XC0:Second row address
    tx_thread_sleep(2);
    for(i=0;i<16;i++)
    {
        LCD_Data((uint8_t)*dp);
        dp=dp+1;
        tx_thread_sleep(2);
    }
}
void Disp_XY(uint8_t posx, uint8_t posy)
{
    uint8_t temp;

    temp=posx;                     //&0x1f; 原为限制的条件
    posy &= 0x1;
    if ( posy )
    {
        temp |= 0xc0;
    }
    else
    {
        temp |= 0x80;
    }
    LCD_Command(temp);

}
void DispOneChar(uint8_t x, uint8_t y, uint8_t Wdata)
{
    Disp_XY(x, y);
    LCD_Data(Wdata);
}

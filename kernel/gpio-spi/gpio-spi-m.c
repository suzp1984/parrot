#include <linux/module.h> 
#include <linux/types.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/ioport.h> 
#include <asm/uaccess.h> 
#include <linux/delay.h> 
#include <bsp.h> 
#include <asm/io.h>
#define PDEBUG 
#ifdef PDEBUG 
#define PLOG(fmt,args...) printk(fmt,##args) 
#else 
#define PLOG(fmt,args...) /*do nothing*/ 
#endif
#define SPI_CMD 0 
#define SPI_DATA 1
#define FUN_GPIO    0 
#define PIN_SDO 15 //GPIOF[15]
#define PIN_SDI 14 
#define PIN_SCLK 16 
#define PIN_CS 29 //GPIOC[29]

#define GPC_BASE             0xF0102080 
#define GPF_BASE             0xF0102140 
#define OFFSET_DAT            0x0 
#define OFFSET_EN            0x4 
#define OFFSET_FUN0            0x24 
#define OFFSET_FUN1            0x28 
#define OFFSET_FUN2            0x2c 
#define OFFSET_FUN3            0x30
// select pin used for gpio
static int tcc_set_pin_fun(int pin, int fun) 
{
    if(pin<8) 
        tcc_writel(((tcc_readl(GPF_BASE+OFFSET_FUN0) & ~(0x0f<<(4*pin))) | (fun<<(4 * pin))), 
                   GPF_BASE+OFFSET_FUN0); 
    else if(pin<16) 
        tcc_writel(((tcc_readl(GPF_BASE+OFFSET_FUN1) & ~(0x0f<<(4*(pin-8)))) | (fun<<(4 * (pin-8)))), 
                   GPF_BASE+OFFSET_FUN1); 
    else if(pin<24) 
        tcc_writel(((tcc_readl(GPF_BASE+OFFSET_FUN2) & ~(0x0f<<(4*(pin-16)))) | (fun<<(4 * (pin-16)))), 
                   GPF_BASE+OFFSET_FUN2); 
    else if(pin<32) 
        tcc_writel(((tcc_readl(GPF_BASE+OFFSET_FUN3) & ~(0x0f<<(4*(pin-24)))) | (fun<<(4 * (pin-24)))), 
                   GPF_BASE+OFFSET_FUN3); 
    return 0; 
} 
static int tcc_set_cs_fun(void) 
{ 
    tcc_writel(((tcc_readl(GPC_BASE+OFFSET_FUN3) & ~(0x0f<<(4*(PIN_CS-24)))) ), 
               GPC_BASE+OFFSET_FUN3); 
    return 0; 
}

// set gpio direction, output: 1 for output, 0 for input
static int tcc_set_gpio_direction(int pin, int output) 
{ 
    tcc_writel(((tcc_readl(GPF_BASE+OFFSET_EN) & ~(1<<pin)) | (output<< pin)), 
               GPF_BASE+OFFSET_EN); 
    return 0; 
} 
static int tcc_set_cs_output(void) 
{ 
    tcc_writel(((tcc_readl(GPC_BASE+OFFSET_EN) | (1<< PIN_CS)) ), 
               GPC_BASE+OFFSET_EN); 
    return 0; 
}
// set gpio pin level, high: 1, low: 0
static int tcc_set_gpio_data(int pin, int level) 
{ 
    tcc_writel(((tcc_readl(GPF_BASE+OFFSET_DAT) & ~(1<<pin) )| (level<< pin)), 
               GPF_BASE+OFFSET_DAT); 
    return 0; 
} 
static int tcc_set_cs_data(int level) 
{ 
    tcc_writel(((tcc_readl(GPC_BASE+OFFSET_DAT) & ~(1<<PIN_CS) )| (level<< PIN_CS)), 
               GPC_BASE+OFFSET_DAT); 
    return 0; 
}
// get gpio pin level, high: 1, low: 0
static int tcc_get_gpio_data(int pin) 
{ 
    return ((tcc_readl(GPF_BASE+OFFSET_DAT) >>pin) & 1); 
}
void SPI_init(void) 
{     
    tcc_set_pin_fun(PIN_SDO, FUN_GPIO); //configure pin sdo and sda as GPIO
    tcc_set_pin_fun(PIN_SDI, FUN_GPIO); 
    tcc_set_pin_fun(PIN_SCLK, FUN_GPIO); 
         
    tcc_set_gpio_direction(PIN_SDO,1); 
    tcc_set_gpio_direction(PIN_SDI,1); 
    tcc_set_gpio_direction(PIN_SCLK,1);
    tcc_set_gpio_data(PIN_SDO,1); 
    tcc_set_gpio_data(PIN_SDI,1); 
    tcc_set_gpio_data(PIN_SCLK,1); 
         
    tcc_set_cs_fun(); 
    tcc_set_cs_output(); 
    tcc_set_cs_data(1); 
         
} 
void SPI_send(bool is_parameter,unsigned char w_data) 
{     
    unsigned char vsignbit; 
     
    //send DNC-bit
    tcc_set_gpio_data(PIN_SCLK,0); 
    tcc_set_gpio_data(PIN_SDO,is_parameter); 
    ndelay(20); 
    tcc_set_gpio_data(PIN_SCLK,1); 
    ndelay(20); 
     
    for(vsignbit=0x80;vsignbit>0;vsignbit>>=1) 
    {     
        tcc_set_gpio_data(PIN_SCLK,0); 
        if(w_data&vsignbit) 
            tcc_set_gpio_data(PIN_SDO,1); 
        else 
            tcc_set_gpio_data(PIN_SDO,0); 
        ndelay(20); 
        tcc_set_gpio_data(PIN_SCLK,1); 
        ndelay(20);         
    } 
    tcc_set_gpio_data(PIN_SDO,1); 
}     
unsigned char SPI_read(void) 
{     
    unsigned char vsignbit,r_data=0;
    tcc_set_gpio_direction(PIN_SDI,1);     
    tcc_set_gpio_data(PIN_SDI,1); 
    tcc_set_gpio_direction(PIN_SDI,0); 
    for(vsignbit=0x80;vsignbit>0;vsignbit>>=1) 
    { 
        tcc_set_gpio_data(PIN_SCLK,0); 
        ndelay(20); 
        if(tcc_get_gpio_data(PIN_SDI)) 
        {     
            r_data = r_data|vsignbit; 
        } 
        tcc_set_gpio_data(PIN_SCLK,1); 
        ndelay(20);         
    } 
    return r_data;     
}

static void set_value() 
{ 
    SPI_send(SPI_CMD, 0xB9); 
    SPI_send(SPI_DATA, 0xFF); 
    SPI_send(SPI_DATA, 0x83); 
    SPI_send(SPI_DATA, 0x63);
    SPI_send(SPI_CMD, 0xB1); 
    SPI_send(SPI_DATA, 0x81); 
    SPI_send(SPI_DATA, 0x30); 
    SPI_send(SPI_DATA, 0x07); 
    SPI_send(SPI_DATA, 0x34); 
    SPI_send(SPI_DATA, 0x02); 
    SPI_send(SPI_DATA, 0x13); 
    SPI_send(SPI_DATA, 0x11); 
    SPI_send(SPI_DATA, 0x00); 
    SPI_send(SPI_DATA, 0x3A); 
    SPI_send(SPI_DATA, 0x42); 
    SPI_send(SPI_DATA, 0x3F); 
    SPI_send(SPI_DATA, 0x3F);
    SPI_send(SPI_CMD, 0x11);
    mdelay(150); 
    SPI_send(SPI_CMD, 0x36); 
    SPI_send(SPI_DATA, 0x08);
    SPI_send(SPI_CMD, 0x3A); 
    SPI_send(SPI_DATA, 0x77);
    SPI_send(SPI_CMD, 0xB3); 
    SPI_send(SPI_DATA,0x09);
    SPI_send(SPI_CMD, 0xB4); 
    SPI_send(SPI_DATA, 0x08); 
    SPI_send(SPI_DATA, 0x12); 
    SPI_send(SPI_DATA, 0x72); 
    SPI_send(SPI_DATA, 0x12); 
    SPI_send(SPI_DATA, 0x06); 
    SPI_send(SPI_DATA, 0x03); 
    SPI_send(SPI_DATA, 0x54); 
    SPI_send(SPI_DATA, 0x03); 
    SPI_send(SPI_DATA, 0x4E); 
    SPI_send(SPI_DATA, 0x00); 
    SPI_send(SPI_DATA, 0x00);
    SPI_send(SPI_CMD, 0xBF); 
    SPI_send(SPI_DATA, 0x00); 
    SPI_send(SPI_DATA, 0x01);
    SPI_send(SPI_CMD, 0xB6); 
    SPI_send(SPI_DATA, 0x00);
    SPI_send(SPI_CMD, 0xCC); 
    SPI_send(SPI_DATA, 0x0A);
    mdelay(10);
    SPI_send(SPI_CMD, 0xE0); 
    SPI_send(SPI_DATA, 0x00); 
    SPI_send(SPI_DATA, 0x40); 
    SPI_send(SPI_DATA, 0x42); 
    SPI_send(SPI_DATA, 0xC1); 
    SPI_send(SPI_DATA, 0x4B); 
    SPI_send(SPI_DATA, 0xA7); 
    SPI_send(SPI_DATA, 0x06); 
    SPI_send(SPI_DATA, 0x0D); 
    SPI_send(SPI_DATA, 0x51); 
    SPI_send(SPI_DATA, 0x56); 
    SPI_send(SPI_DATA, 0x18); 
    SPI_send(SPI_DATA, 0x56); 
    SPI_send(SPI_DATA, 0x17); 
    SPI_send(SPI_DATA, 0x89); 
    SPI_send(SPI_DATA, 0x11); 
    SPI_send(SPI_DATA, 0x00); 
    SPI_send(SPI_DATA, 0x40); 
    SPI_send(SPI_DATA, 0x42); 
    SPI_send(SPI_DATA, 0xC1); 
    SPI_send(SPI_DATA, 0x4B); 
    SPI_send(SPI_DATA, 0xA7); 
    SPI_send(SPI_DATA, 0x06); 
    SPI_send(SPI_DATA, 0x0D); 
    SPI_send(SPI_DATA, 0x51); 
    SPI_send(SPI_DATA, 0x56); 
    SPI_send(SPI_DATA, 0x18); 
    SPI_send(SPI_DATA, 0x56); 
    SPI_send(SPI_DATA, 0x17); 
    SPI_send(SPI_DATA, 0x89); 
    SPI_send(SPI_DATA, 0x11);
    mdelay(5); 
    SPI_send(SPI_CMD, 0x29); 
}
static int __init spi_lcd_init(void) 
{
    PLOG("Register lcd spi control.\n"); 
    SPI_init();
    tcc_set_cs_data(0); 
    ndelay(20);     
    set_value(); 
    tcc_set_cs_data(1); 
    return 0; 
}    
static void __exit spi_lcd_exit(void) 
{ 
     
    printk(KERN_INFO "unregister lcd spi control.\n"); 
     
}

module_init(spi_lcd_init); 
module_exit(spi_lcd_exit);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("J.H.Luo<>"); 
MODULE_VERSION("0.1"); 
MODULE_DESCRIPTION("lcd spi control driver");

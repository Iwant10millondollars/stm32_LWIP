/* 引用头文件 *****************************************************************/
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#include "enc28j60.h"   

#include "bsp_spi1.h"
#include "bsp_usart.h"
#include "timer.h"

#include "lwip/init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"
#include "lwip/tcp_impl.h"

#include "ethernetif.h"
#include "echo.h"

/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
#define     DEBUG   0
#define     TCP_PERIOID     CLOCK_SECOND / 4        // TCP 250ms
#define     ARP_PERIOID     CLOCK_SECOND * 5        // ARP 5s
 
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
struct netif enc28j60;
/* 私有函数声明 ***************************************************************/
void RCC_Config(void);
void GPIO_Config(void);
void LwIP_Config(void);

/* 私有函数  ******************************************************************/
int main(void) 
{
    timer_typedef tcp_timer, arp_timer;
    
    /* 设定查询定时器 ARP定时器 */
    timer_set(&tcp_timer, CLOCK_SECOND / 10);       // tcp处理定时器 100ms
    timer_set(&arp_timer, CLOCK_SECOND * 5);        // arp处理定时器 5S
    
    RCC_Config();
    // NVIC_Config();
    GPIO_Config();          

    timer_config();
    BSP_ConfigSPI1();
    
    BSP_ConfigUSART1();
    printf("\r\nLwIP start!\r\n");
    printf("ipaddr:192.168.1.16\r\n");
    
    printf("ID:xukai871105\r\n");
    printf("Email:xukai19871105@126.com\r\n");
    
    LwIP_Config();
    // http_init();
    echo_init();
    while (1) { 
        
        if (enc28j60_packet_getcount() != 0) {
            ethernetif_input(&enc28j60);
        }
        
        // TCP 定时处理
        if (timer_expired(&tcp_timer)) {
            timer_set(&tcp_timer, CLOCK_SECOND / 4);
#if DEBUG
            GPIOD->ODR ^= GPIO_Pin_3;
#endif
            tcp_tmr();
        }
        
        // ARP 定时处理
        if (timer_expired(&arp_timer)) {
            timer_set(&arp_timer, CLOCK_SECOND * 5);
#if DEBUG
            GPIOD->ODR ^= GPIO_Pin_6;
#endif
            etharp_tmr();
        }
    }
}

void LwIP_Config (void) 
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    
    // 调用LWIP初始化函数
    lwip_init();    
    
    IP4_ADDR(&ipaddr, 192, 168, 1, 16);             // 设置网络接口的ip地址
    IP4_ADDR(&netmask, 255, 255, 255, 0);           // 子网掩码
    IP4_ADDR(&gw, 192, 168, 1, 1);                  // 网关
    
    // 初始化enc28j60与LWIP的接口，参数为网络接口结构体、ip地址、
    // 子网掩码、网关、网卡信息指针、初始化函数、输入函数
    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
    
    // 把enc28j60设置为默认网卡
    netif_set_default(&enc28j60);
    
    netif_set_up(&enc28j60);
}

void RCC_Config(void)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
                            RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
                            RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | 
                            RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // EXTI_InitTypeDef EXTI_InitStructure;

    // LED1@PB5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   
    
    // LED2@PD6 LED3@PD3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3;       
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    // SST25VF016B SPI片选
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                    
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // VS1003_CS@PB12 XPT2046_CS@PB7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_7;      
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // 禁止SPI1总线上的其他设备
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_SetBits(GPIOC, GPIO_Pin_4);
    
    // ENC28J60接收完成中断引脚 PA1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //内部上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

/*
********************************************************************************
*                                   
* 模块名称 : SPI1驱动模块    
* 文件名称 : bsp_spi1.h
* 版    本 : V0.1
* 说    明 : 配置SPI1相关参数
* 修改记录 :
*   版本号  日期        作者      说明
*   V0.1    2012-11-26  徐凯      创建该文件
*
*
********************************************************************************
*/
#ifndef __BSP_SPI1_H
#define __BSP_SPI1_H

/* 引用头文件 *****************************************************************/
#include "stm32f10x.h"
/* 外部数据类型 ***************************************************************/
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 外部函数声明 ***************************************************************/
void BSP_ConfigSPI1(void);
uint8_t BSP_SPI1SendByte(uint8_t byte);
uint8_t BSP_SPI1ReceiveByte(void);

#endif
/***************************************************************END OF FILE****/

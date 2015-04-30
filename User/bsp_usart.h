/*
********************************************************************************
*                                   
* 模块名称 : USART驱动模块    
* 文件名称 : bsp_usart.h
* 版    本 : V0.2
* 说    明 : 头文件
* 修改记录 :
*   版本号  日期        作者      说明
*   V0.1    2012-10-07  徐凯      创建该文件
*   V0.2    2012-10-13  徐凯      加入USART2部分
*
*
********************************************************************************
*/
#ifndef __BSP_USART_H
#define __BSP_USART_H
/* 引用头文件 *****************************************************************/
#include "stm32f10x.h"
/* 外部数据类型 ***************************************************************/
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 外部函数声明 ***************************************************************/
void BSP_ConfigUSART1(void);
void BSP_ConfigUSART2(void);
#endif
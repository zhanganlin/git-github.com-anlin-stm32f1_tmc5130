/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

#ifndef  __BSP_H__
#define  __BSP_H__

#include  <stm32f10x.h>


#include "bsp_int.h"

void  BSP_Init (void);
void bsp_DelayUS(uint32_t _ulDelayTime);
void BSP_Tick_Init (void);
static void NVIC_Configuration(void);

#endif

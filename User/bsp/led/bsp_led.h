#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"


//OUTPUT	
#define H_EN         GPIO_Pin_6        //PC6，水平电机使能
#define V_EN         GPIO_Pin_13        //PA13，水平电机使能



//INPUT
#define KEY_V_ZERO    GPIO_Pin_13   //PC13，按键垂直0位
#define KEY_LD        GPIO_Pin_14  //PC14，按键手动正转
#define KEY_RU        GPIO_Pin_15  //PC15，按键手动反转

#define H_ZERO        GPIO_Pin_3   //PB3，水平0位触点
#define U_LMT         GPIO_Pin_4   //PB4，上极限触点
#define D_LMT         GPIO_Pin_9   //PB9，下极限触点
#define D_LMT_PRE     GPIO_Pin_8   //PB8，下极限接近

#define H_INT        GPIO_Pin_5        //PC5，水平电机使能
#define V_INT        GPIO_Pin_14        //PA14，水平电机使能



void LED_GPIO_Config(void);
void GPIO_IO_Config(void);

#endif /* __LED_H */

  /*
*********************************************************************************************************
*	                                  
*	模块名称 : mmma7361加速度传感器模块  这个文件没有对应的  
*
*	版    本 : V2.0
*	说    明 : 管脚接口见管脚定义 ,这里没有使用0g-detect
*			x,y,z,3轴的接口与adcdouble.c中的接口相对应，模块移植时序注意
*********************************************************************************************************/
#ifndef __MMA7361_H
#define	__MMA7361_H


#include "stm32f10x.h"
#include "adcdouble.h"
#include"delay.h"

#define   SleepGPIO   GPIOC	 //睡眠模式管脚
#define   SleepPin    GPIO_Pin_5
#define   SelftestGPIO   GPIOC//自我检测管脚
#define  SelftestPin    GPIO_Pin_4
#define   gSelectGPIO   GPIOC //灵敏度选择管脚
#define   gSelectPin    GPIO_Pin_0
#define   PortCLK   RCC_APB2Periph_GPIOC
void  MMA7361Init(void); //这里最好只写一个初始化程序，因为读X,Y,Z的值可以在主程序中

#endif /* __ADC_H */



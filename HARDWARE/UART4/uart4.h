/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __UART4_H
#define __UART4_H

#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件

#define UART4_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define UART4_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#if  UART4_RX_ENABLE                          //如果使能接收功能
#define UART4_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern char Uart4_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Uart4_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Uart4_RxBuff[UART4_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif

void Uart4_Init(unsigned int);       
void u4_printf(char*,...) ;          
void u4_TxData(unsigned char *data);

#endif



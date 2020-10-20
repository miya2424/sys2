/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              操作Wifi功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __WIFI_H
#define __WIFI_H

#include "uart4.h"	    //包含需要的头文件

#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)x)  //PA4控制WiFi的复位

#define WiFi_printf       u4_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Uart4_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Uart4_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  UART4_RXBUFF_SIZE  //串口2控制 WiFi

#define SSID   "huawei5i"                     //路由器SSID名称
#define PASS   "12345678"                 //路由器密码

void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_Connect_IoTServer(void);


#endif

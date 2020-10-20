#ifndef __MAX31865_H
#define __MAX31865_H
#include "stm32f10x.h"


/* MAX31865�ο����� */
#define RREF  430  //400��


/* MAX31865���ƿ� */
#define MAX31865_CONTROL_PORT  GPIOA
#define MAX31865_SDO     GPIO_Pin_6
#define MAX31865_CS      GPIO_Pin_11
#define MAX31865_SCLK    GPIO_Pin_8
#define MAX31865_SDI     GPIO_Pin_7
#define MAX31865_DRDY 	 GPIO_Pin_5  

#define MAX31865_CS_SET      GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_CS,Bit_SET)
#define MAX31865_CS_CLR      GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_CS,Bit_RESET)
#define MAX31865_SCLK_SET    GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_SCLK,Bit_SET)
#define MAX31865_SCLK_CLR    GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_SCLK,Bit_RESET)
#define MAX31865_SDI_SET     GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_SDI,Bit_SET)
#define MAX31865_SDI_CLR     GPIO_WriteBit(MAX31865_CONTROL_PORT,MAX31865_SDI,Bit_RESET)

#define MAX31865_SDO_READ    GPIO_ReadInputDataBit(MAX31865_CONTROL_PORT,MAX31865_SDO)
#define MAX31865_DRDY_READ   GPIO_ReadInputDataBit(MAX31865_CONTROL_PORT,MAX31865_DRDY)

void MAX31865_Init(void);
void MAX31865_Cfg(void);
float MAX31865_GetTemp(void);

#endif

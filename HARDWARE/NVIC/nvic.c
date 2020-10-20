#include "stm32f10x_it.h"
#include "usart.h"
#include "string.h"
#include "nvic.h"

//////////////
/*更据ACC、GYR等不同种数据对应接收，便于后期加工使用*/
///////////
void USART2_IRQHandler(void)
{
	static uint8_t rebuf[13]={0},i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rebuf[i++]=USART_ReceiveData(USART2);
		if(rebuf[0]!=0x5a)//判断帧头
			i=0;
	  if((i==2)&&(rebuf[1]!=0x5a))//判断帧头
			i=0;
		if(i>4)//当i计数值=5时，功能字节接受完毕，数据长度字节接收完毕
		{
			if(i==rebuf[3]+5)
			{
	       memcpy(RX_BUF,rebuf,i);
				stata=1;
				i=0;
			}
		}
		USART_ClearFlag(USART2,USART_FLAG_RXNE);//清中断标志
	}	
}

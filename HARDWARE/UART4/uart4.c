/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的源文件               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "uart4.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件

#if  UART4_RX_ENABLE                   //如果使能接收功能
char Uart4_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Uart4_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Uart4_RxBuff[UART4_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

/*-------------------------------------------------*/
/*函数名：初始化串口2发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Uart4_Init(unsigned int bound)
{  	 	
	GPIO_InitTypeDef  GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
//时钟使能	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
//GPIO10使能TX	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;						//10脚
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC,&GPIO_InitStrue);					 //根据设定参数初始化GPIOA_9
//GPIO11使能RX
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;//复用推挽输出
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;						//11脚
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC,&GPIO_InitStrue);					 //根据设定参数初始化GPIOA_10
//USART初始化
	USART_InitStrue.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStrue.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStrue.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStrue.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
// UART4_RX_ENABLE               												   //如果使能接收模式
	USART_InitStrue.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式                                                                              //如果不使能接收模式
	  //USART_InitStrue.USART_Mode = USART_Mode_Tx ;	                           //只发模式
//#endif        
    USART_Init(UART4, &USART_InitStrue);                                      //设置串口2	

#if UART4_RX_ENABLE  	         					        //如果使能接收模式
	USART_ClearFlag(UART4, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);          //开启接收中断
   NVIC_InitStrue.NVIC_IRQChannel = UART4_IRQn;       //设置串口2中断
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级0
	NVIC_InitStrue.NVIC_IRQChannelSubPriority =3;		//子优先级0
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStrue);	                        //设置串口2中断
#endif  

	USART_Cmd(UART4, ENABLE);                              //使能串口2
	
}


//void UART4_IRQHandler(void)
//{
//	u8 UART4_in;
//	if(USART_GetITStatus(UART4,USART_IT_RXNE))
//	{
//			UART4_in=USART_ReceiveData(UART4);
//			USART_SendData(UART4, UART4_in);//向串口3发送数据
//				while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//	}
//}

/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char UART4_TxBuff[UART4_TXBUFF_SIZE];  

void u4_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(UART4_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)UART4_TxBuff);		
	while((UART4->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		UART4->DR = UART4_TxBuff[i];
		while((UART4->SR&0X40)==0);	
	}	
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u4_TxData(unsigned char *data)
{
	int	i;	
	while((UART4->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		UART4->DR = data[i+1];
		while((UART4->SR&0X40)==0);	
	}
}

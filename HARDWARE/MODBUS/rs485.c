#include "rs485.h"
#include "delay.h"
#include "crc16.h"
#include "led.h"
#include "usart.h"     //包含需要的头文件

/*******************************************************************************
* 函 数 名         : RS485_Init
* 函数功能		   : UART5初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/  

u8 UART5_RX_BUF[64];                   //接收缓冲，最大64字节
u8 UART5_RX_CNT=0;                       //接收字节计数器
u8 flagFrame=0;                         //帧接收完成标志，即接收到一帧新数据




void RS485_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE); //使能GPIOA\G时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能USART2时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;	//TX-485	//串口输出PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);		/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	//RX-485	   //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串5
	
	USART_Cmd(UART5, ENABLE);  //使能串口 5
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	RS485_TX_EN=0;				//默认为接收模式	
}

	

//  1ms定时

void TIM5_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=1000;   //自动装载值 
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM5,ENABLE); //使能定时器	
}


//计算发送的数据长度，并且将数据放到*buf数组中                     
u8 UartRead(u8 *buf, u8 len)  
{
	 u8 i;
	if(len>UART5_RX_CNT)  //指定读取长度大于实际接收到的数据长度时
	{
		len=UART5_RX_CNT; //读取长度设置为实际接收到的数据长度
	}
	for(i=0;i<len;i++)  //拷贝接收到的数据到接收指针中
	{
		*buf=UART5_RX_BUF[i];  //将数据复制到buf中
		buf++;
	}
	UART5_RX_CNT=0;              //接收计数器清零
	return len;                   //返回实际读取长度
}


u8 rs485_UartWrite(u8 *buf ,u8 len) 										//发送
{
	u8 i=0; 
   GPIO_SetBits(GPIOD,GPIO_Pin_7);                                            //发送模式
   delay_ms(3);                                                               //3MS延时
    for(i=0;i<=len;i++)
    {
	USART_SendData(UART5,buf[i]);	                                      //通过USARTx外设发送单个数据
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)==RESET);             //检查指定的USART标志位设置与否，发送数据空位标志
    }
    GPIO_ResetBits(GPIOD,GPIO_Pin_7);                                         //设置为接收模式

}




//保持寄存器赋值
extern int altitude;
extern u8 iaq;
extern u16 temperature1,oiltemper1,humidity1,speed1,ic1;
extern u32 gas1,pressure1,longtitude1,latitude1;
unsigned int regGroup[40];  //Modbus寄存器组，地址为0x00~0x04

//unsigned int regGroup[5]={456,257,789,65500};  //Modbus寄存器组，地址为0x00~0x04


	unsigned char i=0,cnt;
	unsigned int crc;
	unsigned char crch,crcl;
	static u8 len;
	static u8 buf[32];

//  float regGroup[20];  //Modbus寄存器组，地址为0x00~0x04
//串口驱动函数，检测数据帧的接收，调度功能函数，需在主循环中调用
void UartDriver()
{
	//保持寄存器赋值
	
	regGroup[0] = oiltemper1;
	regGroup[1] = temperature1;
	regGroup[2] = iaq;
	regGroup[3] = gas1/65536;
	regGroup[4] = gas1;
	regGroup[5] = pressure1/65536;
	regGroup[6] = pressure1;	
	regGroup[7] = altitude;
	regGroup[8] = humidity1;
	regGroup[9] = longtitude1/65536;
	regGroup[10] = longtitude1;
	regGroup[11] = latitude1/65536;
	regGroup[12] = latitude1;
	regGroup[13] = speed1;
	regGroup[14] = ic1;
	
	
	regGroup[15] = t1*10000;
	regGroup[16] = t2*10000;
	regGroup[17] = t3*100;
	regGroup[18] = t4*100;
	regGroup[19] = t5*100;
	regGroup[20] = t6*100;
	regGroup[21] = t7*100;	
	regGroup[22] = t8*100;
	regGroup[23] = t9*100;
	regGroup[24] = t10*100;
	regGroup[25] = t11*100;
	regGroup[26] = f1*100;
	regGroup[27] = f2*100;
	regGroup[28] = f3*100;
	regGroup[29] = f4*100;	
	
	

	if(flagFrame)            //帧接收完成标志，即接收到一帧新数据
	{
		flagFrame=0;           //帧接收完成标志清零
		len = UartRead(buf,sizeof(buf));   //将接收到的命令读到缓冲区中
		if(buf[0]==0x01)                   //判断地址是不是0x01
		{
			crc=GetCRC16(buf,len-2);       //计算CRC校验值，出去CRC校验值
			crch=crc>>8;    				//crc高位
			crcl=crc&0xFF;					//crc低位
			if((buf[len-2]==crch)&&(buf[len-1]==crcl))  //判断CRC校验是否正确
			{
				switch (buf[1])  //按功能码执行操作
				{
					case 0x03:     //读数据
						if((buf[2]==0x00)&&(buf[3]<=0x05))  //寄存器地址支持0x0000~0x0005
						{
							
							if(buf[3]<=0x04) 
							{
								i=buf[3];//提取寄存器地址
								cnt=buf[5];  //提取待读取的寄存器数量
								buf[2]=cnt*2;  //读取数据的字节数，为寄存器*2，因modbus定义的寄存器为16位
								len=3;							
								while(cnt--)
								{
									buf[len++] = regGroup[i]/256;
//									buf[len++]=0x00;				//寄存器高字节补0
									buf[len++]=regGroup[i++];		//低字节
							}
							
						}
							break;
					}
						else  //寄存器地址不被支持时，返回错误码
						{   
							buf[1]=0x83;  //功能码最高位置1
							buf[2]=0x02;  //设置异常码为02-无效地址
							len=3;
							break;
						}
					case 0x06:           //写入单个寄存器
						if((buf[2]==0x00)&&(buf[3]<=0x05))   //寄存器地址支持0x0000-0x0005
						{
							if(buf[3]<=0x04)
							{
								i=buf[3];				//提取寄存器地址
								regGroup[i]=buf[5];		//保存寄存器数据
//								led3=0;
							}
							len -=2;                 //长度-2以重新计算CRC并返回原帧
							break;
						}
						else  
						{							//寄存器地址不被支持，返回错误码
							buf[1]=0x86;           //功能码最高位置1
							buf[2]=0x02;           //设置异常码为02-无效地址
							len=3;
							break;
					}
					default:    //其他不支持的功能码
						    buf[1]=0x80;     //功能码最高位置1
							buf[2]=0x01;     //设置异常码为01—无效功能
							len=3;
							break;
				}
			    crc=GetCRC16(buf,len);           //计算CRC校验值
				buf[len++]=crc>>8;           //CRC高字节
				buf[len++]=crc&0xff;        //CRC低字节
				rs485_UartWrite(buf,len);  //发送响应帧
			}
		}
	}
}


				
void UartRxMonitor(u8 ms) //串口接收监控
{
	static u8 UART5_RX_BKP=0;  //定义USART2_RC_BKP暂时存储诗句长度与实际长度比较
	static u8 idletmr=0;        //定义监控时间
	if(UART5_RX_CNT>0)//接收计数器大于零时，监控总线空闲时间
	{
		if(UART5_RX_BKP!=UART5_RX_CNT) //接收计数器改变，即刚接收到数据时，清零空闲计时
		{
			UART5_RX_BKP=UART5_RX_CNT;  //赋值操作，将实际长度给USART2_RX_BKP
			idletmr=0;                    //将监控时间清零
		}
		else                              ////接收计数器未改变，即总线空闲时，累计空闲时间
		{
			//如果在一帧数据完成之前有超过3.5个字节时间的停顿，接收设备将刷新当前的消息并假定下一个字节是一个新的数据帧的开始
			if(idletmr<5)                  //空闲时间小于1ms时，持续累加
			{
				idletmr +=ms;
				if(idletmr>=5)             //空闲时间达到1ms时，即判定为1帧接收完毕
				{
					flagFrame=1;//设置命令到达标志，帧接收完毕标志
				}
			}
		}
	}
	else
	{
		UART5_RX_BKP=0;
	}
}
/*******************************************************************************
* 函 数 名         : UART5_IRQHandler
* 函数功能		   : UART5中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void UART5_IRQHandler(void)
{
	u8 res;	                                    //定义数据缓存变量
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		res =USART_ReceiveData(UART5);//;读取接收到的数据USART2->DR
		if(UART5_RX_CNT<sizeof(UART5_RX_BUF))    //一次只能接收64个字节，人为设定，可以更大，但浪费时间
		{
			UART5_RX_BUF[UART5_RX_CNT]=res;  //记录接收到的值
			UART5_RX_CNT++;        //使收数据增加1 
		}
	}
}
							
						
							
			
	
	
			



/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现IIc总线功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "iic.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化IIC接口                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;                      //定义一个设置IO的变量

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );    //使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;    //准备设置PB6 PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           //推免输出方式  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //IO速率50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);                     //设置PB6 PB7
	
	IIC_SCL_H;   //SCL拉高
	IIC_SDA_H;   //SDA拉高
}
/*-------------------------------------------------*/
/*函数名：IIC起始信号                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Start(void)
{
	SDA_OUT();     //SDA线输出模式
    IIC_SDA_H;	   //SDA拉高  
	IIC_SCL_H;     //SCL拉高	  
	delay_us(4);   //延时
 	IIC_SDA_L;     //SCL高电平的时候，SDA由高到低，起始信号
}
/*-------------------------------------------------*/
/*函数名：IIC停止信号                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IIC_Stop(void)
{
	SDA_OUT();     //SDA线输出模式
	IIC_SDA_L;     //SDA拉低
	IIC_SCL_H;     //SCL拉高
 	delay_us(4);   //延时
	IIC_SDA_H;     //SCL高电平的时候，SDA由低到高，发出停止信号					   	
}
/*-------------------------------------------------*/
/*函数名：等待应答                                 */
/*参  数：无                                       */
/*返回值：0：成功  1：失败                         */
/*-------------------------------------------------*/
char IIC_Wait_Ack(void)
{
	unsigned char timeout=0; //定义一个变量，用于计算超时时间
	
	IIC_SDA_H;	             //SDA拉高
	SDA_IN();                //SDA线输入模式 
	IIC_SCL_H;	             //SCL拉高
	 
	while(READ_SDA){         //等待SDA变低电平，表示应答到来，不然一直while循环，直到超时	
		timeout++;           //超时计数+1
		if(timeout>250){     //如果大于250	
			IIC_Stop();      //发送停止信号
			return 1;        //返回1，表示失败
		}
		delay_us(2);         //延时
	}
	IIC_SCL_L;               //SCL拉低
	delay_us(20);            //延时	
	return 0;                //返回0，表示成功
} 	
/*-------------------------------------------------*/
/*函数名：发送一个字节                             */
/*参  数：txd：发送的字节                          */
/*返回值：无                                       */
/*-------------------------------------------------*/ 			  
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;                   //定义一个变量用于for循环 
	
	SDA_OUT(); 	                       //SDA线输出模式 
    IIC_SCL_L;                         //SCL拉低，开始数据传输
    for(t=0;t<8;t++){                  //for循环，一位一位的发送，从最高位 位7开始                 
        IIC_SDA_OUT(((txd&0x80)>>7));  //给SDA数据线
        txd<<=1; 	                   //左移一位，准备下一次发送
		delay_us(2);                   //延时
		IIC_SCL_H;                     //SCL拉高
		delay_us(2);                   //延时
		IIC_SCL_L;	                   //SCL拉低
    }	 
} 	    
/*-------------------------------------------------*/
/*函数名：读取一个字节                             */
/*参  数：ack：是否发送应答 1:发 0：不发           */
/*返回值：读取的数据                               */
/*-------------------------------------------------*/   
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i;           //定义一个变量用于for循环 
	unsigned char receive;     //定义一个变量用于保存接收到的数据
		
	receive = 0;               //清除receive，准备接收数据
	SDA_IN();                  //SDA设置为输入
    IIC_SCL_L;                 //SCL拉低
    delay_us(20);               //延时	
	for(i=0;i<8;i++){          //for循环，一位一位的读取	 
		IIC_SCL_H;             //SCL拉高 
		delay_us(20);           //延时
        receive<<=1;           //左移一位，准备下次的读取
        if(READ_SDA)receive++; //读取数据
		delay_us(20);           //延时
		IIC_SCL_L;             //SCL拉低  
		delay_us(20);           //延时 		
    }					 
    if(!ack){        //不需要发送
		SDA_OUT();   //SDA线输出模式 
		IIC_SDA_H;   //时钟脉冲的高电平期间保持稳定的高电平
		IIC_SCL_H;   //SCL拉高
		delay_us(2); //延时
		IIC_SCL_L;   //SCL拉低 
	}else{           //需要发送
		SDA_OUT();   //SDA线输出模式 
		IIC_SDA_L;   //时钟脉冲的高电平期间保持稳定的低电平
		IIC_SCL_H;   //SCL拉高
		delay_us(2); //延时
		IIC_SCL_L;   //SCL拉低
		IIC_SDA_H;   //SDA拉高
	}
    return receive;
}

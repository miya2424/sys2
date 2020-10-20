/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器1功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：定时器1使能1分钟定时                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_60S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	
    TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=2-1;          //设置定时器重复计数2次（TIM1和8 才有这个功能）
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
                       	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
	
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1   
}

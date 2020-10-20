#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}



/*-------------------------------------------------*/
/*函数名：按键扫描函数                             */
/*参  数：无                                       */
/*返回值：有无按键按下，有的话是哪个按键按下       */
/*-------------------------------------------------*/
char KEY_Scan1(void)
{	 	  
	if(KEY1_IN_STA==1){                  //判断控制KEY1的IO输入电平，是否是高电平，如果是进入if	
	delay_ms(5);                     //延时5ms，消除抖动的误判
		if(KEY1_IN_STA==1){              //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是高电平，如果是进入if，说明按键按下了	
			while(1){                    //等待按键抬起
				if(KEY1_IN_STA==0){      //判断控制KEY1的IO输入电平，是否是低电平，如果是进入if	
					delay_ms(5);         //延时5ms，消除抖动的误判
					if(KEY1_IN_STA==0){  //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是低电平，如果是进入if，说明按键抬起了
						return 1;        //返回1，表示KEY1按下了
					}
				}
			}
		}
	}else if(KEY2_IN_STA==0){            //判断控制KEY2的IO输入电平，是否是低电平，如果是进入if	
		delay_ms(5);                     //延时5ms，消除抖动的误判
		if(KEY2_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是低电平，如果是进入if，说明按键按下了	
			while(1){                    //等待按键抬起
				if(KEY2_IN_STA==1){      //判断控制KEY2的IO输入电平，是否是高电平，如果是进入if
					delay_ms(5);         //延时5ms，消除抖动的误判
					if(KEY2_IN_STA==1){  //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是高电平，如果是进入if，说明按键抬起了
						return 2;        //返回2，表示KEY2按下了
					}
				}
			}
		}
	}	
	return 0;                            //如果没有按键按下，返回0
}



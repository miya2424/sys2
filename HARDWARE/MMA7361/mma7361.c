 #include"mma7361.h"
 
 void MMA7361_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(PortCLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin = SleepPin | SelftestPin| gSelectPin ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(SleepGPIO, &GPIO_InitStructure);				// PC1,输入时不用设置速率
}


 void  MMA7361Init(void)
 {
   MMA7361_GPIO_Config();
 GPIO_SetBits( SleepGPIO, SleepPin  );  //不处于睡眠模式 
    //GPIO_ResetBits( SleepGPIO, SleepPin  );  //处于睡眠模式 
   GPIO_SetBits( SelftestGPIO, SelftestPin );
//   GPIO_SetBits(  gSelectGPIO, gSelectPin);    //  1  6g 灵敏度 高,这个总是出问题	 
   GPIO_ResetBits(  gSelectGPIO, gSelectPin);	 // 0  1.5g 灵敏度	低	,这个调试成功了
//	delay_ms(1);//自我检测时间，不知是否正确
//    GPIO_ResetBits( SelftestGPIO, SelftestPin );这一句到底有木有起作用？

 }

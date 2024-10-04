#include "delay.h"
/*

循环功能实现us延时 适用任何单片机 ,比较暴力

*/

//void delay_us(uint32_t nus)
//{
//  __IO uint32_t Delay = nus * Your_JingZhen_Pinlu / 8;//(SystemCoreClock / 8U / 1000000U)
//    //见stm32f1xx_hal_rcc.c -- static void RCC_Delay(uint32_t mdelay)
//  do
//  {
//    __NOP();
//  }
//  while (Delay --);
//}


/*
Systick功能实现us延时 仅适用HAL库，如果使用其他库，请使用上面的

*/


#define Your_JingZhen_Pinlu 72
uint32_t fac_us = Your_JingZhen_Pinlu;
 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;
    ticks=nus*fac_us; 
    told=SysTick->VAL; 
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break; 
        }
    }
}

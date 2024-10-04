#include "time.h"
uint32_t time1 = 0;
void timer_init(void)
{
	HAL_TIM_Base_Start_IT(&htim2); 
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1); 
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
    
		if(htim->Instance == TIM2)
		{
			time1++; 
			
		}
}

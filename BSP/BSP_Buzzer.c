#include "BSP_Buzzer.h"

// 需要外部声明定时器句柄，与初始化时的句柄保持一致
extern TIM_HandleTypeDef htim4;
int16_t Buzzer_ToneFreq[37]=
{
	0,
	32107,30305,28604,26999,25483,24053,22703,21429,20226,19091,18019,17008,
	16053,15152,14302,13499,12742,12026,11352,10714,10113,9545,9010,8504,
	8027,7576,7151,6750,6371,6013,5676,5357,5056,4773,4505,4252,
};//蜂鸣器音调频率表

void Buzzer_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}



void Buzzer_Time(Buzzer_Tone Tone, uint16_t ms)
{
    if (Tone != P)
    {
        __HAL_TIM_SET_PRESCALER(&htim4, Buzzer_ToneFreq[Tone]);
        while (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) == RESET);
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 5);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    }

    HAL_Delay(ms);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

void Buzzer_ON(Buzzer_Tone Tone)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    if (Tone != P)
    {
        __HAL_TIM_SET_PRESCALER(&htim4, Buzzer_ToneFreq[Tone]);
        while (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) == RESET);
        __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 5);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    }
}

void Buzzer_OFF(void)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}



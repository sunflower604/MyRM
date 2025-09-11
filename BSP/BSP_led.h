/*
*文 件 名:  BSP_led.h
*作   者:   陈卓文
*版   本:   V1.0
*日   期:   025.09.10
*描   述:   RGB灯驱动
*备   注:   要使用cubemx配置好再使用；
            每个灯的相关函数均不影响其他灯的亮灭；
*预 配 置:   TIM5的CH1、CH2、CH3配置为PWM输出模式；
            TIM5的时钟频率为84MHz，预分频系数为0，自动重装载值为65535；
*/

#ifndef BSP_LED_H
#define BSP_LED_H
#include "struct_typedef.h"

/**
  * @brief          aRGB show
  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
  * @retval         none
  */
/**
  * @brief          显示RGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' 是透明度,'RR'是红色,'GG'是绿色,'BB'是蓝色
  * @retval         none
  */
void aRGB_led_show(uint32_t aRGB);
void RGB_Init(void);

void RGB_Red_ON(void);
void RGB_Red_OFF(void);
void RGB_Red_Turn(void);

void RGB_Green_ON(void);
void RGB_Green_OFF(void);
void RGB_Green_Turn(void);

void RGB_Blue_ON(void);
void RGB_Blue_OFF(void);
void RGB_Blue_Turn(void);


void RGB_Mix(uint8_t red, uint8_t green, uint8_t blue);

#endif

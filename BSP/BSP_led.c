#include "bsp_led.h"
#include "main.h"

extern TIM_HandleTypeDef htim5;
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
void aRGB_led_show(uint32_t aRGB)
{
    static uint8_t alpha;
    static uint16_t red,green,blue;

    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;

    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}

void RGB_Init(void)
{
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
	aRGB_led_show(0xFF000000);
}

/*
*函数简介:点亮红色LED（最大亮度）
*参数说明:无
*返回类型:无
*备注:无
*/
void RGB_Red_ON(void)
{
    // 获取当前蓝色和绿色通道的PWM值（保持不变）
    uint32_t Light_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t Light_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    
    // 红色最大亮度 = 255(红色值) * 255(最大透明度)
    uint16_t max_red = 255 * 255;
    
    // 只更新红色通道，保持蓝、绿通道当前值
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, max_red);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, Light_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, Light_green);
}

/*
*函数简介:熄灭红色LED（最大亮度）
*参数说明:无
*返回类型:无
*备注:无
*/
void RGB_Red_OFF(void)
{
    // 获取当前蓝色和绿色通道的PWM值（保持不变）
    uint32_t Light_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t Light_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    
    // 红色最小亮度 = 255(红色值) * 255(最大透明度)
    uint16_t min_red = 0 * 0;
    
    // 只更新红色通道，保持蓝、绿通道当前值
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, min_red);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, Light_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, Light_green);
}

/*
*函数简介:反转红色LED
*参数说明:无
*返回类型:无
*备注:无
*/
void RGB_Red_Turn(void)
{
    // 获取当前红色通道的PWM值
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    
    // 红色最大亮度 = 255(红色值) * 255(最大透明度)
    uint16_t max_red = 255 * 255;
    
    // 计算新的红色通道PWM值（反转）
    uint32_t new_red = max_red - current_red;
    
    // 更新红色通道，保持蓝、绿通道当前值
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, new_red);
}

/* 绿色LED控制函数 */
void RGB_Green_ON(void)
{
    uint32_t current_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t max_green = 255 * 255;  // 最大亮度
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, max_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, current_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

void RGB_Green_OFF(void)
{
    uint32_t current_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t min_green = 0;  // 最小亮度（熄灭）
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, min_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, current_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

void RGB_Green_Turn(void)
{
    uint32_t current_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    uint32_t current_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t max_green = 255 * 255;
    
    // 反转绿色状态
    uint32_t new_green = max_green - current_green;
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, new_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, current_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

/* 蓝色LED控制函数 */
void RGB_Blue_ON(void)
{
    uint32_t current_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t max_blue = 255 * 255;  // 最大亮度
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, max_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, current_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

void RGB_Blue_OFF(void)
{
    uint32_t current_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t min_blue = 0;  // 最小亮度（熄灭）
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, min_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, current_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

void RGB_Blue_Turn(void)
{
    uint32_t current_blue = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_1);
    uint32_t current_green = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_2);
    uint32_t current_red = __HAL_TIM_GetCompare(&htim5, TIM_CHANNEL_3);
    uint16_t max_blue = 255 * 255;
    
    // 反转蓝色状态
    uint32_t new_blue = max_blue - current_blue;
    
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, new_blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, current_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, current_red);
}

void RGB_Mix(uint8_t red, uint8_t green, uint8_t blue)
{
    uint16_t mix_red = red * 255;
    uint16_t mix_green = green * 255;
    uint16_t mix_blue = blue * 255;

    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, mix_red);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, mix_green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, mix_blue);
}

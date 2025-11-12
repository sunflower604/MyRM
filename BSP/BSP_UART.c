#include "BSP_UART.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/*
 *函数简介:UART2发送一个字节
 *参数说明:8bits数据
 *返回类型:无
 *备注:需确保UART2已初始化(huart2句柄有效)
 */
void UART2_SendByte(uint8_t Byte)
{
    // 使用HAL库发送一个字节，超时时间设置为10ms
    HAL_UART_Transmit(&huart1, &Byte, 1, 10);
}

/*
 *函数简介:UART2发送一个数组
 *参数说明:8bits数组指针
 *参数说明:数组长度
 *返回类型:无
 *备注:需确保UART2已初始化(huart2句柄有效)
 */
void UART2_SendArray(uint8_t *Array, uint16_t Length)
{
    // 检查输入参数有效性
    if (Array == NULL || Length == 0)
        return;
    
    // 使用HAL库发送指定长度的数组，超时时间设置为100ms
    HAL_UART_Transmit(&huart1, Array, Length, 100);
}



void UART1_TX_u32Num(uint32_t Num) 
{
    char buf[11];
    snprintf(buf, sizeof(buf), "%lu", (unsigned long)Num);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
}



void UART1_TX_u32NumArray(uint32_t *NumArray , uint32_t length) 
{
    uint8_t index , sign=',';
    for(index=0 ; index<length ; index++){
        UART1_TX_u32Num(NumArray[index]);
        if(index < length-1){
            HAL_UART_Transmit(&huart1, &sign, 1, 100);
        }
    }
    sign='\n';
    HAL_UART_Transmit(&huart1, &sign, 1, 100);

}

/*使用示例
    uint32_t Num[10] = {1,2,3,4,5,6,7,8,9,0};
    UART1_TX_u32NumArray(Num , sizeof(Num)/sizeof(Num[0]));
*/

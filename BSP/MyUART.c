#include "MyUART.h"
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



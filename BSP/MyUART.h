#ifndef MYUART_H
#define MYUART_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "usart.h"

extern uint8_t UART1_RxData;//接收数据缓存区
extern uint8_t UART1_RxFlag;//接收完成标志位
extern uint8_t UART2_RxData;//接收数据缓存区
extern uint8_t UART2_RxFlag;//接收完成标志位


void    UART2_SendByte(uint8_t Byte);//UART2发送一个字节
void    UART2_SendArray(uint8_t *Array,uint16_t Length);//UART2发送一个数组





#endif // MYUART_H





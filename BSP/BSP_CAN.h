#ifndef __CAN_FILTER_H
#define __CAN_FILTER_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "can.h"



HAL_StatusTypeDef   CAN_Filter_Init(void);
uint32_t            CAN1_Receive(CAN_RxHeaderTypeDef *RxHeader ,uint8_t *Data);
uint32_t            CAN2_Receive(CAN_RxHeaderTypeDef *RxHeader ,uint8_t *Data);
uint32_t            CAN1_Transmit(CAN_TxHeaderTypeDef *TxHeader , uint8_t *Data);
uint32_t            CAN2_Transmit(CAN_TxHeaderTypeDef *TxHeader , uint8_t *Data);



#endif /*__CAN_FILTER_H*/





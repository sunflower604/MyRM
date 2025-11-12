#include "BSP_CAN.h"
/*
CAN协议：0 xxxxxxxxxxxx 00xxxx n*xxxxxxxx xxxxxxxxxxxxxxx1 11 1111111
|--起始帧 																										0
|--仲裁域 
|----ID（标识符） 																						xxxx xxxx xxx
|----RTR（远程发送请求帧，0为数据帧，1为遥控帧） 								 x
|--控制域 
|----IDE（扩展格式标识符，标准格式为0，扩展格式为1）						 0
|----r0（保留位） 																						0
|----DLC（数据长度） 																					xxxx
|--数据域（由DLC决定，DLC为n，则改域长度为8n） 									n*xxxxxxxx
|--CRC域	
|----校验码 																									xxxx xxxx xxxx xxx
|----界定符 																									1
|--ACK域	
|----ACK槽（发送端发1，接收端给0） 															1
|----界定符 																									1
|--结束帧																											1111 111

*/



typedef enum {
    CToC_SlaveID1 = 0x149,
    CToC_SlaveID2 = 0x189
} CToC_SlaveID;
typedef enum {
    CAN_MODEL_M3508,        // M3508电机
    CAN_MODEL_GM6020,       // GM6020电机
    CAN_MODEL_ROBOMASTER_C  // C板设备
} CAN_DeviceModel;
typedef enum{
	GM6020_1=0x205,//ID1
	GM6020_2=0x206,//ID2
	GM6020_3=0x207,//ID3
	GM6020_4=0x208,//ID4
	GM6020_5=0x209,//ID5
	GM6020_6=0x20A,//ID6
	GM6020_7=0x20B,//ID7
}GM6020_ID;//GM6020电机ID号枚举
typedef enum{
	M3508_1=0x201,//ID1
	M3508_2=0x202,//ID2
	M3508_3=0x203,//ID3
	M3508_4=0x204,//ID4
	M3508_5=0x205,//ID5
	M3508_6=0x206,//ID6
	M3508_7=0x207,//ID7
	M3508_8=0x208,//ID8
}M3508_ID;//M3508电机ID号枚举

uint32_t CAN_CAN1IDList[10][2] =
{{CAN_MODEL_M3508,M3508_1} , {CAN_MODEL_M3508,M3508_2} , {CAN_MODEL_M3508,M3508_3} , {CAN_MODEL_M3508,M3508_4} , {0 , 0}};//CAN1总线上设备ID列表
uint32_t CAN_CAN2IDList[10][2] =
{{CAN_MODEL_GM6020,GM6020_1} , {CAN_MODEL_ROBOMASTER_C,CToC_SlaveID1} , {CAN_MODEL_ROBOMASTER_C,CToC_SlaveID2} , {0 , 0}};


HAL_StatusTypeDef CAN_Filter_Init(void)
{
  CAN_FilterTypeDef CAN_FilterStructure ;
	HAL_StatusTypeDef result;

  CAN_FilterStructure.FilterBank 						= 0;											//选择第几个过滤器
  CAN_FilterStructure.FilterIdHigh 					= (0x200<<5);
  CAN_FilterStructure.FilterMaskIdHigh 			= 0xFFF0;
  CAN_FilterStructure.FilterIdLow 					= 0x0000; 
  CAN_FilterStructure.FilterMaskIdLow 			= 0x0000;
  CAN_FilterStructure.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;				//过滤器关联，FIFO0或FIFO1，过滤完的报文放在哪里
  CAN_FilterStructure.FilterMode 						= CAN_FILTERMODE_IDMASK;	//过滤器的工作模式，IDMASK屏蔽模式 或 IDLIST列表模式
  CAN_FilterStructure.FilterScale 					= CAN_FILTERSCALE_16BIT;	//过滤器的位宽，16位或32位
  CAN_FilterStructure.FilterActivation 			= ENABLE;									//过滤器使能，ENABLE或DISABLE，过滤器的开关
  CAN_FilterStructure.SlaveStartFilterBank 	= 15;											//从can起始过滤器位置，如果有两个can，表示从n开始的过滤器都属于从can，一个can无效
  result = HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterStructure);
	if(	result != HAL_OK)		return result; //如果配置失败，直接返回错误

  CAN_FilterStructure.FilterBank 						= 15;											//选择第几个过滤器
  CAN_FilterStructure.FilterIdHigh 					= (0x000<<5);
  CAN_FilterStructure.FilterIdLow 					= (0x000<<5);
  CAN_FilterStructure.FilterMaskIdHigh 			= 0xFFE3;
  CAN_FilterStructure.FilterMaskIdLow 			= 0xFFE3;
  CAN_FilterStructure.FilterFIFOAssignment 	= CAN_FILTER_FIFO1;				//过滤器关联，FIFO0或FIFO1，过滤完的报文放在哪里
  CAN_FilterStructure.FilterMode 						= CAN_FILTERMODE_IDMASK;	//过滤器的工作模式，IDMASK屏蔽模式 或 IDLIST列表模式
  CAN_FilterStructure.FilterScale 					= CAN_FILTERSCALE_16BIT;	//过滤器的位宽，16位或32位
  CAN_FilterStructure.FilterActivation 			= ENABLE;									//过滤器使能，ENABLE或DISABLE，过滤器的开关
  CAN_FilterStructure.SlaveStartFilterBank 	= 15;											//从can起始过滤器位置，如果有两个can，表示从n开始的过滤器都属于从can，一个can无效
  result = HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterStructure);
	
	return result;
}




uint32_t CAN1_Receive(CAN_RxHeaderTypeDef *RxHeader ,uint8_t *Data)
{
	uint8_t RxData[8] ;

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, RxHeader,  RxData);

	if(RxHeader->RTR == CAN_RTR_DATA){	//数据帧
		for(uint8_t i = 0; i < RxHeader->DLC; i++){//获取数据
			Data[i] = RxData[i]; 		
		}
		return RxHeader->StdId;
	}else{								//遥控帧
		//....
	}
}

uint32_t CAN2_Receive(CAN_RxHeaderTypeDef *RxHeader ,uint8_t *Data)
{
	uint8_t RxData[8] ;

	HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, RxHeader,  RxData);

	if(RxHeader->RTR == CAN_RTR_DATA){	//数据帧
		for(uint8_t i = 0; i < RxHeader->DLC; i++){//获取数据
			Data[i] = RxData[i]; 		
		}
		return RxHeader->StdId;
	}else{								//遥控帧
		//....
	}
}


uint32_t CAN1_Transmit(CAN_TxHeaderTypeDef *TxHeader, uint8_t *Data)
{
	uint32_t TxMailbox;						//实际邮箱号
	uint32_t Timeout = 0;
	while( HAL_CAN_AddTxMessage(&hcan1, TxHeader, Data, &TxMailbox) != HAL_OK ){
		//如果发送失败
		Timeout++;
		if(Timeout > 10000){ //如果超时
			break; //跳出循环
		}
	}

}

uint32_t CAN2_Transmit(CAN_TxHeaderTypeDef *TxHeader, uint8_t *Data)
{
	uint32_t TxMailbox;						//实际邮箱号
	uint32_t Timeout = 0;
	while( HAL_CAN_AddTxMessage(&hcan2, TxHeader, Data, &TxMailbox) != HAL_OK ){
		//如果发送失败
		Timeout++;
		if(Timeout > 10000){ //如果超时
			break; //跳出循环
		}
	}

}



void CAN1_UpdateID(uint32_t ID, uint32_t mask)
{
    CAN_FilterTypeDef sFilterConfig = {0};
    
    sFilterConfig.FilterBank = 15;
    sFilterConfig.FilterIdHigh = (ID << 5) & 0xFFFF;
    sFilterConfig.FilterMaskIdHigh = (mask << 5) & 0xFFFF;
    
    HAL_CAN_Stop(&hcan2);
    
    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    
    HAL_CAN_Start(&hcan2);
}


void CAN2_UpdateID(uint32_t ID, uint32_t mask)
{
    CAN_FilterTypeDef sFilterConfig = {0};
    
    sFilterConfig.FilterBank = 15;
    sFilterConfig.FilterIdHigh = (ID << 5) & 0xFFFF;
    sFilterConfig.FilterMaskIdHigh = (mask << 5) & 0xFFFF;
    
    HAL_CAN_Stop(&hcan2);
    
    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    
    HAL_CAN_Start(&hcan2);
}
    
// void CAN_CAN1ChangeID(uint32_t ID)
// {
//     CAN1->FMR |= CAN_FMR_FINIT;
//     CAN1->sFilterRegister[0].FR1 = ((uint32_t)0xFFE3 << 16) | (ID << 5);
//     CAN1->sFilterRegister[0].FR2 = ((uint32_t)0xFFE3 << 16) | (ID << 5);
//     CAN1->FMR &= ~CAN_FMR_FINIT;
// }
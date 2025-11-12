#include "Hardware_Remote.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "tim.h"
#include "iwdg.h"
#include "stm32f4xx_hal_uart.h"

#define BUFFER_SIZE 18//遥控器数据长度

typedef struct
{
	uint16_t Remote_R_RL;					//通道0-右摇杆左右(右为大),范围364(最左端)~1684(最右端),默认值1024(中间)
	uint16_t Remote_R_UD;					//通道1-右摇杆上下(上为大),范围364(最下端)~1684(最上端),默认值1024(中间)
	uint16_t Remote_L_RL;					//通道2-左摇杆左右(右为大),范围364(最左端)~1684(最右端),默认值1024(中间)
	uint16_t Remote_L_UD;					//通道3-左摇杆上下(上为大),范围364(最下端)~1684(最上端),默认值1024(中间)
	
	uint8_t Remote_LS;						//S1-左侧拨动开关,范围1~3,上为1,下为2,中间为3
	uint8_t Remote_RS;						//S2-右侧拨动开关,范围1~3,上为1,下为2,中间为3
	
	int16_t Remote_Mouse_RL;			//鼠标X轴-鼠标左右速度,范围-32768~32767,向右为正,向左为负,静止值为0
	int16_t Remote_Mouse_DU;			//鼠标Y轴-鼠标前后速度,范围-32768~32767,向后为正,向前为负,静止值为0
	int16_t Remote_Mouse_Wheel;		//鼠标Z轴-鼠标滚轮速度,范围-32768~32767,向前为正,向后为负,静止值为0
	uint8_t Remote_Mouse_KeyL;		//鼠标左键,按下为1,未按下为0
	uint8_t Remote_Mouse_KeyR;		//鼠标右键,按下为1,未按下为0
	
	uint8_t Remote_Key_W;					//键盘W键,按下为1,未按下为0
	uint8_t Remote_Key_S;					//键盘S键,按下为1,未按下为0
	uint8_t Remote_Key_A;					//键盘A键,按下为1,未按下为0
	uint8_t Remote_Key_D;					//键盘D键,按下为1,未按下为0
	uint8_t Remote_Key_Q;					//键盘Q键,按下为1,未按下为0
	uint8_t Remote_Key_E;					//键盘E键,按下为1,未按下为0
	uint8_t Remote_Key_Shift;			//键盘Shift键,按下为1,未按下为0
	uint8_t Remote_Key_Ctrl;			//键盘Ctrl键,按下为1,未按下为0
	uint8_t Remote_KeyPush_Ctrl;	//按下键盘Ctrl键,按下时0,1切换
	uint8_t Remote_KeyPush_Shift;	//按下键盘Shift键,按下时0,1切换
	
	int16_t Remote_ThumbWheel;		//保留字段-遥控器拨轮,范围-3278(最上端)~1684(最下端),默认值1024
}Remote_Data;//遥控器接收结构体

uint8_t Remote_RxData0[BUFFER_SIZE];//遥控器DMA数据存储器0
uint8_t Remote_RxData1[BUFFER_SIZE];//遥控器DMA数据存储器1

Remote_Data Remote_RxData;//遥控器接收数据
Remote_Data Remote_LastRxData;//遥控器上一次接收数据
uint8_t Remote_Status;//遥控器连接状态,默认未连接(0)
uint8_t Remote_StartFlag=1;//遥控器启动标志位,0-未在启动阶段,1-准备启动,2-第一次接收到数据

extern DMA_HandleTypeDef hdma_usart3_rx;  // USART3接收DMA句柄
extern UART_HandleTypeDef huart3;         // USART3句柄
extern TIM_HandleTypeDef htim7;           // TIM7句柄
extern IWDG_HandleTypeDef hiwdg;          // 独立看门狗句柄

/**
  *功能：
  *参数：
  *备注：测试要点 dma转运 
  				 tim7定时中断（如果进入这个中断，说明usart3已经25ms没收到数据）
				 usart3空闲中断 
				 iwdg独立看门狗
				 
  */
//dma两个参数报错，需要重新规划配置
void Remote_Init(void)//遥控器初始化
{
  HAL_DMAEx_MultiBufferStart(&hdma_usart3_rx, 
                          (uint32_t)&USART3->DR,      // 外设地址（USART3数据寄存器）
                          (uint32_t)Remote_RxData0,   // 存储器0地址
                          (uint32_t)Remote_RxData1,   // 存储器1地址（双缓冲需要两个缓冲区）
                          BUFFER_SIZE);               // 传输数据量

  __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);  // 清除TIM7更新标志
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);    // 使能USART3接收寄存器非空中断
  __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);     // 使能TIM7更新中断


  // 使能DMA1的数据流1（对应USART3接收）
  HAL_DMA_Start(&hdma_usart3_rx, 
               (uint32_t)&huart3.Instance->DR,  // 外设地址(USART3数据寄存器)
               (uint32_t)Remote_RxData0,        // 存储器地址(根据实际缓冲区填写)
               BUFFER_SIZE);                    // 传输数据量


  HAL_USART_Receive_DMA(&huart3, rxBuffer, bufferSize);	// 使能串口USART3的DMA接收请求
  HAL_UART_Start(&huart3);                      		// 启动USART3
  HAL_TIM_Base_Start(&htim7);                   		// 启动定时器TIM7
  //HAL_IWDG_EnableWriteAccess(&hiwdg);           		// 使能独立看门狗写访问
  HAL_IWDG_Start(&hiwdg);                       		// 启动独立看门狗
}

void Remote_ON(void)//遥控器开启
{
  HAL_USART_Init(&huart3); // 初始化USART3
}

void Remote_OFF(void)//遥控器关闭
{
  HAL_USART_DeInit(&huart3); // 反初始化USART3
}










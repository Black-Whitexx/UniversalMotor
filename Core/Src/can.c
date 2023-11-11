/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
uint8_t clear_flag;
extern uint8_t global_flag;
extern uint8_t Can_Callback_Message[30];
extern int16_t Sensor_Speed, Sensor_Count, Sensor_Current, Sensor_Position, last_Sensor_Count, Sensor_Row, pos_t;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_5TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

CAN_TxHeaderTypeDef	TxHeader;
CAN_RxHeaderTypeDef	RxHeader;

void Motor1_Data_Process(void)
{
    float pos_f;
    Sensor_Current = (int16_t)((Can_Callback_Message[4] << 8) | Can_Callback_Message[5]);
    Sensor_Current = Sensor_Current>30000 ? 65535 - Sensor_Current : Sensor_Current;
    Sensor_Speed = (int16_t)((Can_Callback_Message[2] << 8) | Can_Callback_Message[3]);
    Sensor_Speed = Sensor_Speed>30000? 65535 - Sensor_Speed : Sensor_Speed;
    Sensor_Count = (int16_t)((Can_Callback_Message[0] << 8) | Can_Callback_Message[1]);
    pos_f = ((float)Sensor_Count)/8192.f;
    if(last_Sensor_Count - Sensor_Count > 4000){
        pos_t++;
    }
    if(last_Sensor_Count - Sensor_Count < -4000){
        pos_t--;
    }
    Sensor_Position = (int16_t)(((float)pos_t + pos_f)*100.f);
    last_Sensor_Count = Sensor_Count;
    //Sensor_Row = Sensor_Position / 1;
}

uint8_t Can_Transmit(CAN_HandleTypeDef *hcan, uint8_t* message,uint32_t ID)
{
    uint32_t TxMailbox;
    TxHeader.StdId=0x00000FFF & ID;
    TxHeader.IDE=CAN_ID_STD;
    TxHeader.RTR=CAN_RTR_DATA;
    TxHeader.DLC=8;

    if(HAL_CAN_AddTxMessage(hcan, &TxHeader, message, &TxMailbox) != HAL_OK){
        return 1;
    }
    else{
        return 0;
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rxbuffer[8];
    if(hcan->Instance==CAN1) {
        HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&RxHeader,rxbuffer);
        for(int i=0; i<8; i++){
            Can_Callback_Message[i]=rxbuffer[i];
        }
        Can_Callback_Message[8] = 1;
        Can_Callback_Message[9] = (uint8_t)(RxHeader.StdId >> 24) & 0xFF;
        Can_Callback_Message[10] = (uint8_t)(RxHeader.StdId >> 16) & 0xFF;
        Can_Callback_Message[11] = (uint8_t)(RxHeader.StdId >> 8) & 0xFF;
        Can_Callback_Message[12] = (uint8_t)(RxHeader.StdId >> 0) & 0xFF;
    }
    else if(hcan->Instance==CAN2) {
        HAL_CAN_GetRxMessage(hcan,CAN_FILTER_FIFO0,&RxHeader,rxbuffer);
        for(int i=0; i<8; i++){
            Can_Callback_Message[i+13]=rxbuffer[i];
        }
        Can_Callback_Message[21] = 2;
        Can_Callback_Message[22] = (uint8_t)(RxHeader.StdId >> 24) & 0xFF;
        Can_Callback_Message[23] = (uint8_t)(RxHeader.StdId >> 16) & 0xFF;
        Can_Callback_Message[24] = (uint8_t)(RxHeader.StdId >> 8) & 0xFF;
        Can_Callback_Message[25] = (uint8_t)(RxHeader.StdId >> 0) & 0xFF;
    }
    Motor1_Data_Process();
}



/* USER CODE END 1 */

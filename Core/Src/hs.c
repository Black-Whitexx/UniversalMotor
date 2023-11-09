//
// Created by YINING on 2023/7/27.
//
#include "hs.h"
#include "string.h"
const char tail[4] = {0x00, 0x00, 0x80, 0x7f};
void hs_UART_Transmit(UART_HandleTypeDef *huart, float a){
    char* buffer, *frame;
    buffer = (char *)(&a);
    HAL_UART_Transmit(huart, buffer, sizeof(float), 1000);
    HAL_UART_Transmit(huart, tail, 4, 1000);
}


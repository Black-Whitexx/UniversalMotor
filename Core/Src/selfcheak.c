//
// Created by YINING on 2023/11/8.
//曾子曰:“吾日三省吾身:为人谋而不忠乎?与朋友交而不信乎?传不习乎?” ——《论语》

#include "selfcheck.h"
extern IWDG_HandleTypeDef hiwdg;
//自检
uint8_t SelfCheck(void){
    HAL_Delay(1);
    HAL_IWDG_Refresh(&hiwdg);
    return 0;
}
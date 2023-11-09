//
// Created by YINING on 2023/11/9.
//

#ifndef R1_DEMO_MOTOR_H
#define R1_DEMO_MOTOR_H
#include "global.h"
uint8_t Motor1_Current(uint16_t current);
uint8_t Motor1_Speed(float speed);
void Motor1_Position(float position, float speed);
#endif //R1_DEMO_MOTOR_H

//
// Created by BxW on 2024/4/24.
//

#ifndef R1_DEMO_ODRIVE_H
#define R1_DEMO_ODRIVE_H
#include "can.h"
struct ODrive_Sturct{
    CAN_HandleTypeDef *odrive_can;
    PIDParDef *odrive_pid_para;
    uint8_t id;
};
void ODrive_Init(struct ODrive_Sturct *_odrive,CAN_HandleTypeDef *_can,PIDParDef *_pid,uint8_t number);
void ODrive_Setspeed();

#endif //R1_DEMO_ODRIVE_H

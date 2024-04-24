//
// Created by BxW on 2024/4/24.
//
#include "odrive.h"
void ODrive_Init(struct ODrive_Sturct *_odrive,CAN_HandleTypeDef *_can,PIDParDef *_pid,uint8_t _id){
    _odrive->odrive_can = _can;
    _odrive->odrive_pid_para = _pid;
    _odrive->id = _id;

}
void ODrive_Setspeed(struct ODrive_Sturct *_odrive,uint32_t _speed){
    Can_Transmit(_odrive->odrive_can,(uint8_t *)&_speed,_odrive->id<<5|0x00D);
}
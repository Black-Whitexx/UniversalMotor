//
// Created by YINING on 2023/11/9.
//
#include "motor.h"
extern int16_t Sensor_Speed, Sensor_Current, Sensor_Position, Sensor_Row, Sensor_Count;
uint16_t EmeStop, Stop_Count;
PIDParDef pid1={60.f,0.f,0.f,0,0,0,2000,-2000,0, INF};//电流环
PIDParDef pid2={10.f,0.f,1.f,0,0,0,2000,-2000,0,INF};//速度环
PIDParDef pid3={20.f,0.001f,0.f,0,0,0,3000,-3000,800,INF};//位置环
float tar_speed, tar_current, tar_position;
int16_t output;
uint8_t txbuffer[8];

/*
uint8_t EmeStop_Detection(void){
    if(Sensor_Current > 10000){
        EmeStop = 1;
        Stop_Count = Sensor_Count;
        return 1;
    }
    else{
        return 0;
    }
}
*/

uint8_t Motor1_Current(int16_t current){
    txbuffer[2] = (int8_t )(current>>8);
    txbuffer[3] = (int8_t)(current & 0xFF);
    if(!Can_Transmit(&hcan1, txbuffer, 0x200)) return 0;
    else return 1;
}

uint8_t Motor1_Speed(int16_t speed){
    output = PID_Process(&pid2,speed, Sensor_Speed);
    txbuffer[2] = output>>8;
    txbuffer[3] = output & 0xFF;
    if(!Can_Transmit(&hcan1, txbuffer, 0x200)) return 0;
    else return 1;
}

uint8_t Bark(int16_t position){
    output = (int16_t)PID_Process(&pid1,position, Sensor_Position);
    txbuffer[2] = (uint8_t)(output>>8);
    txbuffer[3] = (uint8_t)(output & 0xFF);
    if(!Can_Transmit(&hcan1, txbuffer, 0x200)) return 0;
    else return 1;
}
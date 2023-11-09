//
// Created by YINING on 2023/11/9.
//
#include "motor.h"
extern uint16_t Sensor_Speed, Sensor_Current, Sensor_Position, Sensor_Row, Sensor_Count;
uint16_t EmeStop, Stop_Count;
PIDParDef pid1={10000.f,500.f,500.f,0,0,0,8000,-8000,0, INF};//电流环
PIDParDef pid2={30.f,5.f,1.f,0,0,0,50000,-50000,0,INF};//速度环
PIDParDef pid3={20.f,0.001f,0.f,0,0,0,3000,-3000,800,INF};//位置环
float tar_speed, tar_current, tar_position;
uint16_t output;
uint8_t txbuffer[8];

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

uint8_t Motor1_Current(uint16_t current){
    txbuffer[0] = (uint8_t )(current>>8);
    txbuffer[1] = (uint8_t)(current & 0xFF);
    if(!Can_Transmit(&hcan1, txbuffer, 0x200)) return 0;
    else return 1;
}

uint8_t Motor1_Speed(float speed){
    //EmeStop_Detection();
    if(EmeStop){
        output = (uint16_t)PID_Process(&pid1,Stop_Count, Sensor_Count);
    }
        output = (uint16_t)PID_Process(&pid2,speed, Sensor_Speed);

    txbuffer[0] = (uint8_t)(output>>8);
    txbuffer[1] = (uint8_t)(output & 0xFF);
    if(!Can_Transmit(&hcan1, txbuffer, 0x200)) return 0;
    else return 1;
}


//
// Created by YINING on 2023/9/23.
//

#ifndef __PID_STRUCT__
#define __PID_STRUCT__
#include "global.h"
typedef struct _PIDParDef
{
    float P;
    float I;
    float D;
    float output;
    float last_output;
    float integral;
    float Max;//最大值，用于限幅
    float Min;//最小项
    float Mid;//近0值
    float delta;//最大变化值，用于恒加速过程
    float last_error;
    float error;
    float pout;
    float dout;
}PIDParDef;
 float PID_Process(PIDParDef *pid, float precedent, float sensor);
#endif


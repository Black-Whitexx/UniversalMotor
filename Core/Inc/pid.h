//
// Created by YINING on 2023/9/23.
//

#ifndef __PID_STRUCT__
#define __PID_STRUCT__
#include "global.h"
typedef struct _PIDParDef
{
    int16_t P;
    int16_t I;
    int16_t D;
    int16_t output;
    int16_t last_output;
    int16_t integral;
    int16_t Max;//最大值，用于限幅
    int16_t Min;//最小项
    int16_t Mid;//近0值
    int16_t delta;//最大变化值，用于恒加速过程
    int16_t last_error;
    int16_t error;
    int16_t pout;
    int16_t dout;
}PIDParDef;
 int16_t PID_Process(PIDParDef *pid, int16_t precedent, int16_t sensor);
#endif


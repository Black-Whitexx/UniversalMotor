//
// Created by YINING on 2023/9/23.
//
#include "pid.h"
float PID_Process(PIDParDef *pid, float precedent, float sensor)
{
    //calulate:
    pid->last_error = pid->error;
    pid->error = precedent - sensor;
    pid->pout = pid->error * pid->P;
    pid->dout  = (pid->error - pid->last_error) * pid->D;
    if(pid->integral > pid->Max) pid->integral = pid->Max;
    pid->integral += pid->error * pid->I;
    pid->output = pid->pout + pid->dout + pid->integral;
    //limit:
    pid->output = (pid->output>pid->Max) ? pid->Max : pid->output;
    pid->output = (pid->output<pid->Min) ? pid->Min : pid->output;
    pid->output = (fabsf(pid->output)<pid->Mid) ? (pid->output>0)?(pid->Mid):(-pid->Mid) : pid->output;//shit code
    pid->output = (pid->output>pid->Max) ? pid->Max : pid->output;
    pid->output = (fabsf(pid->output - pid->last_output) > pid->delta) ? ((pid->output - pid->last_output)>0)?(pid->last_output+pid->delta):(pid->last_output-pid->delta) : pid->output;//shitter code
    return pid->output;
}


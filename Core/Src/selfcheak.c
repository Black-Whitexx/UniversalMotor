//
// Created by YINING on 2023/11/8.
//����Ի:��������ʡ����:Ϊ��ı�����Һ�?�����ѽ������ź�?����ϰ��?�� ���������

#include "selfcheck.h"
extern IWDG_HandleTypeDef hiwdg;
//�Լ�
uint8_t SelfCheck(void){
    HAL_Delay(1);
    HAL_IWDG_Refresh(&hiwdg);
    return 0;
}
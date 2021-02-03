/*
 * balance.h
 *
 * Created: 2020/12/7 17:40:53
 *  Author: Kennys
 */ 


#ifndef BALANCE_H_
#define BALANCE_H_
#define Kp 0.8f
#define Ki 3.5f
#define Increament_Max 90.0f/Ki
#define Kd 0.0f*PID_Freq
#define PID_Freq 66.7f
#include "../App/servo_joystick.h"
struct PID_Para
{
	float last_error;
	float increment_error;
};

//float PID(float error, float frequency, struct PID_Para Parameter);
float PID(float error, float last_error, float increment_error);
void BalanceControl(float pAngle[3]);


#endif /* BALANCE_H_ */
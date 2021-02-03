/*
 * main.h
 *
 * Created: 2020/12/7 15:20:47
 *  Author: Kennys
 */ 
#include "../lib/uart.h"
#include "../include/MahonyIMU.h"
#include "../include/mpu6050.h"
#include "../app/task.h"
#include "mytest.h"
#include "servo_joystick.h"

#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

extern float q0;
extern float q1;
extern float q2;
extern float q3;
extern float integralFBz;
extern float integralFBy;
extern float integralFBx;

#endif /* MAIN_H_ */
/*
 * balance.c
 *
 * Created: 2020/12/7 17:40:45
 *  Author: Kennys
 */ 
#include "balance.h"
#include "../lib/uart.h"
#include <stdio.h>
extern char MyString[10];

float PID(float error, float last_error, float increment_error)
{
	static float output;
	output = (float)error*Kp+increment_error*Ki+(error-last_error)*Kd;

	if (output>85)
	{
		output = 85;
	}
	if (output<-85)
	{
		output = -85;
	}
	output += 90;
	return output;

}
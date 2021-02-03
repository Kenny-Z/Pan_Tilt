/*
 * task.c
 *
 * Created: 2020/12/7 15:42:33
 *  Author: Kennys
 */ 

/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/sleep.h>

/* Scheduler include files. */
#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/queue.h"
#include "../FreeRTOS/semphr.h"
/* TASK include file. */
#include "../lib/uart.h"
#include "../include/MahonyIMU.h"
#include "../include/MPU6050.h"
#include "../include/balance.h"
/* system time include file. */
#include "time.h"
extern float AccData[3];
extern float GyroData[3];
extern float Angle[3];
extern char MyString[10];
float ADC_Value;
// static TickType_t time1;
// static TickType_t time2;
// static int period;
void vMahonyTaskFunction( void *pvParameters )
{
	for( ;; )
	{
		TickType_t xLastWakeTimeMahony;
		const TickType_t xPeriodFrequencyMahony = 1;// 15ms per tick

		xLastWakeTimeMahony = xTaskGetTickCount();

		for( ;; )
		{
			vTaskDelayUntil( &xLastWakeTimeMahony, xPeriodFrequencyMahony );
// 			time1 = xTaskGetTickCount();
// 			sprintf(MyString,"time1: %i ms\n",time1);
// 			UART_putstring(MyString);
			MahonyAHRSupdateIMU(GyroData[0],GyroData[1],GyroData[2],AccData[0],AccData[1],AccData[2]);
			q2Angle(Angle);
// 			sprintf(MyString,"Angle x: %f ms\n",Angle[0]);
// 			UART_putstring(MyString);
// 			sprintf(MyString,"Angle y: %f ms\n",Angle[1]);
// 			UART_putstring(MyString);
// 			time2 = xTaskGetTickCount();
// 			sprintf(MyString,"time1: %i ms\n",time2);
// 			UART_putstring(MyString);
// 			period = (time2-time1)*15;
// 			sprintf(MyString,"Task Mahony: %i ms\n",period);
// 			UART_putstring(MyString);
		}
	}
	vTaskDelete( NULL );
}
void vMPU6050TaskFunction( void *pvParameters )
{
	for( ;; )
	{
		TickType_t xLastWakeTimeMPU;
		const TickType_t xPeriodFrequencyMPU = 1;// 15ms per tick
		
		xLastWakeTimeMPU = xTaskGetTickCount();

		for( ;; )
		{
			vTaskDelayUntil( &xLastWakeTimeMPU, xPeriodFrequencyMPU );
//			TickType_t time1 = xTaskGetTickCount();
			MPU_read(AccData,GyroData);
// 			TickType_t time2 = xTaskGetTickCount();
// 			sprintf(MyString,"Task MPU: %i ms\n",(time2-time1)*15);
// 			UART_putstring(MyString);
		}
	}
	vTaskDelete( NULL );
}

// Task C for control
void vControlTaskFunction( void *pvParameters )
{
	for( ;; )
	{
		TickType_t xLastWakeTimeControl;
		const TickType_t xPeriodFrequencyControl = 1;// 15ms per tick  15ms*93 = 1395
		static float x_error;
		static float y_error;
		static float DEG1;
		static float DEG2;
		static float last_error_x;
		static float inc_error_x;
		static float last_error_y;
		static float inc_error_y;		
		xLastWakeTimeControl = xTaskGetTickCount();

		for( ;; )
		{
			vTaskDelayUntil( &xLastWakeTimeControl, xPeriodFrequencyControl );
			x_error = - Angle[0];
			y_error = - Angle[1];
			DEG1 = PID(x_error,last_error_x,inc_error_x);
			DEG2 = PID(y_error,last_error_y,inc_error_y);
			if (abs(inc_error_x)<Increament_Max)
			{
				inc_error_x += x_error/PID_Freq;
			}
			last_error_x = x_error;
			if (abs(inc_error_y)<Increament_Max)
			{
				inc_error_y += y_error/PID_Freq;
			}
			last_error_y = y_error;
// 			sprintf(MyString,"Y %f \n",Angle[1]);
// 			UART_putstring(MyString);

			deg_to_servo1(DEG1);
			deg_to_servo2(DEG2);
/*			BalanceControl(Angle);*/
// 			sprintf(MyString,"Task Control\n");
// 			UART_putstring(MyString);
			
		}
	}
	vTaskDelete( NULL );
}
void vADCTaskFunction( void *pvParameters )
{
	for( ;; )
	{
		TickType_t xLastWakeTimeADC;
		const TickType_t xPeriodFrequencyADC = 1;// 15ms per tick  15ms*93 = 1395
		
		xLastWakeTimeADC = xTaskGetTickCount();

		for( ;; )
		{
			vTaskDelayUntil( &xLastWakeTimeADC, xPeriodFrequencyADC);
			//ADC
			ADC_Value = ADC;
			if (ADC_Value>800)
			{
				OCR0B--;
				if (OCR0B<=10)
				{
					OCR0B=11;
				}
			}
			if (ADC_Value<300)
			{
				OCR0B++;
				if (OCR0B>=40)
				{
					OCR0B=39;
				}
			}
//			OCR0B = (float)ADC*(40-10)/1024+10;
// 			sprintf(MyString,"Task ADC:%f\n",ADC_Value);
// 			UART_putstring(MyString);
// 			
		}
	}
	vTaskDelete( NULL );
}
//
void MytaskCreate(void)
{
	/* Create tasks and make some magic*/

	xTaskCreate( vMahonyTaskFunction,"TASKA",256,NULL,3,NULL);
	xTaskCreate( vMPU6050TaskFunction,"TASKB",256,NULL, 2,NULL);
	xTaskCreate( vControlTaskFunction,"TASKC",256,NULL,4,NULL);
	xTaskCreate( vADCTaskFunction,"TASKD",128,NULL,4,NULL);
	
}

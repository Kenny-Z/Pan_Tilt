/*
 * test.c
 *
 * Created: 2020/12/7 23:29:15
 *  Author: Kennys
 */ 

#include "../lib/uart.h"
#include "../include/mpu6050.h"
#include "../include/MahonyIMU.h"
#include "../include/balance.h"
#include "servo_joystick.h"
#include <avr/interrupt.h>

extern float AccData[3];
extern float GyroData[3];
extern float Angle[3];
extern char MyString[10];
extern float ADC_Value;

void speedtest(void)
{

	static float speedtime1;
	static float speedtime2;	
	static float speedperiod;	
	cli();
	// TIMER SET 256 PRESCALER 16M/256 = 62500 Hz
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	//Timer 1 Normal Mode
	TCCR1A  &= ~(1<<WGM10);
	TCCR1A  &= ~(1<<WGM11);
	TCCR1B  &= ~(1<<WGM12);
	TCCR1B  &= ~(1<<WGM13);
	//Clear flag when initializing
	TIFR1 |= (1<<ICF1);
	sei();								//Enable global int	
// 	while (1)
// 	{
// 		//BalanceControl(Angle);
// // 	ADC_Value = ADC;
// // 	OCR0B = (float)ADC*(40-10)/1024+10;
// 	
// 	}
// }


	for (int i = 0;i<100;i++)
	{
		speedtime1 = TCNT1;
// 		MPU_read(AccData,GyroData);
// 		MahonyAHRSupdateIMU(GyroData[0],GyroData[1],GyroData[2],AccData[0],AccData[1],AccData[2]);
// 		q2Angle(Angle);
		//ADC_Value = ADC;
		BalanceControl(Angle);
		speedtime2 = TCNT1;
// 		sprintf(MyString,"speedtime2: %f ms\n",speedtime2);
// 		UART_putstring(MyString);
		speedperiod = speedtime2-speedtime1;
		if(speedperiod<0)
		{speedperiod = speedtime2 + 65535 - speedtime1;}
		speedperiod = (speedtime2 - speedtime1)*(float)1000/62500;
		sprintf(MyString,"Task control: %f ms\n",speedperiod);
		UART_putstring(MyString);
		

// 		sprintf(MyString,"Angle x: %f ms\n",Angle[0]);
// 		UART_putstring(MyString);
// 		sprintf(MyString,"Angle y: %f ms\n",Angle[0]);
// 		UART_putstring(MyString);
// 		sprintf(MyString,"ACC z: %f ms\n",AccData[2]);
// 		UART_putstring(MyString);
// 		sprintf(MyString,"GYRO x: %f ms\n",GyroData[0]);
// 		UART_putstring(MyString);
	}
}
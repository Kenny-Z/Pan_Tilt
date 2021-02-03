#include "mpu6050.h"

float AccData[3]={1,1,1};
float GyroData[3] = {1,1,1};
float Angle[3];
float acc_calib[3];
float gyro_calib[3];
float Temperature;

void MPU_Init(void)
{
	//
	_delay_ms(150);										/* Power up time >100ms */
	//0x68
	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x6B);								/* Write to power management register */
	I2C_Write(0x01);									//select the clock X-axis 
	I2C_Stop();

	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x1A);								//Low pass digital filter
	I2C_Write(0x05);									
	I2C_Stop();
	
	/*
	//1B GyroScale 1000 degree/s
	bit4bit3
	00	+- 250 degrees/s
	01	+- 500 degrees/s
	10	+- 1000 degrees/s
	11	+- 2000 degrees/s
	*/
	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x1B);			
	I2C_Write(0x10);
	I2C_Stop();
	/*
	//1C AccConfig 8g
	bit4bit3
	00	+- 2 g
	01	+- 4 g
	10	+- 8 g
	11	+- 16 g
	*/
	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x1C);
	I2C_Write(0x10);
	I2C_Stop();
	//disable INT 
	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x38);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
	//
	I2C_Start_Wait(0xD0);								/* Start with device write address */
	I2C_Write(0x19);								/* Write to sample rate register */
	I2C_Write(0x07);									/* 1KHz sample rate */
	I2C_Stop();
	
	MPU_Calib(AccData,GyroData);
}

void MPU_read(float Acc[3], float Gyro[3])
{
	I2C_Start_Wait(MPU6050_addr);
	I2C_Write(0x3B);
	I2C_Repeated_Start(0xD1);
	Acc[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Temperature = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
	//_delay_ms(10);
	//transform the raw data to corresponding range
	for (int i=0;i<3;i++)
	{
		Acc[i] =  Acc[i]*8*9.8/32768 - acc_calib[i]; //range of +- 8g
		Gyro[i] = Gyro[i]*1000/32768/180*3.14 - gyro_calib[i]; //range of +- 1000 degrees /180*pi 
	}
}

void MPU_Calib(float Acc[3], float Gyro[3])
{
	static float calib_loops = 1000;
	static float sum_acc_cal_sum[3]={0,0,0};
	static float sum_gyro_cal_sum[3]={0,0,0};
	for (int i =0;i<calib_loops;i++)
	{ 
		MPU_read(Acc,Gyro);
		for (int j=0;j<3;j++)
		{
			sum_acc_cal_sum[j] += Acc[j];
			sum_gyro_cal_sum[j] += Gyro[j];
		}
	}
	for (int j=0;j<3;j++)
	{
		acc_calib[j] =  sum_acc_cal_sum[j]/(float)calib_loops;
		gyro_calib[j] = sum_gyro_cal_sum[j]/(float)calib_loops;
	}
	acc_calib[2] -= 10;
}


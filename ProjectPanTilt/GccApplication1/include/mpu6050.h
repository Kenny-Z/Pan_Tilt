/*
 * mpu6050.h
 *
 * Created: 2020/11/25 11:42:18
 *  Author: Kennys
 */ 


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "..\lib\I2C_Master_H_file.h"
#include "..\lib\uart.h"
#include "MahonyIMU.h"

#define MPU6050_addr 0xD0
void MPU_Init(void);
void MPU_read(float Acc[3], float Gyro[3]);
void MPU_Calib(float Acc[3], float Gyro[3]);

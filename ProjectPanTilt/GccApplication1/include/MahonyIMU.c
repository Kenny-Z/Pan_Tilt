/*
 * MahonyIMU.c
 * It is based on a open-source IMU algorithm.
 * Details can be found https://x-io.co.uk/open-source-ahrs-with-x-imu/
 * Created: 2020/11/20 10:11:50
 *  Author: KennyZh
 */

#include <stdio.h>
#include <math.h>
#include "MahonyIMU.h"
#include "mpu6050.h"

float q0 = 1;
float q1 = 0;
float q2 = 0;
float q3 = 0;

float invSqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   

	return y;
}

void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) 
{
	
	float recipNorm; 

	float halfvx, halfvy, halfvz; 

	float halfex, halfey, halfez; 

	float qa, qb, qc;
	
	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)

	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement

		recipNorm = invSqrt(ax * ax + ay * ay + az * az);

		ax *= recipNorm;

		ay *= recipNorm;

		az *= recipNorm;

		// Estimated direction of gravity and vector perpendicular to magnetic flux

		halfvx = q1 * q3 - q0 * q2;

		halfvy = q0 * q1 + q2 * q3;

		halfvz = q0 * q0 - 0.5f + q3 * q3;

		// Error is sum of cross product between estimated and measured direction of gravity

		halfex = (ay * halfvz - az * halfvy);

		halfey = (az * halfvx - ax * halfvz);

		halfez = (ax * halfvy - ay * halfvx);

		gx += twoKp * halfex;

		gy += twoKp * halfey;

		gz += twoKp * halfez;

	}

	// Integrate rate of change of quaternion

	gx *= (0.5f * (1.0f / sampleFreq));

	gy *= (0.5f * (1.0f / sampleFreq));

	gz *= (0.5f * (1.0f / sampleFreq));

	qa = q0;

	qb = q1;

	qc = q2;

	//update

	q0 += (-qb * gx - qc * gy - q3 * gz);

	q1 += (qa * gx + qc * gz - q3 * gy);

	q2 += (qa * gy - qb * gz + q3 * gx);

	q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion

	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);

	q0 *= recipNorm;

	q1 *= recipNorm;

	q2 *= recipNorm;

	q3 *= recipNorm;

}

void q2Angle(float angle[3])
{
	angle[2] = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw

	angle[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch

	angle[0] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}
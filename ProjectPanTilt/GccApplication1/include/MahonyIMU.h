/*
 * MahonyIMU.h
 *
 * Created: 2020/11/20 10:17:52
 *  Author: Kennys
 */ 


#ifndef MAHONYIMU_H_
#define MAHONYIMU_H_


void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void q2Angle(float angle[3]);

#define twoKp 1.0f
#define sampleFreq 66.7f




#endif /* MAHONYIMU_H_ */
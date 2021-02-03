/*
 * servo_joystick.h
 *
 * Created: 2020/12/7 15:57:25
 *  Author: Kennys
 */ 


#ifndef SERVO_JOYSTICK_H_
#define SERVO_JOYSTICK_H_

void ADC_Init();
void deg_to_servo1 (int deg);
void deg_to_servo2 (int deg);
void initialize_pwm_2servos ();
void initialize_pwm_joyservo ();
#endif /* SERVO_JOYSTICK_H_ */
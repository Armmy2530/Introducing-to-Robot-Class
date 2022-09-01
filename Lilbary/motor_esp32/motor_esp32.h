#pragma once
#ifndef motor_esp32_H
#define motor_esp32_H

#include<Arduino.h>
#include <pwmWrite.h>

class Motor
{
public:
    Motor(int motor_A, int motor_B, int motor_PWM);
    void m(int speed);
protected:
    int A_pin, B_pin, PWM_pin;
};

#endif
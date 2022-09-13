#include<Arduino.h>
class Motor
{
public:
    int A_pin, B_pin, PWM_pin;
    Motor(int motor_A, int motor_B, int motor_PWM)
    {
        A_pin = motor_A;
        B_pin = motor_B;
        PWM_pin = motor_PWM;
        pinMode(A_pin, OUTPUT);
        pinMode(B_pin, OUTPUT);
        pinMode(PWM_pin, OUTPUT);
    }

    void m(int speed)
    {
        if (speed > 0)
        {
            digitalWrite(A_pin, HIGH);
            digitalWrite(B_pin, LOW);
            analogWrite(PWM_pin, speed);
        }
        if (speed < 0)
        {
            digitalWrite(A_pin, LOW);
            digitalWrite(B_pin, HIGH);
            analogWrite(PWM_pin, -speed);
        }
        if (speed == 0)
        {
            digitalWrite(A_pin, LOW);
            digitalWrite(B_pin, LOW);
            analogWrite(PWM_pin, 0);
        }
        if (speed == -999)
        {
            digitalWrite(A_pin, LOW);
            digitalWrite(B_pin, LOW);
            analogWrite(PWM_pin, 255);
        }
    }
};

class Dual_motor
{
public:
    Dual_motor(Motor &L, Motor &R) : _L(L), _R(R) {}
    void m(int L, int R)
    {
        _L.m(L);
        _R.m(R);
    }
private:
   Motor _L;
   Motor _R;
};

#include "motor_uno.cpp"
// Motor L
int enA = 10;
int in1 = 8;
int in2 = 9;
// Motor R
int enB = 11;
int in3 = 13;
int in4 = 12;

// sensor_pin
int L2_pin = 3, L2_value = 0;
int L1_pin = 4, L1_value = 0;
int C_pin = 5, C_value = 0;
int R1_pin = 6, R1_value = 0;
int R2_pin = 7, R2_value = 0;

int turn_ms = 20;
int fd_speed = 80;
int turn_speed = 20;

int baseSpeed = 70;
int maxSpeed = 255;
int motorSpeed;
int leftSpeed, rightSpeed;
int Kp = 23;
int Kd = 4;
float Ki = 0;
int error = 0, pre_error = 0, sum_error;

bool W(int n)
{
    if (n == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool B(int n)
{
    if (n == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
Motor L_motor(in1, in2, enA);
Motor R_motor(in3, in4, enB);

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world");
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(2, INPUT);
    delay(500);
    trackline_R(60, 1);
    tr_sensor(80, 80);
    trackline_R(60, 5);
    tr_sensor(80, 80);
    delay(300);
    bd(40, 40);
    delay(300);
    stop(false);
    fd(80,80);
    delay(600);
    stop(false);
}

void loop()
{
}

void m(int l, int r)
{
    L_motor.m(l);
    R_motor.m(r);
}

void readSensor()
{
    L2_value = digitalRead(L2_pin);
    L1_value = digitalRead(L1_pin);
    C_value = digitalRead(C_pin);
    R1_value = digitalRead(R1_pin);
    R2_value = digitalRead(R2_pin);
}

void trackline_1(int fd_speed, int turn_speed, int turn_ms)
{
    readSensor();
    if (L1_value == 1 && R1_value == 1)
    {
        fd(fd_speed, fd_speed);
    }
    else if (L1_value == 0 && R1_value == 1)
    {
        sl(turn_speed, turn_speed);
        delay(turn_ms);
    }
    else if (L1_value == 1 && R1_value == 0)
    {
        sr(turn_speed, turn_speed);
        delay(turn_ms);
    }
    else
    {
        stop(false);
    }
}

void trackline_pid()
{
    readSensor();
    if (W(L2_value) && W(L1_value) && W(C_value) && B(R1_value))
    {
        error = 3; // w w w w b
    }
    else if (W(L2_value) && W(L1_value) && B(C_value) && B(R1_value))
    {
        error = 2; // w w w b b
    }
    else if (W(L2_value) && W(L1_value) && B(C_value) && W(R1_value))
    {
        error = 1; // w w w b w
    }
    else if (W(L2_value) && B(L1_value) && B(C_value) && W(R1_value))
    {
        error = 0; // w w b b w
    }
    else if (W(L2_value) && B(L1_value) && W(C_value) && W(R1_value))
    {
        error = -1; // w w b w w
    }
    else if (B(L2_value) && B(L1_value) && W(C_value) && W(R1_value))
    {
        error = -2; // w b b w w
    }
    else if (B(L2_value) && W(L1_value) && W(C_value) && W(R1_value))
    {
        error = -3; // w b w w w
    }
    else if (W(L2_value) && W(L1_value) && W(C_value) && W(R1_value))
    {
        error = pre_error; // w w w w w
    }
    motorSpeed = (int)(Kp * error + Kd * (error - pre_error) + Ki * (sum_error));
    leftSpeed = baseSpeed + motorSpeed;
    rightSpeed = baseSpeed - motorSpeed;

    if (leftSpeed > maxSpeed)
        leftSpeed = maxSpeed;
    if (rightSpeed > maxSpeed)
        rightSpeed = maxSpeed;

    if (leftSpeed < -maxSpeed)
        leftSpeed = -maxSpeed;
    if (rightSpeed < -maxSpeed)
        rightSpeed = -maxSpeed;
    m(leftSpeed, rightSpeed);
    pre_error = error;
    sum_error += error;
}
void trackline_R(int turn_speed, int line)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid();
        if (B(R2_value) && B(R1_value))
        {
            delay(100);
            if (B(R2_value) && B(R1_value))
            {
                while (B(R2_value) && B(R1_value))
                {
                    fd(fd_speed, fd_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    stop(false);
}

void trackline_L(int turn_speed, int line)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid();
        if (B(L2_value) && B(L1_value))
        {
            delay(100);
            if (B(L2_value) && B(L1_value))
            {
                while (B(L2_value) && B(L1_value))
                {
                    fd(fd_speed, fd_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    stop(false);
}

void trackline_Cross(int turn_speed, int line)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid();
        if (B(R1_value) && B(L1_value))
        {
            delay(100);
            if (R1_value == 0 && L1_value == 0)
            {
                while (L1_value == 0 && R1_value == 0)
                {
                    fd(fd_speed, fd_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    stop(false);
}

void tr_sensor(int L_speed, int R_speed)
{
    fd(L_speed, R_speed);
    delay(100);
    stop(false);
    delay(100);
    sr(L_speed, R_speed);
    delay(400);
    stop(false);
    readSensor();
    while (C_value == 1)
    {
        sr(L_speed, R_speed);
        readSensor();
    }
    delay(100);
    stop(false);
}

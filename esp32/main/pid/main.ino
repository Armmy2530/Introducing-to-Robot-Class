#include <motor_esp32.h>
#include <pwmWrite.h>
#include <PS4Controller.h>

// Motor L
int enA = 27;
int in1 = 26;
int in2 = 25;
// Motor R
int enB = 13;
int in3 = 14;
int in4 = 12;

Motor R_motor(in3, in4, enB);
Motor L_motor(in1, in2, enA);

// sensor_pin
int L2_pin = 33, L2_value = 0;
int L1_pin = 32, L1_value = 0;
int C_pin = 35, C_value = 0;
int R1_pin = 34, R1_value = 0;
int R2_pin = 39, R2_value = 0;

int ref_sensor[5] = {1000, 2350, 2948, 2748, 2348};
boolean sensor_bool[5] = {1, 1, 1, 1, 1};
int sensor_val[5] = {1, 1, 1, 1, 1};
int dif_ref = 500;

int baseSpeed = 150;
int maxSpeed = 255;
int motorSpeed;
int leftSpeed, rightSpeed;
int Kp = 17;
int Kd = 1;
float Ki = 00.0000001;
int error = 0, pre_error = 0, sum_error;


int turn_ms = 20;
int fd_speed = 80;
int turn_speed = 20;

int ref = 2500;

void setup()
{
    Serial.begin(115200);
    calibrate_sensor(5);
    delay(10000);
    readSensor();
    trackline_R(100,1,50);
    tr_sensor(150,0);
    trackline_Cross(100,5,50);
    tl_sensor(0,150);
    m(100,100);
    delay(500);
    m(0,0);
}
void loop()
{
}
void readSensor()
{
    sensor_val[0] = analogRead(L2_pin);
    sensor_val[1] = analogRead(L1_pin);
    sensor_val[2] = analogRead(C_pin);
    sensor_val[3] = analogRead(R1_pin);
    sensor_val[4] = analogRead(R2_pin);
    for (int n = 0; n <= 4; n++)
    {
        sensor_bool[n] = W(sensor_val[n], ref_sensor[n]);
    }
    L2_value = int(sensor_bool[0]);
    L1_value = int(sensor_bool[1]);
    C_value  = int(sensor_bool[2]);
    R1_value = int(sensor_bool[3]);
    R2_value = int(sensor_bool[4]);
}

void calibrate_sensor(int n_check)
{
    readSensor();
    int white_val[5] = {0, 0, 0, 0, 0};
    int black_val[5] = {0, 0, 0, 0, 0};
    int avg_val[5] = {0, 0, 0, 0, 0};
    int confirm_black = 0;
    Serial.println("Start Calibrat sensor ..........");
    for (int i = 0; i < n_check; i++)
    {
        for (int n = 0; n <= 4; n++)
        {
            readSensor();
            white_val[n] = sensor_val[n];
        }
        m(120, 120);
        while (true)
        {
            readSensor();
            for (int i = 0; i <= 4; i++)
            {
                readSensor();
                if (abs(sensor_val[i] - white_val[i]) >= dif_ref)
                {
                    confirm_black++;
                }
            }
            if (confirm_black == 5)
            {
                for (int n = 0; n <= 4; n++)
                {
                    black_val[n] = sensor_val[n];
                }
                delay(50);
                m(0, 0);
                confirm_black = 0;
                break;
            }
            else
                confirm_black = 0;
        }
        for (int n = 0; n <= 4; n++)
        {
            if (avg_val[n] = 0)
            {
                avg_val[n] = (white_val[n] + black_val[n]) / 2;
            }
            else
            {
                avg_val[n] = avg_val[n] + ((white_val[n] + black_val[n]) / 2) / 2;
            }
        }
        Serial.print("loop: ");
        Serial.print(i);
        for (int n = 0; n <= 4; n++)
        {
            Serial.print(" ");
            Serial.print(avg_val[n]);
        }
        Serial.println();
        while (true)
        {
            readSensor();
            // for (int n = 0; n <= 4; n++)
            // {
            //     Serial.print(avg_val[n]);
            //     Serial.print(" ");
            //     Serial.print(sensor_val[n]);
            //     Serial.print(" ");
            //     Serial.print(W(sensor_val[n],avg_val[n]));
            //     Serial.print(" ");
            // }
            if((W(sensor_val[0], avg_val[0]) && W(sensor_val[1], avg_val[1]) && W(sensor_val[2], avg_val[2]) && W(sensor_val[3], avg_val[3]) && W(sensor_val[4], avg_val[4]))){Serial.print("True ");break;}
            // Serial.println();
            m(-100, -100);
        }
        delay(500);
        m(0, 0);
        delay(1000);
    }
    for (int n = 0; n <= 4; n++)
    {
        ref_sensor[n] = avg_val[n];
        Serial.print("Final Ref");
        Serial.print(" ");
        Serial.print(ref_sensor[n]);
    }
    Serial.println();
}

void m(int l, int r)
{
    if (l > 255)
    {
        l = 255;
    }
    if (r > 255)
    {
        r = 255;
    }
    L_motor.m(l);
    R_motor.m(r);
}

bool W(int n, int ref)
{
    if (n > ref)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool B(int n, int ref)
{
    if (n < ref)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void trackline_pid()
{
    readSensor();
    if (W(L2_value) && W(L1_value) && W(C_value) && W(R1_value) && B(R2_value))
    {
        error = 4; // w w w w b
    }
    else if (W(L2_value) && W(L1_value) && W(C_value) && B(R1_value) && B(R2_value))
    {
        error = 3; // w w w b b
    }
    else if (W(L2_value) && W(L1_value) && W(C_value) && B(R1_value) && W(R2_value))
    {
        error = 2; // w w w b w
    }
    else if (W(L2_value) && W(L1_value) && B(C_value) && B(R1_value) && W(R2_value))
    {
        error = 1; // w w b b w
    }
    else if (W(L2_value) && W(L1_value) && B(C_value) && W(R1_value) && W(R2_value))
    {
        error = 0; // w w b w w
    }
    else if (W(L2_value) && B(L1_value) && B(C_value) && W(R1_value) && W(R2_value))
    {
        error = -1; // w b b w w
    }
    else if (W(L2_value) && B(L1_value) && W(C_value) && W(R1_value) && W(R2_value))
    {
        error = -2; // w b w w w
    }
    else if (B(L2_value) && B(L1_value) && W(C_value) && W(R1_value) && W(R2_value))
    {
        error = -3; // b b w w w
    }
    else if (B(L2_value) && W(L1_value) && W(C_value) && W(R1_value) && W(R2_value))
    {
        error = -4; // b w w w w
    }
    else if (W(L2_value) && W(L1_value) && W(C_value) && W(R1_value) && W(R2_value))
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
void trackline_R(int turn_speed, int line,int ms_delay)
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
    delay(ms_delay);
    stop(false);
}

void trackline_L(int turn_speed, int line,int ms_delay)
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
    delay(ms_delay);
    stop(false);
}

void trackline_Cross(int turn_speed, int line,int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid();
        if (B(R2_value) && B(L2_value))
        {
            delay(100);
            if (R2_value == 0 && L2_value == 0)
            {
                while (L2_value == 0 && R2_value == 0)
                {
                    fd(fd_speed, fd_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    delay(ms_delay);
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


void tl_sensor(int L_speed, int R_speed)
{
    fd(L_speed, R_speed);
    delay(100);
    stop(false);
    delay(100);
    sl(L_speed, R_speed);
    delay(400);
    stop(false);
    readSensor();
    while (C_value == 1)
    {
        sl(L_speed, R_speed);
        readSensor();
    }
    delay(100);
    stop(false);
}

#include <motor_esp32.h>
#include <pwmWrite.h>
#include <PS4Controller.h>

// Motor L
int enA = 27;
int in1 = 26;
int in2 = 33;
// Motor R
int enB = 13;
int in3 = 14;
int in4 = 12;

Motor R_motor(in3, in4, enB);
Motor L_motor(in1, in2, enA);

// sensor_pin
int L2_pin = 25;
int L1_pin = 32;
int C_pin  = 35;
int R1_pin = 34;
int R2_pin = 39;

int ref_sensor[5] = {1000, 2350, 2948, 2748, 2348};
boolean sensor_bool[5] = {1, 1, 1, 1, 1};
int sensor_val[5] = {1, 1, 1, 1, 1};
int dif_ref = 300;

int turn_ms = 20;
int fd_speed = 80;
int turn_speed = 20;

int ref = 2500;

void setup()
{
    pinMode(L2_pin,INPUT);
    pinMode(L1_pin,INPUT);
    pinMode(C_pin,INPUT);
    pinMode(R1_pin,INPUT);
    pinMode(R2_pin,INPUT);
    Serial.begin(115200);
    calibrate_sensor(5);
}
void loop()
{
    readSensor();
    for (int n = 0; n <= 4; n++)
    {
        Serial.print(sensor_val[n]);
        Serial.print(" ");
    }
    for (int n = 0; n <= 4; n++)
    {
        Serial.print(sensor_bool[n]);
    }
    Serial.println();
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
            for (int n = 0; n <= 4; n++)
            {
                Serial.print(sensor_val[n]);
                Serial.print(" ");
            }
            Serial.println(" ");
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
            if ((W(sensor_val[0], avg_val[0]) && W(sensor_val[1], avg_val[1]) && W(sensor_val[2], avg_val[2]) && W(sensor_val[3], avg_val[3]) && W(sensor_val[4], avg_val[4])))
            {
                Serial.print("True ");
                break;
            }
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

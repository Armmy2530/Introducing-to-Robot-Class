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
boolean sensor_bool[5] = {1,1,1,1,1};
int sensor_val[5] = {1,1,1,1,1};

int turn_ms = 20;
int fd_speed = 80;
int turn_speed = 20;

int ref = 2500;

void setup()
{
    Serial.begin(115200);
}
void loop()
{
    readSensor();
    for(int n = 0; n<=5 ; n++){
        Serial.print(sensor_val[n]);
        Serial.print(" ");
    }
    for(int n = 0; n<=5 ; n++){
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
    for(int n = 0; n<=5 ; n++){
        sensor_bool[n] = W(sensor_val[n],ref_sensor[n]);
    }
}
void m(int l, int r)
{
    if(l > 255){l = 255;}
    if(r > 255){r = 255;}
    L_motor.m(l);
    R_motor.m(r);
}

bool W(int n,int ref)
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
bool B(int n,int ref)
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

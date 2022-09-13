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

float current_L = 0, pev_L = 0, target_L = 0;
float current_R = 0, pev_R = 0, target_R = 0;
float freerange = 17;
// drive mode 2 setting
int min_percentage = 25;
int max_percentage = 125;

Motor R_motor(in3, in4, enB);
Motor L_motor(in1, in2, enA);

// sensor_pin
int L2_pin = 33, L2_value = 0;
int L1_pin = 32, L1_value = 0;
int C_pin  = 35, C_value = 0;
int R1_pin = 34, R1_value = 0;
int R2_pin = 39, R2_value = 0;

int turn_ms = 20;
int fd_speed = 80;
int turn_speed = 20;

int ref = 2500;

int baseSpeed = 170;
int maxSpeed = 255;
int motorSpeed;
int leftSpeed, rightSpeed;
int Kp = 14;
int Kd = 4;
float Ki = 0;
int error = 0, pre_error = 0, sum_error;

bool W(int n)
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
bool B(int n)
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


struct Remote
{
  int L_X;
  int L_Y;
  int R_X;
  int R_Y;
  boolean L1;
  boolean R1;
  int L2;
  int R2;
  boolean L3;
  boolean R3;
  boolean Option;
  boolean Share;
  boolean Up;
  boolean Down;
  boolean Left;
  boolean Right;
  boolean Square;
  boolean Cross;
  boolean Circle;
  boolean Triangle;
};

Remote controller_data;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world");
}

void loop()
{
    readSensor();
    trackline_pid();
    Serial.print(W(L2_value));
    Serial.print(W(L1_value));
    Serial.print(W(C_value));
    Serial.print(W(R1_value));
    Serial.print(W(R2_value));
    Serial.print(" ");
    Serial.print(B(L2_value));
    Serial.print(B(L1_value));
    Serial.print(B(C_value));
    Serial.print(B(R1_value));
    Serial.print(B(R2_value));
    Serial.println(" ");
}
void readSensor()
{
    L2_value = analogRead(L2_pin);
    L1_value = analogRead(L1_pin);
    C_value  = analogRead(C_pin);
    R1_value = analogRead(R1_pin);
    R2_value = analogRead(R2_pin);
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

void m(int l, int r)
{
    if(l > 255){l = 255;}
    if(r > 255){r = 255;}
    L_motor.m(l);
    R_motor.m(r);
}


bool isinFreerange(int value, float freerange)
{
    int percent = round(125 * (freerange / 100));
    int min_per = 0 - percent;
    int max_per = 0 + percent;
    // Serial.print("value:");
    // Serial.print(value);
    // Serial.print("freerange:");
    // Serial.print(freerange);
    // Serial.print("percent:");
    // Serial.print(percent);
    // Serial.print("min:");
    // Serial.print(min_per);
    // Serial.print("max:");
    // Serial.print(max_per);
    // Serial.print("true:");
    // Serial.println(value >= min_per && value <= max_per);
    if(value >= min_per && value <= max_per)return true; else return false;
}

void getRemote()
{
  controller_data.L_X = PS4.LStickX();
  controller_data.L_Y = PS4.LStickY();
  controller_data.R_X = PS4.RStickX();
  controller_data.R_Y = PS4.RStickY();
  controller_data.Right = PS4.Right();
  controller_data.Down = PS4.Down();
  controller_data.Up = PS4.Up();
  controller_data.Left = PS4.Left();
  controller_data.Square = PS4.Square();
  controller_data.Cross = PS4.Cross();
  controller_data.Circle = PS4.Circle();
  controller_data.Triangle = PS4.Triangle();
  controller_data.L1 = PS4.L1();
  controller_data.R1 = PS4.R1();
  controller_data.Share = PS4.Share();
  controller_data.Option = PS4.Options();
  controller_data.L3 = PS4.L3();
  controller_data.R3 = PS4.R3();
  controller_data.L2 = PS4.L2Value();
  controller_data.R2 = PS4.R2Value();
}

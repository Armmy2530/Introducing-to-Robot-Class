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
    PS4.begin("7c:9e:bd:48:4f:5a");
}

void loop()
{
    getRemote();
    if (isinFreerange(controller_data.L_X, freerange) && isinFreerange(controller_data.L_Y, freerange))
    {
        target_L = 0;
        target_R = 0;
        current_L = target_L;
        current_R = target_R;
        pev_L = current_L;
        pev_R = current_R;
    }
    else if (isinFreerange(controller_data.L_Y, freerange))
    {
        target_L = float(controller_data.L_X) / 128 * 255;
        target_R = 0 - float(controller_data.L_X) / 128 * 255;
        current_L = target_L;
        current_R = target_R;
        pev_L = current_L;
        pev_R = current_R;
    }
    else if (isinFreerange(controller_data.L_X, freerange))
    {
        target_L = float(controller_data.L_Y) / 128 * 255;
        target_R = float(controller_data.L_Y) / 128 * 255;
        current_L = target_L;
        current_R = target_R;
        pev_L = current_L;
        pev_R = current_R;
    }
    else if (controller_data.L_X > 0)
    {
        target_R = (255 / 100) * (float(controller_data.L_Y) / 128) * float(map(controller_data.L_X, 0, 128, max_percentage, min_percentage));
        target_L = 255 * (float(controller_data.L_Y) / 128);
        current_L = target_L;
        current_R = target_R;
        pev_L = current_L;
        pev_R = current_R;
    }
    else if (controller_data.L_X < 0)
    {
        target_L = (255 / 100) * (float(controller_data.L_Y) / 128) * float(map(controller_data.L_X, -128, 0, min_percentage, max_percentage));
        target_R = (float(controller_data.L_Y) / 128) * 255;
        current_L = target_L;
        current_R = target_R;
        pev_L = current_L;
        pev_R = current_R;
    }
    m(current_L, current_R);
    Serial.print("currentL: ");
    Serial.print(current_L);
    Serial.print("currentR: ");
    Serial.println(current_R);
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

#include <PS4Controller.h>

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
    Serial.print("LX:");
    Serial.print(controller_data.L_X);
    Serial.print(" LY:");
    Serial.print(controller_data.L_Y);
    Serial.print(" RX:");
    Serial.print(controller_data.R_X);
    Serial.print(" RY:");
    Serial.print(controller_data.R_Y);
    Serial.print(" L1:");
    Serial.print(controller_data.L1);
    Serial.print(" L2:");
    Serial.print(controller_data.L2);
    Serial.print(" L3:");
    Serial.print(controller_data.L3);
    Serial.print(" R1:");
    Serial.print(controller_data.R1);
    Serial.print(" R2:");
    Serial.print(controller_data.R2);
    Serial.print(" R3:");
    Serial.print(controller_data.R3);
    Serial.print(" Opt:");
    Serial.print(controller_data.Option);
    Serial.print(" Sha:");
    Serial.print(controller_data.Share);
    Serial.println();
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

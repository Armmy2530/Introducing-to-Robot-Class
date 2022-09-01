#include<motor_esp32.h>
#include <pwmWrite.h>
// Motor L 
int enA =27;
int in1 =26;
int in2 =25;
// Motor R 
int enB =13;
int in3 =14;
int in4 =12;

Motor R_motor(in3,in4,enB);
Motor L_motor(in1,in2,enA);

void setup(){
    Serial.begin(115200);
    Serial.println("Hello world");
    m(255,255);delay(500);m(0,0);delay(500);
    m(-255,-255);delay(500);m(0,0);delay(500);
    m(-255,255);delay(500);m(0,0);delay(500);
    m(255,-255);delay(500);m(0,0);delay(500);

}

void loop(){

}

void m(int l,int r){
    L_motor.m(l);
    R_motor.m(r);
}
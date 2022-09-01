#include"motor.cpp"
// Motor L
int enA = 10;
int in1 = 8;
int in2 = 9;
// Motor R 
int enB = 11;0
int in3 = 13;
int in4 = 12;

int turn_ms = 20;

Motor L_motor(in1,in2,enA);
Motor R_motor(in3,in4,enB);
Dual_motor Dual(L_motor,R_motor);


void setup(){
    Serial.begin(115200);
    Serial.println("Hello world");
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    delay(500);
}

void loop(){
    int L = digitalRead(5);
    int R = digitalRead(4);
    if(L == 1 && R == 1){m(100,90);}
    else if(L == 0 && R == 1){tl(90,90);delay(turn_ms);}
    else if(L == 1 && R == 0){tr(90,90);delay(turn_ms);}
    else{stop(false);}
}

void m(int l,int r){
    Dual.m(l,r);
}

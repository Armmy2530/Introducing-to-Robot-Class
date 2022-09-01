#define r1 14
#define r2 12
#define r  13
#define l1 26
#define l2 25
#define l  27

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int pwmChanne2 = 1;
const int resolution = 8;
int dutyCycle = 200;

void setup(){
    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin( r, pwmChannel);
    ledcAttachPin(l, pwmChanne2);
    pinMode(l1,OUTPUT);
    pinMode(l2,OUTPUT);
    pinMode(r1,OUTPUT);
    pinMode(r2,OUTPUT);
    Serial.begin(115200);
    Serial.println("Hello world");
    m(255,255);
}
void loop(){

}

void m(int L,int R){
    if(L>0){
        digitalWrite(l1,HIGH);digitalWrite(l2,LOW);ledcWrite(pwmChannel, L);
    }
    if(L<0){
        digitalWrite(l1,LOW);digitalWrite(l2,HIGH);ledcWrite(pwmChannel, L*-1);
    }
    if(L==0){
        digitalWrite(l1,HIGH);digitalWrite(l2,HIGH);ledcWrite(pwmChannel, 0);
    }
    if(R>0){
        digitalWrite(r1,HIGH);digitalWrite(r2,LOW);ledcWrite(pwmChanne2, R);
    }
    if(R<0){
        digitalWrite(r1,LOW);digitalWrite(r2,HIGH);ledcWrite(pwmChanne2, R*-1);
    }
    if(R==0){
        digitalWrite(r1,HIGH);digitalWrite(r2,HIGH);ledcWrite(pwmChanne2,0);
    }
}

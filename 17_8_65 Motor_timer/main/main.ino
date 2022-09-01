#define r1 14
#define r2 12
#define r  13
#define l1 25
#define l2 26
#define l  27

void setup(){
    for(int i=8;i<=13;i++){
        pinMode(i,OUTPUT);
    }
    Serial.begin(115200);
    Serial.println("Hello world");
    m(105,95);delay(1900);m(0,0);delay(500);
    m(90,0);delay(750);m(0,0);delay(500);
    m(105,95);delay(600);m(0,0);delay(500);
    m(90,0);delay(600);m(0,0);delay(500);
    m(105,95);delay(800);m(0,0);delay(500);
    m(90,0);delay(350);m(0,0);delay(500);
    m(105,95);delay(1350);m(0,0);delay(500);
}

void loop(){

}

void m(int L,int R){
    if(L>0){
        digitalWrite(l1,HIGH);digitalWrite(l2,LOW);analogWrite(l,L);
    }
    if(L<0){
        digitalWrite(l1,LOW);digitalWrite(l2,HIGH);analogWrite(l,L-1);
    }
    if(L==0){
        digitalWrite(l1,HIGH);digitalWrite(l2,HIGH);analogWrite(l,0);
    }
    if(R>0){
        digitalWrite(r1,HIGH);digitalWrite(r2,LOW);analogWrite(r,R);
    }
    if(R<0){
        digitalWrite(r1,LOW);digitalWrite(r2,HIGH);analogWrite(r,R-1);
    }
    if(R==0){
        digitalWrite(r1,HIGH);digitalWrite(r2,HIGH);analogWrite(r,0);
    }
}
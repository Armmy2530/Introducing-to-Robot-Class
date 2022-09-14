void stop(boolean hard){
    if(hard){ m(-999,-999);}else{ m(0,0);}
}
void tl(int speedL,int speedR){m(0,speedR);}
void tr(int speedL,int speedR){m(speedL,0);}
void sl(int speedL,int speedR){m(-speedL,speedR);}
void sr(int speedL,int speedR){m(speedL,-speedR);}
void fd(int speedL,int speedR){m(speedL,speedR);}
void bd(int speedL,int speedR){m(-speedL,-speedR);}
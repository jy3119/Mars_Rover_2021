void go(){
    digitalWrite(pwmr, HIGH);
    digitalWrite(pwml, HIGH); 
}
void forward(){
    digitalWrite(DIRR, HIGH);
    digitalWrite(DIRL, LOW);    
}
void turnaround(int rad){
  bool tdone = false;
  if (Dir_steerd>1){
    digitalWrite(DIRR, LOW);
    digitalWrite(DIRL, LOW);
    digitalWrite(pwmr, HIGH);
    digitalWrite(pwml, HIGH); 
    tdone = true;
  }
  if(Dir_steerd<-1){
    digitalWrite(DIRR, HIGH);
    digitalWrite(DIRL, HIGH);
    digitalWrite(pwmr, HIGH);
    digitalWrite(pwml, HIGH); 
    tdone = true;
  }
  int forward = 0;
  if (tdone == true && forward<rad){
    forward();
    go();
  forward = forward - distance_y;
    
  }

  
}
void comparedis(int x1,int y1,int x2,int y2){
  float dis = returndistance(x1,y1,x2,y2);
  if (dis<100){
    return true;
  }
}
void setup() {
  Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print(ballx);
  delay(1000);

}

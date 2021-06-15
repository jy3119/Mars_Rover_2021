int tdone = 1;
int ton = 6;

void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(tdone = 1 && ton >0){
    Serial.print(tdone);
    Serial.print('\n');
    ton = ton -1;
    delay(100);
  }
}

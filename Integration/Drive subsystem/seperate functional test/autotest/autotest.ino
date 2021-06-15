void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if (t_done == 0&&(target_a-Dir_steerd)  > 0) {
    digitalWrite(DIRR, HIGH);
    digitalWrite(DIRL, HIGH);
    digitalWrite(pwmr, HIGH);
    digitalWrite(pwml, HIGH); 
    if ((target_a - Dir_steerd) < 5) {             //stop 20 degree before
      digitalWrite(pwmr, LOW);
      digitalWrite(pwml, LOW);
      t_done = 1;
      delay(100);
    }
   }
  if (t_done == 0&&(target_a-Dir_steerd)  < 0) {
    digitalWrite(DIRR, LOW);
    digitalWrite(DIRL, LOW);
    digitalWrite(pwmr, HIGH);
    digitalWrite(pwml, HIGH); 
    if ((target_a - Dir_steerd) > -5) {             //stop 20 degree before
      digitalWrite(pwmr, LOW);
      digitalWrite(pwml, LOW);
      t_done = 1;
      delay(100);
    }
   }    
}

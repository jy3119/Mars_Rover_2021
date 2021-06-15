float Pi = 3.1416;

double returndeg(int xtarget,int ytarget,int xcurrent,int ycurrent,double degcurrent) 
{
  int xdiff = xtarget - xcurrent;
  int ydiff = ytarget - ycurrent;
  double diffratio = xdiff / ydiff;
  double degdiff = atan(diffratio)*180/Pi;
  double degtarget = degcurrent - degdiff;
  return degtarget;
  delay(1000);
}
float returndistance(int xtarget,int ytarget,int xcurrent,int ycurrent){
  int xdiff = xtarget - xcurrent;
  int ydiff = ytarget - ycurrent;
  int xsqr = pow(xdiff,2);
  int ysqr = pow(ydiff,2);
  return sqrt(ysqr+xsqr);
  
}

void setup() {
  // put your setup code here, to run once:

  double degaim = returndistance(3,4,0,0);
  Serial.begin(38400);
  //degaim = atan(1);

}

void loop() {
  // put your main code here, to run repeatedly:
  float degaim = returndistance(3,4,0,0);
  Serial.println(degaim);
}

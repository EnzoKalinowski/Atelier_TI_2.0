#include <Servo.h>

Servo servoH;
Servo servoV;

int thetaH,thetaV;
int serialRead;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  
  servoV.attach(3);
  servoH.attach(2);
  

  thetaH=90;
  thetaV=90;
  servoH.write(thetaH);
  servoV.write(thetaV);
}

// the loop routine runs over and over again forever:
void loop() {
  serialRead=Serial.parseInt();
  thetaH=serialRead/1000;
  thetaV=serialRead-thetaH*1000;
  
  servoH.write(thetaH);
  servoV.write(thetaV);
  
  delay(10);
}

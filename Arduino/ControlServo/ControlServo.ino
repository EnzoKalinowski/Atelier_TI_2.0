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
  char buf[6];
  Serial.readBytes(buf, 7);
 
  String str(buf);
  String horizontal = str.substring(0, 3);
  String vertical = str.substring(3, 6);

  int h = horizontal.toInt();
  int v = vertical.toInt();
  Serial.print("H");
  Serial.println(h);
  Serial.print("V");
  Serial.println(v);

  if(h != 0){
    servoH.write(h);
  }
  if(v != 0){
    servoV.write(v);
  }
  
  delay(10);
}

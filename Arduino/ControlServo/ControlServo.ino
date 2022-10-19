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
  Serial.readBytes(buf, 6);
  std::string str(buf);
  std::string horizontal = str.substr(0, 3);
  std::string vertical = str.substr(3, 3);
  
  servoH.write(std::stoi(horizontal));
  servoV.write(std::stoi(vertical));
  
  delay(10);
}


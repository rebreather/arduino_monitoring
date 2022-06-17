#include "ESP32Servo.h"

#define SERVO_PIN 21
Servo myservo;


void setup() {
  // put your setup code here, to run once:

    myservo.setPeriodHertz(50); //서보모터의 PWM 주기를 지정
  myservo.attach(SERVO_PIN, 500, 2400); //핀 번호, 최소 펄스 폭, 최대 펄스 폭
  myservo.write(10); //주어진 각도의 위치로 서보모터 회전

}

void loop() {
  for (int i = 10; i<180; i+=5)
  {
    myservo.write(i);
    delay(200);
  }
  for (int j = 175; j>0; j-=5)
  {
    myservo.write(j);
    delay(200);
  }

}

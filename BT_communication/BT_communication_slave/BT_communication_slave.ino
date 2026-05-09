// [슬레이브 아두이노용 코드 - 신호 수신 및 LED 제어]
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  pinMode(13, OUTPUT);      // 내장 LED 핀을 출력으로 설정
  BTSerial.begin(9600);     // HC-06 기본 보드레이트
}

void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    
    if (data == '1') {
      digitalWrite(13, HIGH); // '1'을 받으면 LED 켬
    } 
    else if (data == '0') {
      digitalWrite(13, LOW);  // '0'을 받으면 LED 끔
    }
  }
}
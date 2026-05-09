// [마스터 아두이노용 코드 - PC에서 입력받아 무선 송신]
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  Serial.begin(9600);       // PC와 통신 속도
  BTSerial.begin(9600);     // 블루투스 통신 속도 (작동 모드에서는 9600)
  Serial.println("마스터 아두이노 준비 완료. '1' 또는 '0'을 입력하세요.");
}

void loop() {
  // PC의 시리얼 모니터에서 값을 입력하면 블루투스로 전송
  if (Serial.available()) {
    char toSend = Serial.read();
    BTSerial.write(toSend);
    
    // 무엇을 보냈는지 시리얼 모니터에 출력
    if(toSend != '\n' && toSend != '\r'){
      Serial.print("전송함: ");
      Serial.println(toSend);
    }
  }
}
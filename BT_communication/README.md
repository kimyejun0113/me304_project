# 아두이노 HC-05 / HC-06 블루투스 통신 세팅 가이드

단일 USB 케이블 환경에서 아두이노 2대와 블루투스 모듈(HC-05, HC-06)의 통신을 설정하고 테스트하는 전체 과정입니다.

## 📌 핵심 이해: 핀 연결 규칙
* **AT 설정 모드 (직결 연결):** 아두이노 칩을 기절시키고(RESET-GND 연결) 컴퓨터와 블루투스가 직접 통신. `RX-RX(0번 핀)`, `TX-TX(1번 핀)` 연결.
* **실제 작동 모드 (교차 연결):** 아두이노와 블루투스가 통신. `아두이노 RX(2번) - 블루투스 TX`, `아두이노 TX(3번) - 블루투스 RX` 교차 연결.

---

## 🟢 PHASE 1: 슬레이브(HC-06) 세팅

### 1-1. 주소 확인 (AT 모드)
1. **배선:** 5V-VCC, GND-GND, RX(0)-RXD, TX(<SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  pinMode1)-TXD, **RESET-GND (점퍼선 연결)**
2. 시리얼 모니터: `9600 보드레이트`,(13, OUTPUT);      // 내장 LED 핀을 출력으로 설정
  BTSerial.begin(9600);     // HC-06 기본 `No line ending`
3. 명령어 `AT` 입력 ➡️ `OK` 확인
4. 명령어 `AT+ADDR?` 입력 ➡️ **주소 메모 보드레이트
}

void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    
    if (예: 98D3:31:F43015)**

### 1-2. 작동 코드 업로드
1. **배선 (data == '1') {
      digitalWrite(13, HIGH); // '1'을 받으면 LED 켬
    } 
    else 변경:** **RESET-GND 선 제거**, D2-TXD, D3-RXD 연결
2. 코드 업로드 후 USB 분리 (배터리 if (data == '0') {
      digitalWrite(13, LOW);  // '0'을 받으면 LED 끔
    }
  }
 작동 대기)

}
```cpp
// [슬레이브용 코드 - 신호 수신 및 LED 제어]
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  pinMode(13, OUTPUT);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    if (data == '1') digitalWrite(13, HIGH);
    else if (data == '0') digitalWrite(13, LOW);
  }
}
```

---

## 🔵 PHASE 2: 마스터(HC-05) 세팅

### 2-1. 설정 및 페어링 (AT 모드)
1. **배선:** 1-1과 동일한 직결 방식 (RESET-GND 연결 포함)
2. **AT 모드 진입:** HC-05의 버튼을 누른 채 USB 연결 (LED 2초 간격 깜빡임 확인)
3. 시리얼 모니터: `38400 보드레이트`, `Both NL & CR`
4. 명령어 입력:
   * `AT` ➡️ OK 확인
   * `AT+ROLE=1` ➡️ 마스터 설정 (OK 확인)
   * `AT+CMODE=0` ➡️ 지정 주소 통```

---

## 🔵 PHASE 2: 마스터(HC-05) 세팅

### 2-1. 설정 및 페어링 (AT 모드)
1. **배선:** 1-1과 동일한 직결 방식 (RESET-GND 연결 포함)
2. **AT 모드 진입:** HC-05의 작은 버튼을 누른 상태에서 USB 연결 (LED 2초 간격 천천히 깜빡임 확인)
3. 시리얼 모니터: `38400 보드레이트`, `Both NL & CR`
4. 명령어 입력:
   * `AT` ➡️ OK 확인
   * `AT+ROLE=1` ➡️ 마스터 역할 설정 (OK 확인)
   * `AT+CMODE=0` ➡️ 특정 지정된 주소와만 통신 (OK 확인)
   * `AT+BIND=98D3,31,F43015` ➡️ **(1-1에서 메모한 주소의 콜론(:)을 콤마(,)로 바꿔서 입력)** (OK 확인)

### 2-2. 작동 코드 업로드
1. **배선 변경:** **RESET-GND 선 제거**, D2-TXD, D3-RXD 연결
2. 코드 업로드

```cpp
// [마스터 아두이노용 코드 - PC에서 입력받아 무선 송신]
#include 신 (OK 확인)
   * `AT+BIND=98D3,31,F43015` ➡️ **(메모한 주<SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  Serial.begin소의 ':'를 ','로 변경하여 입력)** (OK 확인)

### 2-2. 작동 코드 업로드
1. **배선 변경:** **RESET-(9600);       // PC와 통신 속도
  BTSerial.begin(9600);     // 블루투스 통신 속도 (GND 선 제거**, D2-TXD, D3-RXD 연결
2. 코드 업로드

작동 모드에서는 9600)
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
      Serial.println```cpp
// [마스터용 코드 - PC 입력 무선 송신]
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // RX: D2, TX: D3

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Ready. Type '1' or '0'.");
}

void loop() {
  if (Serial.available()) {
    char toSend = Serial.read();
    BTSerial.write(toSend);
    if(toSend != '\n' && toSend != '\r'){
      Serial.print("Sent: ");
      Serial.println(toSend);
    }
  }
}
```

---

## 🚀 PHASE 3: 최종 테스트
1. **마스터((toSend);
    }
  }
}
HC-05):** PC 연결 유지, 시리얼 모니터 오픈 (9600)
2. **슬레이브(HC-06):** 외부 전원(배터리) 연결
3. **연결 확인:** 두 모듈 LED 패턴 변경 (동시 2번 깜빡임 등)
4. **명령:** 마스터 시리얼 모니터에 `1` 또는 `0` 입력하여 슬레이브 LED 제어 확인
```

---

## 🚀 PHASE 3: 최종 무선 통신 테스트
1. **마스터(HC-05):** PC 연결 유지, 시리얼 모니터 오픈 (9600 보드레이트)
2. **슬레이브(HC-06):** 외부 전원(보조배터리 등) 연결
3. **연결 확인:** 두 모듈 LED 패턴 변경 (빠르게 깜빡이다가 동시 2번 깜빡임 등으로 변경됨)
4. **명령:** 마스터 시리얼 모니터에 `1` 또는 `0`을 입력하여 슬레이브의 주황색 LED(13번 핀) 제어 확인
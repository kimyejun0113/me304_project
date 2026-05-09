# 아두이노 블루투스 무선 통신 (HC-05 / HC-06)

아두이노 2대 + 블루투스 모듈 2개로 무선 LED 제어. USB 케이블 1개로 세팅 완료.

## 준비물

- Arduino UNO × 2
- HC-05 (마스터) × 1
- HC-06 (슬레이브) × 1
- USB 케이블 × 1
- 점퍼 와이어, 보조 배터리

## 핀 연결 규칙

**AT 설정 모드** — 아두이노 칩 비활성화(RESET↔GND), 컴퓨터↔블루투스 직접 통신

```
RX(0) ↔ RXD    TX(1) ↔ TXD    RESET ↔ GND
```

**작동 모드** — 아두이노↔블루투스 교차 연결

```
D2 ↔ TXD    D3 ↔ RXD
```

---

## PHASE 1: 슬레이브(HC-06)

USB를 슬레이브 아두이노에 연결.

### 1-1. 주소 확인 (AT 모드)

배선: `5V↔VCC`, `GND↔GND`, `RX(0)↔RXD`, `TX(1)↔TXD`, `RESET↔GND`

시리얼 모니터: **9600 / No line ending**

```
AT          → OK
AT+ADDR?    → 주소 메모 (예: 98D3:31:F43015)
```

### 1-2. 코드 업로드 (작동 모드)

RESET↔GND 제거 → 교차 연결(`D2↔TXD`, `D3↔RXD`)로 변경 후 업로드:

```cpp
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);

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

업로드 완료 → **USB 분리**, 옆에 보관.

---

## PHASE 2: 마스터(HC-05)

USB를 마스터 아두이노에 연결.

### 2-1. 마스터 설정 (AT 모드)

배선: `5V↔VCC`, `GND↔GND`, `RX(0)↔RXD`, `TX(1)↔TXD`, `RESET↔GND`

HC-05 **작은 버튼을 누른 채** USB 연결 → LED 2초 간격 깜빡임 확인

시리얼 모니터: **38400 / Both NL & CR**

```
AT                      → OK
AT+ROLE=1               → 마스터 설정
AT+CMODE=0              → 지정 주소만 통신
AT+BIND=98D3,31,F43015  → 슬레이브 주소 (콜론→콤마)
```

### 2-2. 코드 업로드 (작동 모드)

RESET↔GND 제거 → 교차 연결(`D2↔TXD`, `D3↔RXD`)로 변경 후 업로드:

```cpp
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Ready. Send '1' or '0'");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
    if (c != '\n' && c != '\r') {
      Serial.print("Sent: ");
      Serial.println(c);
    }
  }
}
```

---

## PHASE 3: 테스트

1. 마스터: USB 연결 유지, 시리얼 모니터(9600)
2. 슬레이브: 보조 배터리로 전원만 공급
3. 양쪽 LED 깜빡임 패턴 변화 → 페어링 완료
4. `1` 입력 → 슬레이브 LED ON / `0` 입력 → LED OFF

---

## 트러블슈팅

| 증상 | 확인 |
|------|------|
| AT 응답 없음 | RESET↔GND 연결 확인 |
| HC-05 AT 진입 실패 | 버튼 누른 채 전원 인가 |
| 페어링 안 됨 | AT+BIND 주소, 콜론→콤마 변환 확인 |
| 업로드 실패 | RESET↔GND 제거, 0·1번 핀 비움 |
| LED 무반응 | 양쪽 보드레이트 9600 확인 |
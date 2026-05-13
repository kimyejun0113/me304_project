/*
 * MASTER — PD 게인 2포텐 + 조이스틱 XY + Attack + I2C LCD (회로도 기준)
 *
 * 연결:
 *   배터리 (+) → VIN, (-) → GND (USB 전원 없이 동작 가능)
 *   HC-05 VCC→5V GND→GND  TXD→D2  RXD→D3
 *   Attack 버튼 한쪽→D4  다른쪽→GND (INPUT_PULLUP)
 *   가변저항 P(Kp): 한쪽→5V 반대→GND 가운데→A0
 *   가변저항 D(Kd): 한쪽→5V 반대→GND 가운데→A1
 *   조이스틱 +5V/GND, VRx→A2, VRy→A3
 *   LCD(I2C): GND, VDD→5V, SDA→A4, SCL→A5 (주소 0x27 또는 0x3F)
 *
 * 라이브러리: LiquidCrystal I2C (Frank de Brabander)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_ATTACK = 4;

unsigned long lastLcdMs = 0;

static void drawLcd(int kp, int kd, int atk, int vrx, int vry) {
  char row0[17];
  char row1[17];

  snprintf(row0, sizeof(row0), "P:%4d  D:%4d", kp, kd);
  snprintf(row1, sizeof(row1), "%s X%3d Y%3d", atk ? "ATK " : "--- ", vrx, vry);

  lcd.setCursor(0, 0);
  lcd.print(row0);
  lcd.setCursor(0, 1);
  lcd.print(row1);
}

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Master Ready"));

  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(PIN_ATTACK, INPUT_PULLUP);
}

void loop() {
  unsigned long now = millis();

  int kp  = analogRead(A0);
  int kd  = analogRead(A1);
  int vrx = constrain(map(analogRead(A2), 0, 1023, 0, 254), 0, 254);
  int vry = constrain(map(analogRead(A3), 0, 1023, 0, 254), 0, 254);
  int atk = (digitalRead(PIN_ATTACK) == LOW) ? 1 : 0;

  // 슬레이브 형식: vry,vrx,kp,ki,kd,atk — PD만 쓰므로 ki 자리는 항상 0
  char buf[40];
  snprintf(buf, sizeof(buf), "%d,%d,%d,0,%d,%d", vry, vrx, kp, kd, atk);

  btSerial.println(buf);
  Serial.println(buf);

  if (now - lastLcdMs >= 120) {
    lastLcdMs = now;
    drawLcd(kp, kd, atk, vrx, vry);
  }

  delay(50);
}

/*
 * SLAVE — BT 수신 후 L298N 2모터 구동 (LCD 없음, 회로도 기준)
 *
 * 연결:
 *   배터리 (+) → Arduino VIN + L298N 12V, (-) → 공통 GND (Arduino GND + L298N GND)
 *   HC-06 VCC→5V GND→GND  TXD→D2  RXD→D3
 *   L298N: ENA→D10, IN1→D12, IN2→D6 | ENB→D11, IN3→D9, IN4→D8
 *   좌측 모터 OUT1/OUT2, 우측 모터 OUT3/OUT4
 *
 * 프로토콜 (마스터와 동일): vry,vrx,kp,kd,atk
 */

#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);

const int ENA = 10;
const int IN1 = 12;
const int IN2 = 6;
const int ENB = 11;
const int IN3 = 9;
const int IN4 = 8;

unsigned long lastOkMs = 0;
const unsigned long LINK_TIMEOUT_MS = 400;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  btSerial.setTimeout(25);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  controlMotor(0, 0);
}

void loop() {
  unsigned long now = millis();

  if (now - lastOkMs >= LINK_TIMEOUT_MS) {
    controlMotor(0, 0);
  }

  if (!btSerial.available()) {
    return;
  }

  char line[48];
  size_t len = btSerial.readBytesUntil('\n', line, sizeof(line) - 1);
  line[len] = '\0';

  while (len > 0 && (line[len - 1] == '\r' || line[len - 1] == ' ')) {
    line[--len] = '\0';
  }

  int vry, vrx, kp, kd, atk;
  if (sscanf(line, "%d,%d,%d,%d,%d", &vry, &vrx, &kp, &kd, &atk) != 5) {
    Serial.print(F("parse skip: "));
    Serial.println(line);
    return;
  }

  vry = constrain(vry, 0, 254);
  vrx = constrain(vrx, 0, 254);
  kp = constrain(kp, 0, 1023);
  kd = constrain(kd, 0, 1023);
  atk = atk ? 1 : 0;

  lastOkMs = now;

  int throttle = map(vry, 0, 254, -255, 255);
  int steering = map(vrx, 0, 254, -255, 255);

  int rightSpeed = throttle - steering;
  int leftSpeed = throttle + steering;

  const int capNorm = 170;
  const int capAtk = 255;
  int cap = atk ? capAtk : capNorm;

  rightSpeed = constrain(rightSpeed, -cap, cap);
  leftSpeed = constrain(leftSpeed, -cap, cap);

  controlMotor(leftSpeed, rightSpeed);

  Serial.print(F("RX Y="));
  Serial.print(vry);
  Serial.print(F(" X="));
  Serial.print(vrx);
  Serial.print(F(" P="));
  Serial.print(kp);
  Serial.print(F(" D="));
  Serial.print(kd);
  Serial.print(F(" ATK="));
  Serial.print(atk);
  Serial.print(F(" L="));
  Serial.print(leftSpeed);
  Serial.print(F(" R="));
  Serial.println(rightSpeed);
}

// 좌측 = Motor A (ENA·IN1·IN2), 우측 = Motor B (ENB·IN3·IN4)
void controlMotor(int left, int right) {
  const int dead = 20;

  if (left > dead) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, abs(left));
  } else if (left < -dead) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(left));
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  if (right > dead) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, abs(right));
  } else if (right < -dead) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(right));
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}

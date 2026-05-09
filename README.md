# ME304: BattleBot Project

ME304 공학설계 2조 — 배틀로봇 프로젝트

---

## 1. 하드웨어

- **MCU**: Arduino Uno
- **센서**: MPU-6050 (6축 자이로 + 가속도계)
- **모터**: DC 모터
- **구조물**: 3D 프린팅

## 2. 소프트웨어

- **제어 알고리즘**: PID 제어

## 3. 사용 가이드

1. 기본 소스 코드: `Balancing_Robot_v1.ino`
2. `me304_arduino_libraries.zip` 파일을 아두이노 라이브러리 폴더에 압축 해제
   보통 아래 경로에 있음
   ```
   C:\Documents\Arduino\libraries\
   ```
3. 호환 보드 사용 시 CH340 드라이버 설치 필요
4. 블루투스 무선통신은 BT_communication README 참고

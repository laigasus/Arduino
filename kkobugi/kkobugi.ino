#include <stdio.h>
#include <stdlib.h>
/*--------------------온습도 관련--------------------*/
#include <DHT11.h>
DHT11 dht(8); // dht 11 선언부 핀번호는 보고 수정
float temp, humi;                       //온,습도
static char nowMode = 'O';
static float humidBefore = 0, humidAfter = 0;
void moving();
/*--------------------LCD 관련---------------------*/
#include <LiquidCrystal_I2C.h>//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD
static int backlightOn = 1;       //자동으로 디스플레이를 꺼지게 하도록 도움
static int timeToTurnOffBacklight = 0;
void DisplayNowMode(char text)    //현재 팬의 작동모드를 LCD에 표기함
{
  lcd.setCursor(15, 0);
  lcd.print(text);
}
void lcdModuleSetup() {
  lcd.begin();
  //lcd.noBacklight();
}
void autoBacklightOff()
{
  if (backlightOn == 1) //장치가 명령을 받을 때만 LCD 백라이트가 켜짐
  {
    timeToTurnOffBacklight++;

    if (timeToTurnOffBacklight > 4)
    {
      backlightOn = 0;
      lcd.noBacklight(); timeToTurnOffBacklight = 0;
      //장치를 제어하지 않으면 일정시간 후에 자동으로 LCD 백라이트가 어두워짐
    }
  }
  else
  {
    lcd.noBacklight();
  }
}
/*--------------------시간 관련--------------------*/
#include <DS1302.h>
DS1302 rtc(9, 10, 11);                   //시간 센서
#define TIME_MSG_LEN  11
#define TIME_HEADER  'T'
#define TIME_REQUEST  7


boolean timerSwitch = false;
unsigned long time;                     //비동기식 시간 조율. 기다려야하는 delay와 달리 동시적인 실행 가능

void clockModuleSetup() {
  time = millis();                      //비동기식 시간 조율. 프로그램 시작 전에 시간을 저장
  //rtc.halt(false);                      //시간 동작 여부 확인
  //rtc.writeProtect(false);               //덮어쓰기 방지로 시간 오류 제거

  //rtc.setTime(11, 59, 0);              //시간이 안맞을 경우 이것으로 조율을 할 수 있음(표준 24시)
  //rtc.setDate(5, 12, 2020);            //이녀석도 마찬가지(오른쪽 순서로 년, 월, 일 조율 가능)
  //put your setup code here, to run once:
}
void Clock()              //현재시간을 불러옴
{
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
}
/*-----------------가습기 관련----------------------*/
int waterPump = 4;
void waterModuleSetup() {
  pinMode(5, OUTPUT);
  pinMode(waterPump, INPUT_PULLUP);
}
/*------------------블루투스 관련-------------------*/
#include <Wire.h>
#include <SoftwareSerial.h>
int bluetoothTx = 2;
int bluetoothRx = 3;
static String cmd = "";

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
void bluetoothModuleSetup() {
  bluetooth.begin(9600);
  bluetooth.setTimeout(10);
}

static boolean isMenu = true;

void Bluetooth() {
  String tmp;
  if (bluetooth.available()) {
    if (isMenu) { // 실행 모드를 입력받는 IF문입니다. 1회만 기능하도록 1회 수행 후 isMenu를 False값으로 변경했습니다.
      tmp = bluetooth.readString(); // 실행 모드를 결정할 문자열을 입력받습니다 예) H / C
      if (tmp[0] == 'H' || tmp[0] == 'C') {
        nowMode = tmp[0]; // 조작 모드를 변경합니다.
        isMenu = false;

      } else {
        Serial.print("Wrong Command!: ");
        Serial.println(tmp);
      }
    } else { // 조작 모드를 입력 받은 이후 문자열은 커멘드로 입력받아집니다.
      cmd = bluetooth.readString();
      doProcess(nowMode, cmd); // 현재 조작 모드와 커멘드를 넘깁니다.
    }
  }
  cmd = "";// 명령이 끝난 이후 커멘드 문자열을 초기화하여 다음 커멘드를 입력 가능하도록 합니다.
  tmp = "";
}

void doProcess(char nowMode, String cmd) { // 입력받은 조작 모드로 분기를 나눈 이후 입력받은 커멘드로 명령을 실행합니다.
  if (nowMode == 'H') {// H 를 받으면 수동 조작 모드로 넘어갑니다
    handle(cmd);
  } else if (nowMode == 'C') { // C 를 받으면 좌표 이동 조작 모드로 넘어갑니다
    coordinate(cmd);
  } else {
    Serial.println("Wrong Menu!!");
    Serial.println(nowMode);
  }
}

/*-----------------초음파 거리 관련------------------*/
int ultrasonicTrig = 6;
int ultrasonicEcho = 7;
void ultraSonicModuleSetup() {
  pinMode(ultrasonicTrig, OUTPUT);
  pinMode(ultrasonicEcho, INPUT);
}
/*-----------------바퀴 이동 함수-------------------*/
#include <Servo.h>
Servo servoLeft;
Servo servoRight;


void maneuver(int speedLeft, int speedRight, int msTime)
{
  servoLeft.attach(12);
  servoRight.attach(13);
  // -200 -100......0......100 200
  servoLeft.writeMicroseconds(1500 + speedLeft); //왼쪽 바퀴 속도
  servoRight.writeMicroseconds(1500 - speedRight); //오른쪽 바퀴 속도
  delay(msTime); // 이동거리 15cm, 1000ms
  servoLeft.detach();
  servoRight.detach();
}
/*---------------------온 습도 관련------------------*/
void DTHStatusLCD() { // LCD 패널 설정입니다.
  lcd.setCursor(0, 1);
  lcd.print("T ");
  lcd.print(temp);    //온도 출력(C)
  lcd.print(" ");

  lcd.setCursor(8, 1);
  lcd.print("H ");
  lcd.print(humi);    //습도 출력(%)
  lcd.print("%");
}
float DHT()       //온습도 센서 설정입니다.
{
  int err;

  if ((err = dht.read(humi, temp)) == 0)
  {

  }
  else
  {
    Serial.print("ERROR NO.: ");
    Serial.print(err);
  }
  return humi;
}

void DHTDifference() {
  if (humidBefore == 0) {
    humidBefore = DHT();

  } else if (humidAfter == 0) {
    humidAfter = DHT();
  } else {
    if (humidAfter - humidBefore > 5) {
      Serial.print("물 뿌리는 중~ ^오^: ");
      Serial.println(humidAfter - humidBefore);
    } else {
      Serial.print("물이 없어용 ㅜ.ㅜ!: ");
      Serial.println(humidAfter - humidBefore);
    }
    humidAfter = 0;
    humidBefore = 0;
  }
}

/*---------------------기 타-----------------------*/

void serialModuleSetup() {
  Serial.begin(9600);
}

#include "FindPathAlgorithm.h" // 최단 경로를 계산해주는 알고리즘입니다. 헤더 파일로 추가했습니다.
#include "moving.h" // 좌표 주행 함수입니다.
void handle(String cmd) { // 메뉴얼 조작 합수입니다. 블루투스 조작 모드 선택 이후 사용이 가능합니다.
  if (cmd.equals("201")) {
    Serial.println("전진\n");
    maneuver(200, 200, 905);
  }
  else if (cmd.equals("202")) {
    Serial.println("좌회전\n");
    maneuver(-200, 200, 85);
  }
  else if (cmd.equals("203")) {
    Serial.println("우회전\n");
    maneuver(200, -200, 85);
  }
  else if (cmd.equals("204")) {
    Serial.println("후진\n");
    maneuver(-200, -200, 905);
  }
  else if (cmd.equals("205")) {
    Serial.println("정지\n");
    maneuver(0, 0, 0);
  }
  else if (cmd.equals("206")) {
    Serial.println("물 분사\n");
    digitalWrite(waterPump, HIGH);
  }
  else if (cmd.equals("207")) {
    Serial.println("물 정지\n");
    digitalWrite(waterPump, LOW);
  }
  else if (cmd.equals("208")) {
    Serial.println("우회전\n");
    maneuver(200, -200, 300);
    maneuver(200, 200, 1075);
  }
  else if (cmd.equals("exit")) {
    nowMode = 'O';
    isMenu = true;
    cmd = "";
  }
  else {
    Serial.println("없는 명령어");
  }
}

boolean isMan = false;
long distance;
int spinningTime = 4000;
void sparkling(long distance) { // 가습기 작동 관련 함수입니다. 초음파 센서로 사람 감지 유무에따라 작동합니다.
AGAIN:
  servoLeft.attach(12);
  servoRight.attach(13);
  servoLeft.writeMicroseconds(1560);
  servoRight.writeMicroseconds(1560);
  if (distance < 80) {
    //주변에 사람 또는 사람이 인접해 있으면 물을 뿌리지 않습니다
    isMan = true;
    Serial.println("범위내 사람 감지");
    delay(spinningTime);

  }//멈추고 2초 대기
  if (isMan == true) {
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1500);
    servoLeft.detach();
    servoRight.detach();

    Serial.println("대기중");
    isMan == false;
    distance = 81;
    goto AGAIN;
  } else {
    digitalWrite(waterPump, HIGH);
    servoLeft.writeMicroseconds(1440);
    servoRight.writeMicroseconds(1440);
    Serial.println("물뿌림");
    delay(8000);
    digitalWrite(waterPump, LOW);
    servoLeft.detach();
    servoRight.detach();
  }
}

long detect() {  // 초음파 거리 측정 함수입니다. mm단위로 측정합니다.
  long duration, distance;
  digitalWrite(ultrasonicTrig, LOW);
  digitalWrite(ultrasonicEcho, LOW);
  delay(2);
  digitalWrite(ultrasonicTrig, HIGH);
  delay(10);
  digitalWrite(ultrasonicTrig, LOW);
  duration = pulseIn(ultrasonicEcho, HIGH);
  distance = (duration * 340) / 1000 / 2;
  return distance;
}

void coordinate(String cmd) { // 좌표 주행 함수입니다.
  int startPoint[2];
  int endPoint[2];
  int obsNum;
  int cnt = 0;
  char ch;
  char *ptr = &ch;

  switch (cmd[0])
  {
    case 'S':
      { // 시작점을 입력받는 분기문입니다. 예) S{1,1}
        Serial.println("시작 좌표\n");
        for (int i = 0; i < cmd.length(); i++) {
          ch = cmd[i];
          if (atoi(ptr) != 0) {
            startPoint[cnt] = atoi(ptr); // 블루투스 신호로 받은 문자열 좌표를 atoi를 통해 숫자열로 변환 후 배열에 대입합니다.
            cnt++;
          }
        }//시작 좌표를  startPoint에 저장함
        cnt = 0;
        Serial.print("Start point");
        Serial.print(startPoint[0]);
        Serial.print(",");
        Serial.println(startPoint[1]);
      }
      break;

    case 'E': // 도착점을 입력받는 분기문입니다. 예 E{4,4}
      {
        Serial.println("끝 좌표\n");
        for (int i = 0; i < cmd.length(); i++) {
          ch = cmd[i];
          if (atoi(ptr) != 0) {
            endPoint[cnt] = atoi(ptr);  // 시작점과 동일한 방식으로 변환합니다.
            cnt++;
          }
        }//목적지 좌표를  endPoint에 저장함
        cnt = 0;
        Serial.print("End point");
        Serial.print(endPoint[0]);
        Serial.print(",");
        Serial.println(endPoint[1]);

      }
      break;

    case 'X': // 장애물 개수와 좌표를 입력받는 분기문입니다. 예) X2{2,2}{3,3}
      {

        char ch2;
        char *ptr2 = &ch2;
        //장애물 생성법: X3{1,1}v{2,2}v{3,3}v

        ch2 = cmd[1]; // 블루투스로 넘겨받는 값 중 2번째 값은 장애물의 개수를 말합니다.

        obsNum = atoi(ptr2);
        int obstaclePoint[obsNum][2];

        int obsCnt = 0;
        for (int i = 3; i < cmd.length(); i++) {  // 장애물 개수만큼 2차원 배열에 장애물 좌표를 대입합니다.
          ch2 = cmd[i];
          if (atoi(ptr2) != 0) {
            obstaclePoint[obsCnt][cnt] = atoi(ptr2);
            Serial.print("obstaclePoint:");
            Serial.println(obstaclePoint[obsCnt][cnt]);

            cnt++;
          }
          if (cnt == 2) {
            cnt = 0;
            obsCnt++;
          }
        }// findPath는 대입받은 출발 지점과 도착지점으로 최단 경로를 구해주는 알고리즘 소스입니다.

        findPath(startPoint[0], startPoint[1], endPoint[0], endPoint[1] , obstaclePoint, obsNum);// 시작과 끝 좌표, 장애물 좌표와 개수 인자값을 넘깁니다.
        moving(); // 좌표 주행을 구현한 함수입니다.
        sparkling(distance);// 가습기 작동 함수입니다.
        nowMode = 'O';// 블루투스 연결 설정을 다시 가능하기 위해 현재 모드를 초기화 하고 메뉴로 돌아가게 합니다.
        isMenu = true;
        cmd = "";
      }
      break;
  }
}



/*------------------------------------------------*/


/*ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ*/
void setup() {

  waterModuleSetup(); // 가습기 모듈 핀 설정입니다.
  ultraSonicModuleSetup(); // 초음파 센서 핀 설정입니다
  digitalWrite(waterPump, LOW); // 가습기 모듈이 처음에 작동을 멈춘 상태로 시작합니다
  serialModuleSetup(); // 시리얼 모듈 설정입니다.
  clockModuleSetup(); // 시계 모듈 설정입니다
  lcdModuleSetup(); // lcd 모듈 설정입니다
  DTHStatusLCD(); // 온습도 센서 설정입니다
  bluetoothModuleSetup(); // 블루투스 모듈 설정입니다
  servoLeft.detach(); // 블루투스 명령을 받은 후 모터가 작동 가능하도록 detach로 처음 설정했습니다.
  servoRight.detach();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long time2 = millis();
  Clock();
  DisplayNowMode(nowMode);
  DTHStatusLCD();
  distance = detect();
  //DHTDifference();
  if (time2 - time >= 2000)           //비동기식 시간 제어. 2초 간격으로 명령 수행
  {
    DHT();
    time = time2;                     //시간제어 반복하기 위해 사용
  }
  Bluetooth();
  //autoBacklightOff();
}

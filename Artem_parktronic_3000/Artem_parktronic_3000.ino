#include <iarduino_HC_SR04_int.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7,8); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб
// Назначаем пины

int Control[2]; 

const int leftDirForwardPin = 14;    //Цифровой пин левого мотора дающий команду "Вперёд"
const int leftDirBackwardPin = 15;   //Цифровой пин левого мотора дающий команду "Назад" 
const int rightDirForwardPin = 16;   //Цифровой пин правого мотора дающий команду "Вперёд"
const int rightDirBackwardPin = 17;  //Цифровой пин правого мотора дающий команду "Назад" 

const int PWMLeftPin = 9;           //Шим выход левого потора
const int PWMRightPin = 10;          //Шим выход правого мотора

int leftMotorValue;                 // Значение стика левого мотора
int rightMotorValue;                // Значение стика правого мотора

bool isLeftSideMoveForward = true;  //Флаг, показывающий едет ли левый трак вперёд. (true - вперёд, false - назад)
bool isRightSideMoveForward = true; //Флаг, показывающий едет ли правый трак вперёд. (true - вперёд, false - назад)

unsigned int deadZoneWindow = 15;   //Размер мёртвой зоны в центре стика

int leftSpeed, rightSpeed;          //Значения скорости для ШИМ выходов

iarduino_HC_SR04_int sensor1(4,2);
//iarduino_HC_SR04_int sensor2(5,3);

int topSensor, botSensor;

void setup()
{
  Serial.begin(9600);         // Открываем сериал. Для монитора последовательного интерфейса
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  
  //Назначение выходных портов
  pinMode(leftDirForwardPin, OUTPUT);
  pinMode(leftDirBackwardPin, OUTPUT);
  pinMode(rightDirForwardPin, OUTPUT);
  pinMode(rightDirBackwardPin, OUTPUT);
  pinMode(PWMLeftPin, OUTPUT);
  pinMode(PWMRightPin, OUTPUT);
}

void loop()   
{
  byte pipeNo;
   while(radio.available(&pipeNo))
  {
    radio.read(&Control, sizeof(Control));
    Serial.print(Control[0]);
    Serial.print(" ");
    Serial.print(Control[1]);
    leftMotorValue = Control[0]; // читаем значение оси X
    rightMotorValue = Control[1]; // читаем значение оси Y
    
    topSensor = sensor1.distance();
    //botSensor = sensor2.distance();
    
    Serial.print(" ");
    Serial.print(topSensor);
    Serial.println(" ");
    //Serial.println(botSensor);
    if(Control[0] > 0 && Control[1] > 1)
    {
      // Управляем двигателем в зависимости от значения джостика
      // Управление левым мотором:
      if (leftMotorValue >= 1 && leftMotorValue <= (512 - deadZoneWindow) && topSensor > 15)
      {
        //Если значение от 0 до (512 - мёртвая зона)едем назад
        leftSpeed = map(leftMotorValue, (512 - deadZoneWindow), 1, 0, 255);
        isLeftSideMoveForward = false;
      }
      else if (leftMotorValue >= (512 + deadZoneWindow)&& leftMotorValue <= 1023)
      {
        //Если значение от (512 + мёртвая зона) едем вперёд
        leftSpeed = map(leftMotorValue, (512 + deadZoneWindow), 1024, 0, 255);
        isLeftSideMoveForward = true;
      }
      else
      {
        //Значение за допустимыми пределами либо в мёртвой зоне. останавливаемся
        leftSpeed = 0;
      }
    
      
      // Управление правым мотором:
      if (rightMotorValue >= 1 && rightMotorValue <= (512 - deadZoneWindow) && topSensor > 15)
       {
        //Если значение от 0 до (512 - мёртвая зона)едем назад
        rightSpeed = map(rightMotorValue, (512 - deadZoneWindow), 1, 0, 255);
        isRightSideMoveForward = false;
      }
      else if (rightMotorValue >= (512 + deadZoneWindow)&& rightMotorValue <= 1023)
      {
        //Если значение от (512 + мёртвая зона) едем вперёд
        rightSpeed = map(rightMotorValue, (512 + deadZoneWindow), 1024, 0, 255);
        isRightSideMoveForward = true;
      }
      else
      {
        //Значение за допустимыми пределами либо в мёртвой зоне. останавливаемся
        rightSpeed = 0;
      }
      //Подача управляющих сигналов на драйвер
      SetTrackSpeed(leftSpeed, true);
      SetTrackSpeed(rightSpeed, false);
    }
  }
}



void SetTrackSpeed(int trackSpeed, bool isLeftTrack)
{
  if(isLeftTrack)
  {   //Управление для левого трака
    if(trackSpeed == 0)
    {
      StopLeftTrack();
    }
    else
    { //Если трак движится вперёд
      if(isLeftSideMoveForward)
      {
        digitalWrite(leftDirForwardPin, HIGH);
        digitalWrite(leftDirBackwardPin, LOW);
        analogWrite(PWMLeftPin, trackSpeed);
      }
      else //Если трак движится назад
      {
        digitalWrite(leftDirForwardPin, LOW);
        digitalWrite(leftDirBackwardPin, HIGH);
        analogWrite(PWMLeftPin, trackSpeed);
      }
    }
  }
  else //Управление для правого трака
  {
    if(trackSpeed == 0)
    {
      StopRightTrack();
    }
    else
    { //Если трак движится вперёд
      if(isRightSideMoveForward)
      {
        digitalWrite(rightDirForwardPin, HIGH);
        digitalWrite(rightDirBackwardPin, LOW);
        analogWrite(PWMRightPin, trackSpeed);
      }
      else //Если трак движится назад
      {
        digitalWrite(rightDirForwardPin, LOW);
        digitalWrite(rightDirBackwardPin, HIGH);
        analogWrite(PWMRightPin, trackSpeed);
      }
    }
  }
}

void StopLeftTrack()
{
  analogWrite(PWMLeftPin, 0);
  digitalWrite(leftDirForwardPin, LOW);
  digitalWrite(leftDirBackwardPin, LOW);
}

void StopRightTrack()
{
  analogWrite(PWMRightPin, 0);
  digitalWrite(rightDirForwardPin, LOW);
  digitalWrite(rightDirBackwardPin, LOW);
}

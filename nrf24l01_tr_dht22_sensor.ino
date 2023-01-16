// для сенсоров
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include "stDHT.h" // библиотека для DHT22
// для радиомодуля
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

// для сенсоров
//#define DHTPIN 2
//#define DHTTYPE DHT22

Adafruit_BMP085 bmp; // инициализация датчика
DHT sens(DHT22); // Указать датчик DHT11, DHT21, DHT22

// LM35 аналоговый датчик температуры
int raw = 0;

float t1 = 0;
float t2 = 0;
float h = 0;
float p = 0;

long data[4]; // массив для значений

// для радиомодуля
RF24 radio(8, 7);    //(CE, CSN)

void setup(){
  Serial.begin(9600);
  // для радиомодуля
  radio.begin(); //активировать модуль
  delay(1000);
  radio.setChannel(120);
  radio.openWritingPipe(0xAABBCCDD11LL); 
  radio.setPALevel (RF24_PA_HIGH); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  radio.powerUp();
  // для сенсоров
  pinMode( A0, INPUT); // LM35
  pinMode(2, INPUT); // DHT22
  digitalWrite(2, HIGH);
  bmp.begin(); // BMP180 pressure
  delay(1000);
}
void loop (){
  raw = analogRead(A0);
  t1 = (raw/1023.0)*5.0*1000/10;
  t2 = sens.readTemperature(2); // чтение датчика на пине 2
  h = sens.readHumidity(2);    // чтение датчика на пине 2
  p = bmp.readPressure();
  
  // сбор данных в массив
  data[0] = t1;
  data[1] = t2;
  data[2] = h;
  data[3] = p;

  // отправка массива на передатчик
  if(radio.write(&data, sizeof(data))){
    digitalWrite(8, LOW);
  } else digitalWrite(8, HIGH);

  // вывод массива в консоль
  for(byte x = 0; x < 4; x++){
      Serial.print(data[x]);
      Serial.println();
  }
  delay(2000);
}

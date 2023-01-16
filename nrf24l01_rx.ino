#include <nRF24L01.h>
#include <RF24.h>

#include <SPI.h>

RF24 radio(8, 7);    //(CE, CSN)
//String data;
long data[4];
void setup()
{
  Serial.begin(9600);
  radio.begin(); //активировать модуль
  delay(1000);
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток) в ответ на входящий сигнал
  radio.setChannel(120);
  radio.openReadingPipe(1, 0xAABBCCDD11LL); 
  radio.setPALevel (RF24_PA_HIGH); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  radio.powerUp();
  radio.startListening();
}

void loop() 
{
  if(radio.available()){
    radio.read(&data, sizeof(data)); // чтение данных и ввывод в порт
    
    for(int x = 0; x < 4; x++){
      Serial.print(data[x]);
      Serial.println();  
    }
    Serial.println();
    delay(2000);
  }
}

/*
Arduino RC Plane project: Transmitter
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define jlx 1
#define jly 2
#define jrx 3
#define jry 4
#define led 5

float Row, Yaw, Pitch, Thrust;

RF24 radio(10, 9);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";

struct Data_Package {
  byte Row;
  byte Yaw;
  byte Pitch;
  byte Thrust;
};

Data_Package data;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  pinMode(led, OUTPUT);

  data.Row = 127;
  data.Yaw = 127;
  data.Pitch = 127;
  data.Thrust = 0;

  digitalWrite(led, HIGH);
}

void loop() {
  data.Row = map(analogRead(jry), 0, 1023, 0, 255);
  data.Yaw = map(analogRead(jly), 0, 1023, 0, 255);
  data.Pitch = map(analogRead(jrx), 0, 1023, 0, 255);
  data.Thrust = map(analogRead(jlx), 0, 1023, -255, 255);
  if(data.Thrust < 0) {
    data.Thrust = 0;
  }
  
  radio.write(&data, sizeof(Data_Package));
}

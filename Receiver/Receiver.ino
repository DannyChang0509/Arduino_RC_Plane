/*
Arduino RC Plane project: Receiver
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

#define motor 4
#define led 5

Servo sr, sy, sp;
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
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  sr.attach(1);
  sy.attach(2);
  sp.attach(3);
  pinMode(motor, INPUT);
  pinMode(led, OUTPUT);

  resetData();

  digitalWrite(led, HIGH);
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis();
  }
  
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) {
    resetData();
  }

  sr.write(map(data.Row, 0, 255, 0, 180));
  sy.write(map(data.Yaw, 0, 255, 0, 180));
  sp.write(map(data.Pitch, 0, 255, 0, 180));
  analogWrite(motor, map(data.Thrust, 0, 255, 0, 1023));
}

void resetData() {
  data.Row = 127;
  data.Yaw = 127;
  data.Pitch = 127;
  data.Thrust = 0;
}

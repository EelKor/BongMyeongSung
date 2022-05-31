#include <SPI.h>
#include "RF24.h"

// 핀 선언
const int pinSwitch = 2;

int ignition = 1;

int msg = 0;
const byte address[6] = "Fire";
 
RF24 radio(7,8);  // CE, CSN
 
void setup(void) 
{
  radio.begin();
  Serial.begin(9600);

  pinMode(pinSwitch,INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
  
  radio.setPALevel(RF24_PA_MAX);    // MIN, LOW, HIGH, MAX
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15,15);
  radio.setCRCLength(RF24_CRC_16);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();
}
 
void loop(void) 
{
  ignition = digitalRead(pinSwitch);
  if(ignition == HIGH)
  {
    digitalWrite(13,HIGH);
    radio.write(&ignition, sizeof(ignition));
  }

  else
  {
    digitalWrite(13, LOW);
    radio.write(&ignition, sizeof(ignition));
  }
  delay(5);
}

//   NRF24L01_Test_RX.ino (2017.10.21)
//
//   TMRH20 Library - https://github.com/nRF24/RF24
//   How to - http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
//   Tutorial - http://sankios.imediabank.com/nrf24l01
// 
//   CE-7 / CSN-8 / MOSI-11 / MISO-12 / SCK-13
// 
 
#include <SPI.h>
#include "RF24.h"
 
float rxData[6] = {0,};
byte address[][6] = {"1Node"};
const byte startPoint = 0;
 
RF24 radio(7,8);  // CE, CSN
 
void setup(void) {
  Serial.begin(115200);
  radio.begin();
  /*
  radio.setPALevel(RF24_PA_MAX);    // MIN, LOW, HIGH, MAX
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15,15);
  radio.setCRCLength(RF24_CRC_16);
  radio.setChannel(108);
  */
  radio.setPALevel(RF24_PA_HIGH);    // MIN, LOW, HIGH, MAX
  radio.setChannel(110);
  radio.openReadingPipe(0, address[0]);
  radio.startListening();
}
 
void loop(void) 
{
  if(radio.available()) 
  {   
      radio.read(&rxData, sizeof(rxData));
      Serial.write(2); 
      Serial.print(rxData[0]);
      Serial.print(" ");
      Serial.print(rxData[1]);
      Serial.print(" ");
      Serial.print(rxData[2]);
      Serial.print(" ");
      Serial.print(rxData[3]);
      Serial.print(" ");
      Serial.print(rxData[4]);
      Serial.print(" ");
      Serial.println(rxData[5]);
  }
  
}
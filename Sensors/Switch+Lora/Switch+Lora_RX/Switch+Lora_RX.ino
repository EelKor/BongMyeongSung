
#include <SoftwareSerial.h>
 
#define RELAY 7

SoftwareSerial lora(2, 3);

int offCount;

void setup()
{
  Serial.begin(9600);
  pinMode(RELAY,OUTPUT);

  /* LORA 통신모듈 초기화 및 설정 */
    lora.begin(9600);
    delay(1000);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(1000);
    lora.println("AT+ADDRESS=77");
    delay(1000);
    lora.println("AT+NETWORKID=2");
    delay(1000);
}

void loop()
{ 
  String inString;

  while(lora.available())
  {
    if (lora.available()) {
      inString = lora.readStringUntil('\n');
      Serial.println(inString);
    }
  }
  // 수신패킷이 정상적이고 점화 명령이면.
  if(inString.length() > 15) // 1 은 아스키 코드로 49
  { 
    digitalWrite(RELAY, HIGH);
    Serial.println("IGNITION");
    offCount = 0;
  }
  else
  {
    offCount++;
    if(offCount > 50) digitalWrite(RELAY, LOW);
  }
  delay(10);
}

#include <SoftwareSerial.h>
 
#define RELAY 7

SoftwareSerial lora(2,3);

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY,OUTPUT);

  /* LORA 통신모듈 초기화 및 설정 */
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=77");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    //lora.println("AT+BAND=92000000");
}

void loop()
{ 
  String inString;

  while(lora.available())
  {
    if(lora.available())
    {
      inString = lora.readStringUntil("\n");
    }
  }
  Serial.println((int)inString[9]);
  // 수신패킷이 정상적이고 점화 명령이면.
  if(inString.length() == 19 && (int)inString[9]-48 == 1) // 1 은 아스키 코드로 49
  { 
    digitalWrite(RELAY, HIGH);
    Serial.println("IGNITION");
  }
  else
  {
    digitalWrite(RELAY,LOW);
    Serial.println("WAITING");
  }
}
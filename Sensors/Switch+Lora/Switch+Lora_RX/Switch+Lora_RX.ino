#include <SoftwareSerial.h>
 
SoftwareSerial lora(2,3);

void setup(){
    Serial.begin(115200);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=77");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    //lora.println("AT+BAND=92000000");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);
}

void loop()
{ 
  String inString;
  digitalWrite(LED_BUILTIN, LOW);
  while(lora.available())
  {
    if(lora.available())
    {
      inString += String(char(lora.read()));
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  if(inString.length() > 0)
  {
    Serial.print(inString);
  }
}
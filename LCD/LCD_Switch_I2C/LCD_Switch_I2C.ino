// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Press Button");
  
   // 스위치 핀 초기화
    pinMode(3, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
}

void loop()
{
  
  int swt1 = digitalRead(3);
    int swt2 = digitalRead(6);
    int swt3 = digitalRead(7);
    int swt4 = digitalRead(8);
  
    // 0 이 버튼 눌린거
    if(swt1 == 0)
    { 
      lcd.clear();
      lcd.print("5 $");

      
    }
  
    if(swt2 == 0)
    { 
      lcd.clear();
      lcd.print("10 $");
    }
  
    if(swt3 == 0)
    { 
      lcd.clear();
      lcd.print("15 $");
    }
  
    if(swt4 == 0)
    {
      lcd.clear();
      lcd.print("20 $");
    }
}

#include "HX711.h"

const float custom_scale_factor = 0.189;
const int DOUT = 2;
const int CLK = 3;

HX711 scale;

float calibration_factor = 20170.00;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT,CLK);
  delay(5000);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  unsigned long time = millis();
  Serial.write(2);
  Serial.print(time);
  Serial.print("\t");
  Serial.print(scale.get_units()*custom_scale_factor);
  Serial.print("\t");
  Serial.println(calibration_factor);

  if(Serial.available())
  {
    char key = Serial.read();
    if(key == '+')
      calibration_factor += 10;
    else if(key == '-')
      calibration_factor -= 10;
  }
}

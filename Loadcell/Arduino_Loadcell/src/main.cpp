#include <Arduino.h>
#include "HX711.h"

const int DOUT = 3;
const int CLK = 2;

HX711 scale;

float calibration_factor = 20170.00;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  // put your setup code here, to run once:Serial.begin(9600);
  scale.begin(DOUT,CLK);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale...");
  delay(5000);
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + to increase calibration factor");
  Serial.println("Press - to decrease calibration factor");

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
  // put your main code here, to run repeatedly:scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units()*0.453592);
  Serial.print(" kg");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char key = Serial.read();
    if(key == '+')
      calibration_factor += 10;
    else if(key == '-')
      calibration_factor -= 10;
  }
}
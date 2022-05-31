/* 5선식 Load cell 작동 코드 */

/* ============ 핀 레이아웃 ============ */
/* 
 * Loadcell  HX711          HX711   Arduino
 * 빨간색선 -> E+(VCC)        VCC   ->  5V
 * 검은색선 -> E-(GND)        DT    ->  D2
 * 흰색선   -> o+ (OUTPUT)   SCK   -> D3
 * 초록색선 -> o-            GND   -> GND
 * 
 * 노란색선은 EMI 노이즈 제거용, 사용 안해도 무방
 */

#include "HX711.h"

// HX711 - DOUT, DT (데이터선) 
// HX711 - CLK, SCK (클럭선) 
const int DOUT = 2;
const int CLK = 3;

HX711 scale;

float calibration_factor = 22000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  Serial.begin(9600);
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

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  Serial.print(scale.get_units());
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

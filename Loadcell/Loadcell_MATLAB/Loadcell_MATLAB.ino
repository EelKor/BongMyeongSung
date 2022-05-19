/* 5선식 Load cell 작동 코드 */

/* ============ 핀 레이아웃 ============ */
/* HX711 - Loadcell
 * E+(VCC) -> 빨간색선
 * E-(GND) -> 검은색선
 * O+(OUTPUT) -> 흰색선
 * O-  ->  초록색선
 * 
 * 노란색선은 EMI 노이즈 제거용, 사용 안해도 무방
 */

#include "HX711.h"

// HX711 - DOUT, DT (데이터선) 
// HX711 - CLK, SCK (클럭선) 
const int DOUT = 2;
const int CLK = 3;
unsigned long time;
HX711 scale;

float calibration_factor = 21000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT,CLK);
  delay(5000);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  time = millis();
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.write(2);
  Serial.print(time);
  Serial.print(" ");
  Serial.print(scale.get_units());
  Serial.println();
}

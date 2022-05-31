
/* MPU6050 + nR24L
 * 자이로 센서 데이터를 RF 모듈을 통해 무선전송

 * 작성자: 충남대학교 항공우주공학과 이승신 202004142
 * 작성일: 2020-05-17
*/


/* NRF24L01 핀 레이아웃
 * TMRH20 Library - https://github.com/nRF24/RF24
 * How to - http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo   
 * Tutorial - http://sankios.imediabank.com/nrf24l01
  
 * CE-D7 / CSN-D8 / MOSI-D11 / MISO-D12 / SCK-D13 / VCC-3.3V 
 * 아두이노 우노 기준
*/

/*
 * MPU6050 핀 레이아웃
 *  VCC -> 5V
 *  GND -> GND
 *  SCL -> A5
 *  SDA -> A4
 *  INT -> D2
 *  아두이노 UNO 기준
*/
 
/* ================ Include 부분 ==========================*/
#include <SPI.h>
#include "RF24.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


/* ======================= DEFINE 부분 ==================== */
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
#define OUTPUT_READABLE_YAWPITCHROLL
#define OUTPUT_READABLE_WORLDACCEL
/* ======================= MPU 변수선언 ==================== */
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

MPU6050 mpu;

/* ======================= NRF24 변수선언 ==================== */
float sendData[6] = {0,};
const byte address[6] = "1Node";
const byte STX = 2;
const byte ETX = 3;

RF24 radio(7,8);  // CE, CSN


/* ===================== MPU6050 함수 선언 =====================*/
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
 

void setup(void) {

  /* ==================== MPU6050 setup =======================================*/
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        //Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        //Serial.print(F("DMP Initialization failed (code "));
        //Serial.print(devStatus);
        //Serial.println(F(")"));
    }

  /* ==================== NRF24L01 setup =======================================*/
  radio.begin();
  
  radio.setPALevel(RF24_PA_MAX);    // MIN, LOW, HIGH, MAX
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15,15);
  radio.setCRCLength(RF24_CRC_16);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();

  /* ==================== 시리얼 통신 =======================================*/
  Serial.begin(115200);
}
 
void loop(void) {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
      // other program behavior stuff here
      // .
      // .
      // .
      // if you are really paranoid you can frequently test in between other
      // stuff to see if mpuInterrupt is true, and if so, "break;" from the
      // while() loop to immediately process the MPU data
      // .
      // .
      // .
  }
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      //Serial.println(F("FIFO overflow!"));

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;
      #ifdef OUTPUT_READABLE_QUATERNION
          // display quaternion values in easy matrix form: w x y z
          mpu.dmpGetQuaternion(&q, fifoBuffer);
          Serial.print("quat\t");
          Serial.print(q.w);
          Serial.print("\t");
          Serial.print(q.x);
          Serial.print("\t");
          Serial.print(q.y);
          Serial.print("\t");
          Serial.println(q.z);
      #endif
      #ifdef OUTPUT_READABLE_EULER
          // display Euler angles in degrees
          mpu.dmpGetQuaternion(&q, fifoBuffer);
          mpu.dmpGetEuler(euler, &q);
          Serial.print("euler\t");
          Serial.print(euler[0] * 180/M_PI);
          Serial.print("\t");
          Serial.print(euler[1] * 180/M_PI);
          Serial.print("\t");
          Serial.println(euler[2] * 180/M_PI);
      #endif
      #ifdef OUTPUT_READABLE_YAWPITCHROLL
          // display Euler angles in degrees
          mpu.dmpGetQuaternion(&q, fifoBuffer);
          mpu.dmpGetGravity(&gravity, &q);
          mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
          //Serial.write(2);
          //Serial.print(ypr[0] * 180/M_PI);
          //Serial.print("\t");
          //Serial.print(ypr[1] * 180/M_PI);
          //Serial.print("\t");
          //Serial.println(ypr[2] * 180/M_PI);
          
      #endif
      #ifdef OUTPUT_READABLE_REALACCEL
          // display real acceleration, adjusted to remove gravity
          mpu.dmpGetQuaternion(&q, fifoBuffer);
          mpu.dmpGetAccel(&aa, fifoBuffer);
          mpu.dmpGetGravity(&gravity, &q);
          mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
          //Serial.print("areal\t");
          //Serial.print(aaReal.x);
          //Serial.print("\t");
          //Serial.print(aaReal.y);
          //Serial.print("\t");
          //Serial.println(aaReal.z);
      #endif
      #ifdef OUTPUT_READABLE_WORLDACCEL
          // display initial world-frame acceleration, adjusted to remove gravity
          // and rotated based on known orientation from quaternion
          mpu.dmpGetQuaternion(&q, fifoBuffer);
          mpu.dmpGetAccel(&aa, fifoBuffer);
          mpu.dmpGetGravity(&gravity, &q);
          mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
          mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
          //Serial.print("aworld\t");
          //Serial.print(aaWorld.x);
          //Serial.print("\t");
          //Serial.print(aaWorld.y);
          //Serial.print("\t");
          //Serial.println(aaWorld.z);
      #endif
  }

  sendData[0] = ypr[0] * 180/M_PI;
  sendData[1] = ypr[1] * 180/M_PI;
  sendData[2] = ypr[2] * 180/M_PI;
  sendData[3] = aaWorld.x;
  sendData[4] = aaWorld.y;
  sendData[5] = aaWorld.z;
  //radio.write(&STX, 1);
  radio.write(&sendData, sizeof(sendData));
  //radio.write(&ETX, 1);
  delay(1);
}
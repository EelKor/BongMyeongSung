1. 프로그램 기능
 MPU-6050을 이용해 ROLL, PITCH, YAW 각도 및 X, Y, Z 가속도 데이터를 nRF240L 을 통해 PC로 전송
 
2. 개발환경
 Arduino Uno
 
3. PinMap and Wiring
<MPU-6050> - <Arduino Uno>
       VCC - 5V
       GND - GND
       SCL - A5
       SDA - A4
       INT - D2
       
 <nRF24L01> - <Arduino UNO>
         CE - D7
        CSN - D8
       MOSI - D11
       MISO - D12
        SCK - D13
        VCC - 3.3V
        
4. 소스파일
* MPU+nRF24/MPU+nR24.ino - MPU 데이터 처리 및 데이터 전송 부분
* MPU+nRF24_RX/MPU+nR24_RX.ino - 데이터 수신 및 PC로 전송 부분
* MPU+nRF24_RX/MPU+nR24_RX.m - 수신받은 데이터를 기반으로 MATLAB을 이용해 데이터 시각화

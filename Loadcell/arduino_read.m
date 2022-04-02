% bluetooth_Send.m
% 작 성 자: 기계융합공학과 이승신(2018013246)
% 작 성 일: 2021-04-28
% 설    명
% 사용자로 부터 속도, 조향값을 입력받아 프로토콜에 따라 데이터 전송
% 또한 미


clear all; close all;
clc
% 블루투스 리스트 표시
fprintf("******  Incoming Data from Arduino *****\n");
device = serialport('COM15', 115200)

while 1
    readdata = read(device,1,"uint8");

    if readdata == 2
        sensor = read(device,8,"uint8");
        temperture = sensor(1);
        distance = sensor(3);
        finedust = sensor(5);
        humid = sensor(7);
        
        fprintf("humid: %d, dust: %d temp: %d distance: %d\n" , humid, temperture, finedust, distance)
    else
        flush(device)
    end

end
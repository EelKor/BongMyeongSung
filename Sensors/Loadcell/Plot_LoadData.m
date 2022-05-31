close all;

%% 시리얼 통신 연결부분 
fprintf("******  Incoming Data from Arduino *****\n");
device = serialport('COM10', 9600)
configureTerminator(device, "CR/LF");



%% 시리얼 통신 부분
time = zeros(100);
weight = zeros(100);

figure(1)
grid on;
title("Thrust")
xlabel("i")
ylabel("Kgf")
 for i = 1:1000


    if read(device,1, 'uint8') == 2

        readdata = readline(device);
        loadcellData = split(readdata);
        Finaldata = str2double(loadcellData);
        time(i) = Finaldata(1);
        weight(i) = Finaldata(2);

        fprintf("Time: %d, Weight: %.2f\n", time(i), weight(i));
        hold on;
        plot(i,weight(i),'.','Color','red');
        drawnow;


    else
        % 시작바이트 2가 인식되지 않으면 버퍼 비우기
        flush(device)
    end
    
 end
 

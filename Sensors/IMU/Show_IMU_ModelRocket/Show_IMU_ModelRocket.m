clear; close all;

%% 시리얼 통신 연결부분 
fprintf("******  Incoming Data from Arduino *****\n");
device = serialport('COM14', 115200)
configureTerminator(device, "CR/LF");

%% STL 로켓 모델 불러오기
model = stlread("modelRocket.stl");

%% 그래프 설정 
fig = figure(1);
axis equal
xlabel('X');
ylabel('Y');
zlabel('Z');
view(3)
grid off;


%% 그래프 애니메이션 부분
 for i = 1:10000


    if read(device,1, 'uint8') == 2

        double_ypr = 0;
        for jj = 1:10
            readdata = readline(device);
            ypr = split(readdata);
            double_ypr = double_ypr + double(ypr);
        end

        double_ypr = double_ypr / 10;

        % 회전각도 설정
        turn_x_ang = double_ypr(3);
        turn_y_ang = -double_ypr(2);
        z_ang = 0;

        fprintf("%f.2, %f.2, %f.2\n", double_ypr(2), double_ypr(3), device.NumBytesAvailable)
    
        tic
        cla
        s = patch('vertices',model.vertices,'faces',model.faces,'Facecolor',[0.8 1 0.8]);
        rotate(s,[1 0 0], turn_x_ang)
        rotate(s, [0 1 0], turn_y_ang)
        rotate(s, [0 0 1], z_ang)


        drawnow;

    else
        % 시작바이트 2가 인식되지 않으면 버퍼 비우기
        flush(device)
    end
    
    % 연산시간 측정 및 애니메이션 균일화
    % 그래프 업데이트 주기를 균일화 하기 위해 0.03초 고정으로 그래프가 
    % 업데이트 되도록 pause() 추가
    %t = toc;
    %pausetime = 0.03-t;
    %if pausetime < 0
    %    pausetime = 0;
    %    pause(pausetime);
    %else
    %    pause(pausetime)
    %enda
    %pause(0.01)
end


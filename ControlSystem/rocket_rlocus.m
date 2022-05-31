clc;
clear;
close all;

servo_tf = tf([224.8],[1 22.33 225.4]);
plant_tf = tf([1],[1 2 4 0]);
pid_tf = tf([1 1 1],[1]);
add_zero = tf([1 10],[1]);


oltf = servo_tf * plant_tf * pid_tf * add_zero;                          

rlocus(oltf)

k = 100
cltf = feedback(oltf,1);
%step(k * cltf);

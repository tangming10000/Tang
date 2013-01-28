%显示速度场
close all;
clear;clc;
D = load('data_Vel.txt');
Length = length(D);
L=194;
W=8;
H=25;
Data = zeros(L,W,H);
for i = 1:H
    for j = 1:W
        for k = 1:L
            Idx = (i-1)*W*L + (j-1)*L +k;
            Data(k,j,i)= D(Idx);
        end
    end
end
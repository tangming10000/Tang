clear;clc;
net = newlin([-1 1;-1 1],1);
net.IW{1,1} = [1,2];
net.b{1} = 0;
P = [1 2 2 3; 2 1 3 1];
A = sim(net,P)
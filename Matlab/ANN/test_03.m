clear;clc;
net = newlin([-1 1],1,[0 1]);
net.biasConnect = 0;
net.IW{1,1}=[1 2];
P = {1,2,3,4};
A= sim(net,P)
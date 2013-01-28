clear;clc;
t=0.001:0.001:2;
load('filter_45_Lowpass.mat');
y=sin(2*pi*t)+sin(200*pi*t);
subplot(211);
plot(t,y);

[b,a]=tf(Hd);
z=filter(b,a,y);
subplot(2,1,2);
plot(t,z);
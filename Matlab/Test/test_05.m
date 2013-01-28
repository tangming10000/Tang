clear;clc;
x=0.0:0.01:2*pi;
y1=0.5*(pi-x);
subplot(2,1,1);
plot(x,y1);
subplot(2,1,2);
m = 20;
n = length(x);
y2 = y1;
for j = 1:n
   for i = 1:m
        y2(j) = y2(j) + sin(j*0.01*i)/i;
    end
end
plot(x,y2);
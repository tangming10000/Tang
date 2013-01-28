clear;clc;
%load the filter parameters Hd
load('filter_45_Highpass.mat');

%load the segy files
[D H] = ReadSegy('..\2.sgy');

%filter the segy data
[b,a]=tf(Hd);
[L N] = size(D);
Result=zeros(L,N);
for i = 1:N
    temp = D(:,i);
    Result(:,i) = filter(b,a,temp);
end

%show the result
figure(1);
subplot(1,2,1);
imshow(D);
subplot(1,2,2);
imshow(Result)
figure(2)
subplot(2,1,1);
plot(D(:,1));
subplot(2,1,2);
plot(Result(:,1));

% t=0.001:0.001:2;
% y=sin(2*pi*t)+sin(200*pi*t);
% subplot(211);
% plot(t,y);
% 
% [b,a]=tf(Hd);
% z=filter(b,a,y);
% subplot(2,1,2);
% plot(t,z);
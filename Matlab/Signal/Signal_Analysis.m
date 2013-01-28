clear;clc;

N = 200; % 窗大小
MAX =2000; %信号长度

x=1:MAX;
w=1:N;
R=zeros(MAX:N/2);
%初始化
w(:)=0;x(:)=0;


%原始信号
for t=0:MAX
    x(t+1) = sin(0.05*pi*t*t*0.001);
end
subplot(2,2,1);
t=0:0.001:MAX/1000.0;
plot(t,x);

%传统信号处理
subplot(2,2,2);
X=fft(x);
X=abs(X);
plot(X(1:MAX/2));

%矩形窗的短时窗傅里叶变换
subplot(2,2,3);
for i=1:MAX
    w(:)=0;
    if (i+N)<=MAX
        w=x(i:i+N);
    else
        w(1:MAX-i+1)=x(i:MAX);
    end
    w=fft(w);
    w=abs(w)*5.0;%乘以一个系数
    R(i,:)=w(1:N/2);
end
image(R');


%高斯加窗的STWFT
subplot(2,2,4);
for i=1:MAX
    w(:)=0;
    if (i+N)<=MAX
        w=x(i:i+N);
    else
        w(1:MAX-i+1)=x(i:MAX);
    end
    w=Gauss_window(w);
    w=fft(w);
    w=abs(w)*5.0;%乘以一个系数
    R(i,:)=w(1:N/2);
end
image(R');

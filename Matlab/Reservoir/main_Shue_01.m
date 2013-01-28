clear;clc;
w = zibof(35);
%H = 100;    %每层层厚
H = [500 300 200 300 200];
vp = [2.0 2.4 2.4 2.1 2.5 2.7];
vs = [1.2 1.2 1.4 1.1 1.5 1.6];
rho = [2.4 2.2 2.2 2.0 2.4 2.5];%泊松比
delta = zeros(size(vp));
for i = 1:6
    delta(i) = 0.5 * ( vp(i)^2 - 2*vs(i)^2) / ( vp(i)^2 - vs(i)^2);
end

x = 1:45;
for i = 1:5
    for j = 1:60
        zu(i,j)= Richard(vp(i),vs(i),rho(i),vp(i+1),vs(i+1),rho(i+1),j);
    end
end
figure(1);
zu = zu';
for i = 1:5    
    plot(zu);
    grid on;
    h = legend('1','2','3','4','5');
end
title('各个界面反射系数变化曲线');xlabel('道号');ylabel('反射系数')

for j = 1:60
    P = vp(1)/vp(2);
    th = sin( j * 0.25 / 180 *pi);
    temp =0;
    for i = 1:5       
        L = H(i)/ sqrt( 1 - th^2);
        temp = temp + 2*L/vp(i);
        t(i,j) = temp;
        th = th / P;
    end
end

Data = zeros(60,500);
for i = 1:60
    for j = 1:5
        temp = round(t(j,i));
        Data(i,temp)=zu(i,j);
    end
end
for i = 1:60
    ZZ(i,:) = conv( w,Data(i,:));
end
figure(2)
x=1:60;t=1:579;
ZZ=ZZ';
%wiggle(x,t,ZZ,'VA');
title('各个界面反射系数变化曲线');xlabel('入射角');ylabel('时间')


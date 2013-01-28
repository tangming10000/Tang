clear;clc;
wave=10*zibof(60);          %涓婚涓?0Hz鐨勫瓙娉?


ds1 = 2.24; v1 =2.25;   %涓婅宀╁眰鐨勯?搴﹀拰瀵嗗害閫熷害(Km/s)鍜屽瘑搴?g/cm^3)
ds3 = 2.24; v3 = 2.25;  %涓嬭宀╁眰鐨勯?搴?Km/s)鍜屽瘑搴?g/cm^3)
Max = 110;
%缁欏嚭浣撶Н妯￠噺Kg = 0.00013
 Km = 36.6; Kg = 0.13;Kw = 2.25;Ko=1.02;
 %Km 鐮傚博鐨勪綋绉ā閲忥紝 Kg 姘旂殑浣撶Н妯￠噺锛?Kw 姘寸殑浣撶Н妯￠噺锛孠o娌圭殑浣撶Н妯￠噺
 
PP = 0.2;   %瀛旈殭搴?

Sds = 2.640;Gds = 0.10;Ods = 0.800; Wds = 1.000; %瀵嗗害(g/cm^3)

Vp = 3.100;Vs=1.517;                %鐮傚博閫熷害鍜屽瘑搴?
Kd = Sds*( Vp^2 - 4/3*Vs^2);    %宀╃煶楠ㄦ灦浣撶Н妯￠噺Kd
U = Sds*Vs^2;                           %宀╃煶楠ㄦ灦鍓垏妯￠噺Ud

real Kf;
Kf = 1:Max;
Ds = zeros(size(Kf));
K = zeros(size(Kf));
V = zeros(size(Kf));
T = zeros(size(Kf));
Zu1 = zeros(size(Kf));
Zu2 = zeros(size(Kf));
%瀹氫箟瀛樺偍绌洪棿

for i = 1: 21    
        pp = (i-1) *0.05;        
        Kf(i) = (Kw * Ko)/( Ko*(1 - pp) + Kw*pp);
        Ds(i) = Sds*(1 - PP)  + PP * (Ods*pp + Wds*(1-pp) );
end
for i = 22:31
         pp = (i - 21 )  *0.005;         
         Kf(i) = ( Kg *Ko) / ( Kg*(1-pp) + Ko *pp);
         Ds(i) = Sds*(1 - PP)  + PP * (Gds*pp + Ods*(1-pp) );
end
for i = 32:50
         pp = (i - 31 )  *0.05 + 0.05;       
         Kf(i) = ( Kg *Ko) / ( Kg*(1-pp) + Ko *pp);
         Ds(i) = Sds*(1 - PP)  + PP * (Gds*pp + Ods*(1-pp) );
end
for i = 51:71
         pp = ( i - 51) *0.0025;
         Kf(i) = (Kw * Kg) / ( ( 1.0 - pp)*Kw + pp*Kg );
         Ds(i) = Sds*(1 - PP)  + PP * (Wds*pp + Gds*(1-pp) );
end
     
for i = 72:81
         pp = ( i - 71) * 0.095+ 0.05;
         Kf(i) = (Kw * Kg) / ( ( 1.0 - pp)*Kw + pp*Kg );
         Ds(i) = Sds*(1 - PP)  + PP * (Wds*pp + Gds*(1-pp) );
end
for i = 82:91
         pp = ( i - 81 ) *0.005;         
         Kf(i) = (1 - pp)/Kw + 0.5*pp/Kg + 0.5*pp/Ko;
         Kf(i) = 1.0/Kf(i);
         Ds(i) = Sds*(1 - PP)  + PP * (0.5*(Ods+Gds)*pp  + (1 - pp) * Wds);
end
for i = 92:110
         pp = ( i - 91 ) *0.05 + 0.05;         
         Kf(i) = (1 - pp)/Kw + 0.5*pp/Kg + 0.5*pp/Ko;
         Kf(i) = 1.0/Kf(i);
         Ds(i) = Sds*(1 - PP)  + PP * (0.5*(Ods+Gds)*pp + (1 - pp) * Wds);
end

for i = 1:Max
    K(i) = Gassman(Kd,Km,Kf(i),PP);                                      %璁＄畻楗卞拰宀╃煶浣撶Н妯￠噺
    V(i) = sqrt( (K(i)+ 4.0*U/3.0 ) / Ds(i));                               %璁＄畻绾垫尝閫熷害Vp
    T(i) = 100.0/v1/1000+ 150.0/V(i)/1000;                           %璁＄畻鏃堕棿锛坢s锛?
    Zu1(i) = (V(i)*Ds(i) - v1*ds1) / (V(i) *Ds(i) + v1*ds1);        %璁＄畻椤剁晫闈㈢殑娉㈤樆鎶楃郴鏁癦u1
    Zu2(i) = ( v3*ds3 - V(i) * Ds(i)) / ( v3*ds3 + V(i) * Ds(i));   %璁＄畻搴曠晫闈㈢殑娉㈤樆鎶楃郴鏁癦u2
end

Pu = zeros(150,Max);
t1 = round(100.0 / v1 );
for i = 1:Max
    Pu(t1,i) = Zu1(i);
    position = round(T(i) * 1000 );
    Pu(position,i) = Zu2(i);
end
for i = 1: Max
    Pus(:,i) = conv(Pu(:,i),wave);
end

figure(1);
plot(Zu1);
title('顶界面反射系数 ');
xlabel('偏移距');ylabel('反射系数');grid on;
figure(2);
plot(Zu2);
title('底界面反射系数 ');
xlabel('偏移距');ylabel('反射系数');grid on;
figure(3);
plot(V);
title('层速度变化情况');
xlabel('偏移距');ylabel('速度(Km/s)');grid on;
figure(4);
plot(T*1000);
title('旅行时');
xlabel('偏移距');ylabel('时间(ms)');grid on;
figure(5);
[Length Wide]=size(Pus);
x=1:Wide;t=1:Length;
wiggle(x,t,Pus,'VA');
grid on;
title('合成地震剖面');ylabel('时间(ms/t)');xlabel('偏移距');




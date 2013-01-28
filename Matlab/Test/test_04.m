clear;clc;
x = -pi:0.1:pi;
o = sin(x);
[N L] = size(x) ;
NW = 5;

for i = 1:NW
    w(i) = rand();
end

ee = 1e-6;
u0 = 0.2;
t = 200;
k = 1;
E = 1.0 + ee;
Ep = 0;
while ( abs(E) > ee )
    E = 0;
    xx = 0;
    yy = 0;
    for i = 1:L
        xx = x(i);
        yy = 0;
        for j = 1:NW
            yy = yy + xx*w(i);
        end
    end
    
    E = E + Ep;
    
end
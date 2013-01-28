clear;clc;
x = zeros(1000,3);
b = [ 1; 0.8; -1 ];

for i = 1:1000
    for j = 1:3
        temp = rand() * 3 - 1.5;
        x(i,j) = temp;
    end 
end

w = [-0.3043 -0.8195 0.3855];

u0 = 0.2;
t = 200;
k = 1;
e = 1.0;

while ( abs(e) > 1e-8)
    u = u0 / ( 1 + k / t);
    subplot(2,1,1);stem(k,u,'+');hold on;
    d = x(k,:) * b;
    sum = 0;
    for h = 1:3
        sum = sum + x(k,h) * w(h);
    end
    e = d - sum;
    subplot(2,1,2);stem(k,e,'*');hold on;
    for h = 1 :3
        w(h) = w(h) + u * e * x(k,h);
    end
    %fprintf('%d: %8.6f  %8.6f  %8.6f\n',k,w(1),w(2),w(3));
    k  = k +1;
end
k
w



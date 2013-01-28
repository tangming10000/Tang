clear;clc;
P = 0:0.1:2.0*pi;
T = exp(P);
net = newff([0,2.0*pi],[5,1],{'tansig','purelin'},'trainlm');
net = init(net);
net.trainParam.show = 100;
net.trainParam.epochs = 20000;
net.trainParam.goal = 0.0001;
net.trainParam.lr = 0.01;
[net,tr] = train(net,P,T);
P1 = 0:0.01:2.0*pi;
T1 = sim(net,P1);
figure(1);
plot(P,T,'*',P1,T1)

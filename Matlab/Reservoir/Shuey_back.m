function [Rp R0 G]=Shuey_back(vp1,vs1,rho1,vp2,vs2,rho2,th)
vp=(vp2+vp1)/2; 
Dvp=vp2-vp1;
rho =(rho1+rho2)/2;
Drho=rho2-rho1;

R0=0.5*(Dvp/vp + Drho/rho);
B=(Dvp/vp)/R0*2;

D1=(vs1/vp1)^2;PD1=(0.5-D1)/(1.0-D1);
D2=(vs2/vp2)^2;PD2=(0.5-D2)/(1.0-D2);

PD=(PD1+PD2)/2.0;DPD=PD2-PD1;

A0=B-(1+B)*(1+2*PD)/(1-PD);

G=A0*R0 +DPD/(1-PD)^2;

Rp=R0+G*(sin(th)^2);
end
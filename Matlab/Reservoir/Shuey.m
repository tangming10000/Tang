function [Rp R0 G]=Shuey(vp1,vs1,rho1,vp2,vs2,rho2,th)
vp=(vp2+vp1)/2; 
Dvp=vp2-vp1;
rho =(rho1+rho2)/2;
Drho=rho2-rho1;
vs=(vs1+vs2)/2;
Dvs=vs2-vs1;

R0=0.5*(Dvp/vp + Drho/rho);


G=0.5*Dvp/vp-4*(vs/vp)^2*(Dvs/vs)-2*(vs/vp)^2*(Drho/rho);

%Rp=R0+G*(sin(th)^2)+0.5*(Dvp/vp)*(tan(th)^2 - sin(th)^2);
Rp=R0+G*(sin(th)^2);
end
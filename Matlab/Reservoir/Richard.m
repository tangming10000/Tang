function R = Richard(vp1,vs1,rho1,vp2,vs2,rho2,th)
z=(vs1/vp1 + vs2/vp2)/2;
drho = (rho2 - rho1)/2; arho = (rho2 + rho1)/2;
dvp = ( vp2 - vp1 )/2;avp = (vp1 + vp2 ) /2 ;
dvs = ( vs2 - vs1) /2; avs = ( vs2 + vs1 ) /2;

th = th / 180*pi;
R = 0.5 *( 1 - 4 * z * sin(th)^2) * drho/arho + 0.5* sec(th)^2 *dvp/avp  -4* sin(th)^2 * dvs /avs;
end
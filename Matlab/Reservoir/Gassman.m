function f=Gassman(Kd,Kf,Km,p)
    f = Kd + ( 1 - Kd/Km)^2 / ( p/Kf + (1-p)/Km - Kd/Km^2);
end
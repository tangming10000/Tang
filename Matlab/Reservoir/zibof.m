function B=zibof(f)
m=1;
B=zeros(1,80);
for n=-20:1:20
    fn=(1-2*(0.001*n*pi*f)^2)*exp(-(0.001*n*pi*f)^2);
    B(m)=fn;
    m=m+1;
end
end

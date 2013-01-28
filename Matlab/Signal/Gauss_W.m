N=100;s=0.4;
for i=1:N;
    w(i) = exp( -0.5 * ( ( i - (N-1)/2.0 ) / ( s * ( N - 1 ) / 2.0 ) )^2 );
end
figure(1)
plot(w);
figure(2)
wf=fft(w);
wf = abs(wf);
plot(wf);
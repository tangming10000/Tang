function f = Gauss_window(x)
[m N] = size(x) ;
s=0.4;
w=1:N;
f=1:N;
    for i = 1 : N
        w(i) = exp( -0.5 * ( ( i - (N-1)/2.0 ) / ( s * ( N - 1 ) / 2.0 ) )^2 );
        f(i) = w(i) * x(i);
    end

end
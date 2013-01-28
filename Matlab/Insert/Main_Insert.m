close all;
clear;clc;
%对计算的数据做插值
Data = load('data.txt');
[LL WW] = size(Data) ;
Width = 8;
Height=26;
Length=194;
DD=zeros(Width,Length,Height);
% 将读入的地震数据转换成三维数据体
for i = 1:Height
    for j = 1:Length
        for k = 1:Width
            DD(k,j,i)=Data((i-1)*Length+j,k);
        end
    end
end

%对数据做三维插值
% Data2 = Insert_3D(DD);
% 针对读入的三维数据体做插值

%对数据做二维插值
for i = 1:Width
    D = zeros(Length,Height);
    for j = 1:Height
        for k = 1:Length
            D(k,j) = DD(i,k,j);
        end
    end
    
    D = Insert_2D(D);

    for j = 1:Height
        for k = 1:Length
             DD(i,k,j) = D(k,j);
        end
    end
    
end

%通过做图比较，沿深度-测线方向做距离平方成反比插值，能够得到比较好的效果
% implay(DD-3.6)
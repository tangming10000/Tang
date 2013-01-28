function DD = Insert_2D(Data)
%对输入的数据Data做二维距离平方反比插值
[Length Heigth] = size(Data);
% 设置搜索半径,大小1000m
R=1000;
% 设置在X,Y,Z方向上的间隔
XInterval=50;
YInterval=200;
ZInterval=200;
XNum = R/XInterval;
YNum = R/YInterval;
ZNum = R/ZInterval;

%搜索整个数据体，并进行插值
for i = 1:Length
    for j = 1:Heigth
        data=Data(i,j);
        if (data < 0 )
            %对小于0的处的值，进行插值
            
            %获取该点搜索的起始点和终止点
            XS = i - XNum;
            XE = i + XNum;
            ZS = j - ZNum;
            ZE = j + ZNum;
            if (XS <= 0 )
                XS = 1;
            end
            if (XE > Length)
                XE = Length;
            end
            if ( ZS <= 0 )
                ZS = 1;
            end
            if ( ZE > Heigth )
                ZE = Heigth;
            end
            
            %获取该点周围的非零值点
            Num = 0;
            XX=0;
            ZZ=0;
            for ii = XS:XE
                for jj = ZS:ZE
                    if (Data(ii,jj) > 0 )
                        DX = ii - i;
                        DZ = jj - j;
                        Dis = DX*DX*XInterval*XInterval+DZ*DZ*ZInterval*ZInterval;
                        if ( Dis < R*R)
                            Num = Num + 1;
                            XX(Num) = Dis;
                            ZZ(Num) = Data(ii,jj);
                        end
                    end
                    
                end
            end
            
            %获取完搜索半径内的值后，开始插值
            Dis = sum(1.0 ./ XX);
            Sum = 0;
            for ii = 1:Num
                Sum = Sum + ZZ(ii)/XX(ii)/Dis;
            end
            DD(i,j)=Sum;
        else
            DD(i,j)=Data(i,j);
        end 
    end
end

end
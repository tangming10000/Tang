function Data_Result = Insert_3D(Data)
%对输入的数据做三维的距离平方反比插值

[Width Length Height]=size(Data);

% 设置搜索半径,大小1000m
R=400;
% 设置在X,Y,Z方向上的间隔
XInterval=200;
YInterval=50;
ZInterval=200;
XNum = R/XInterval;
YNum = R/YInterval;
ZNum = R/ZInterval;

%从三位数据体中抽取要插值的数据
Data_Result=zeros(Width,Length,Height);

for i = 1:Height
    for j = 1:Length
        for k = 1:Width
            %若值小于零，则从周围数据中抽取开始插值
            if (Data(k,j,i) < 0 )
                %计算起始点和终止点
                XS = k - XNum;
                XE = k + XNum;
                YS = j - YNum;
                YE = j + YNum;
                ZS = i - ZNum;
                ZE = i + ZNum;
                
                if( XS <= 0)
                    XS = 1;
                end
                if( YS <= 0 )
                    YS = 1;
                end
                if( ZS <= 0 )
                    ZS = 1;
                end
                
                if (XE > Width)
                    XE = Width;
                end
                if (YE > Length)
                    YE = Length;
                end
                if (ZE > Height)
                    ZE = Height;
                end
                
                %开始搜索参与计算值
                Num = 0;
                DIS=0;
                Result=0;
                for ii = ZS:ZE
                    for jj = YS:YE
                        for kk = XS:XE
                            if (Data(kk,jj,ii)>0)
                                xx = kk-k;
                                xx = xx^2*XInterval^2;
                                yy = jj-j;
                                yy = yy^2+YInterval^2;
                                zz = ii-i;
                                zz = zz^2*ZInterval^2;
                                dis = xx+yy+zz;
                                if (dis < R^2)
                                    Num = Num +1;
                                    DIS(Num) = dis;
                                    Result(Num)=Data(kk,jj,ii);
                                end
                            end
                        end
                    end
                end
                
                %插值计算
                ms = sum(1.0 ./ DIS);
                temp = 0;
                for ii = 1:Num
                    temp = temp + Result(ii)/DIS(ii)/ms;
                end
                Data_Result(k,j,i) = temp;
            else
                %若该点值大于零，则复制值到新数组中
                Data_Result(k,j,i) = Data(k,j,i);
            end
        end
    end
end

end
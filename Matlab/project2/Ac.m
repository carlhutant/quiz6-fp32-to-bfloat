x1=random('Normal',0,0.08,1,900);
y1=random('Normal',0,0.08,1,900);
x2=random('Normal',0.9,0.08,1,900);
y2=random('Normal',0,0.08,1,900);
x3=random('Normal',0.5,0.08,1,900);
y3=random('Normal',0.8,0.08,1,900);
% hold on
% plot(x1,y1,'.b');
% plot(x2,y2,'.g');
% plot(x3,y3,'.r');
d1=[0,0,0];
d2=[0,0,0];
d3=[0,0,0];
pass=0;
count=1;
lost=1;
lostlist=0;
while lost~=0 && count<=100
    lost=0;
    pass=1;
    for i=1:900
        temp1=[x1(i),y1(i)]*d1(1:2)'+d1(3);
        temp2=[x1(i),y1(i)]*d2(1:2)'+d2(3);
        temp3=[x1(i),y1(i)]*d3(1:2)'+d3(3);
        temp1=1/(1+exp(-temp1));
        temp2=1/(1+exp(-temp2));
        temp3=1/(1+exp(-temp3));
        if temp1<=temp2
            pass=0;
            d1(1)=d1(1)+0.1*x1(i);
            d1(2)=d1(2)+0.1*y1(i);
            d1(3)=d1(3)+0.1;
            d2(1)=d2(1)-0.1*x1(i);
            d2(2)=d2(2)-0.1*y1(i);
            d2(3)=d2(3)-0.1;
        end
        if temp1<=temp3
            pass=0;
            d1(1)=d1(1)+0.1*x1(i);
            d1(2)=d1(2)+0.1*y1(i);
            d1(3)=d1(3)+0.1;
            d3(1)=d3(1)-0.1*x1(i);
            d3(2)=d3(2)-0.1*y1(i);
            d3(3)=d3(3)-0.1;
        end
        if pass==0
            lost=lost+1;
            pass=1;
        end
    
        temp1=[x2(i),y2(i)]*d1(1:2)'+d1(3);
        temp2=[x2(i),y2(i)]*d2(1:2)'+d2(3);
        temp3=[x2(i),y2(i)]*d3(1:2)'+d3(3);
        temp1=1/(1+exp(-temp1));
        temp2=1/(1+exp(-temp2));
        temp3=1/(1+exp(-temp3));
        if temp2<=temp1
            pass=0;
            d1(1)=d1(1)-0.1*x2(i);
            d1(2)=d1(2)-0.1*y2(i);
            d1(3)=d1(3)-0.1;
            d2(1)=d2(1)+0.1*x2(i);
            d2(2)=d2(2)+0.1*y2(i);
            d2(3)=d2(3)+0.1;
        end
        if temp2<=temp3
            pass=0;
            d2(1)=d2(1)+0.1*x2(i);
            d2(2)=d2(2)+0.1*y2(i);
            d2(3)=d2(3)+0.1;
            d3(1)=d3(1)-0.1*x2(i);
            d3(2)=d3(2)-0.1*y2(i);
            d3(3)=d3(3)-0.1;
        end
        if pass==0
            lost=lost+1;
            pass=1;
        end
    
        temp1=[x3(i),y3(i)]*d1(1:2)'+d1(3);
        temp2=[x3(i),y3(i)]*d2(1:2)'+d2(3);
        temp3=[x3(i),y3(i)]*d3(1:2)'+d3(3);
        temp1=1/(1+exp(-temp1));
        temp2=1/(1+exp(-temp2));
        temp3=1/(1+exp(-temp3));
        if temp3<=temp1
            pass=0;
            d1(1)=d1(1)-0.1*x3(i);
            d1(2)=d1(2)-0.1*y3(i);
            d1(3)=d1(3)-0.1;
            d3(1)=d3(1)+0.1*x3(i);
            d3(2)=d3(2)+0.1*y3(i);
            d3(3)=d3(3)+0.1;
        end
        if temp3<=temp2
            pass=0;
            d2(1)=d2(1)-0.1*x3(i);
            d2(2)=d2(2)-0.1*y3(i);
            d2(3)=d2(3)-0.1;
            d3(1)=d3(1)+0.1*x3(i);
            d3(2)=d3(2)+0.1*y3(i);
            d3(3)=d3(3)+0.1;
        end
        if pass==0
            lost=lost+1;
            pass=1;
        end
    end
    lostlist(count)=lost;
    count=count+1;
end
hold on
plot(x1,y1,'ob');
plot(x2,y2,'og');
plot(x3,y3,'or');
x=linspace(-1,2,100);
y=linspace(-1,2,100);
for i=1:100
    for j=1:100
        temp1=[x(i),y(j)]*d1(1:2)'+d1(3);
        temp2=[x(i),y(j)]*d2(1:2)'+d2(3);
        temp3=[x(i),y(j)]*d3(1:2)'+d3(3);
        if temp1>=temp2 && temp1>=temp3
            plot(x(i),y(j),'.b');
        elseif temp2>=temp3
            plot(x(i),y(j),'.g');           
        else
            plot(x(i),y(j),'.r');
        end
    end
end

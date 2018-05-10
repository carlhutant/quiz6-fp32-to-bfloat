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

hold on
plot(x1,y1,'ob');
plot(x2,y2,'og');
plot(x3,y3,'or');
x=linspace(-1,2,100);
y=linspace(-1,2,100);
for i=1:100
    for j=1:100
        min=100;
        d=0;
        for d1i=1:900
            temp=(x(i)-x1(d1i))^2+(y(j)-y1(d1i))^2;
            if min>temp
                min=temp;
                d=1;
            end
        end
        for d2i=1:900
            temp=(x(i)-x2(d2i))^2+(y(j)-y2(d2i))^2;
            if min>temp
                min=temp;
                d=2;
            end
        end
        for d3i=1:900
            temp=(x(i)-x3(d3i))^2+(y(j)-y3(d3i))^2;
            if min>temp
                min=temp;
                d=3;
            end
        end
        if d==1
            plot(x(i),y(j),'.b');
        elseif d==2
            plot(x(i),y(j),'.g');           
        else
            plot(x(i),y(j),'.r');
        end
    end
end

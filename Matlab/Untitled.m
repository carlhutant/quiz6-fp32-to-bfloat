clear
I=imread('HW2_einstein_orig.tif');
spot1=0;
spot2=100;
spot3=255;
count=1;
for i=0:255
    if i<=spot1
        a=0;
    elseif i<=spot2
        for j=1:100
            ref(count)=uint8(i);
            count=count+1;
        end
    elseif i<=spot3
        for j=1:20
            ref(count)=uint8(i);
            count=count+1;
        end
    else
        a=0;
    end
end
a=[];
for i=1:294000
    a(i)=I(i);
end
subplot(2,1,1);
hist(a,linspace(0,255,256));
J = imhistmatch(I,ref);
a=[];
for i=1:294000
    a(i)=J(i);
end
subplot(2,1,2);
hist(a,linspace(0,255,256));
figure;
imshow(J);
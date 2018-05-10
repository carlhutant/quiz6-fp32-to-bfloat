
clear
rng default
% m:���� v:�ܲ��� x:���ܼ�
% �ۭq²���禡  : func=@(�һݰѼ�)(�禡)
f_1=@(x,m,v)((exp(((-(x-m).^2)/(2*v))))/(sqrt(2*v*pi)));
%(exp(((-(x-m).^2)/(2*v))))/(sqrt(2*v*pi))

m=3;
v=4;
X=linspace(m-3*sqrt(v),m+3*sqrt(v),100);%�W���n�e�Ϫ��d��

Y1= f_1(X,m,v);
Y2= normpdf(X,m,sqrt(v));
Y = random('Normal',m,sqrt(v),1,1000);
figure
subplot(3,1,2)
hist(Y,100);
str=sprintf('Problem 1 Histogram form rand()\n1-D Gaussian (�g,�m^{2})=%f,%f',m,v);
title(str);

subplot(3,1,1)
plot(X,Y1,'b')%�v�I�e��
str=sprintf('Problem 1\n1-D Gaussian (�g,�m^{2})=%f,%f',m,v);
title(str);

subplot(3,1,3)
plot(X,Y2,'r')%�v�I�e��
str=sprintf('Problem 1 toolbox\n1-D Gaussian (�g,�m^{2})=%f,%f',m,v);
title(str);
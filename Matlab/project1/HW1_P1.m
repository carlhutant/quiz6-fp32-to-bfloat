
clear
rng default
% m:平均 v:變異數 x:自變數
% 自訂簡易函式  : func=@(所需參數)(函式)
f_1=@(x,m,v)((exp(((-(x-m).^2)/(2*v))))/(sqrt(2*v*pi)));
%(exp(((-(x-m).^2)/(2*v))))/(sqrt(2*v*pi))

m=3;
v=4;
X=linspace(m-3*sqrt(v),m+3*sqrt(v),100);%規劃要畫圖的範圍

Y1= f_1(X,m,v);
Y2= normpdf(X,m,sqrt(v));
Y = random('Normal',m,sqrt(v),1,1000);
figure
subplot(3,1,2)
hist(Y,100);
str=sprintf('Problem 1 Histogram form rand()\n1-D Gaussian (μ,σ^{2})=%f,%f',m,v);
title(str);

subplot(3,1,1)
plot(X,Y1,'b')%逐點畫圖
str=sprintf('Problem 1\n1-D Gaussian (μ,σ^{2})=%f,%f',m,v);
title(str);

subplot(3,1,3)
plot(X,Y2,'r')%逐點畫圖
str=sprintf('Problem 1 toolbox\n1-D Gaussian (μ,σ^{2})=%f,%f',m,v);
title(str);
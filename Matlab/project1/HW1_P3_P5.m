clear

m=0;
v=1;
Y=random('norm',m,v,1,500);
figure
hist(Y,100)
str=sprintf('Problem 3\n Histogram of 1-D Gaussian \n(£g,£m^2)=%f,%f',m,v);
title(str);
%%
figure
clear

M=[1 2];
C=[3 0;0 4];
N=100*100;
Z=mvnrnd(M,C,N);
plot(Z(:,1),Z(:,2),'.')
str=sprintf('Problem 4-1\n 2-D Gaussian data \n£g=(%f,%f)\nCovariance=%f %f\n                      %f %f',M,C);
title(str);
%%
figure
hist3(Z,[25 25])
A=hist3(Z,[25 25]);
set(get(gca,'child'),'FaceColor','interp','CDataMode','auto');
str=sprintf('Problem 4-2\n Histogram of 2-D Gaussian');
title(str);

%%
figure
contour(A,'ShowText','on')
str=sprintf('Problem 5\n Contour of 2-D Gaussian');
title(str);
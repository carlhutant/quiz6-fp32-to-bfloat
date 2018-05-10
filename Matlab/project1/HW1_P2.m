
clear
rng default


M=[1;2];
C=[1 0;0 1];
x=linspace(M(1)-3*sqrt(C(1,1)),M(1)+3*sqrt(C(1,1)),100);
y=linspace(M(2)-3*sqrt(C(2,2)),M(2)+3*sqrt(C(2,2)),100);
[X,Y]=meshgrid(x,y);

% sqrt(2*pi*det(C))
% ((X-M(1))^2/(2*C(1,1))) 
% ((Y-M(2))^2/(2*C(2,2)))
%(((X-M(1))^2/(2*C(1,1)))+((Y-M(2))^2/(2*C(2,2))));
f_2=@(X,Y,M,C)(1/sqrt(det(2*pi*C)))*...
    exp((-1/2)*([X;Y]-M)'*inv(C)*([X;Y]-M));
Z1=zeros(size(X));
for i=1:length(X)
    for j=1:length(Y)
        Z1(i,j)=f_2(X(i,j),Y(i,j),M,C);
    end
end

% toolbox
Z2=mvnpdf([X(:) Y(:)],M',C);
Z2=reshape(Z2,sqrt(length(Z2)),sqrt(length(Z2)));

Z3= mvnrnd(M',C,10000);
figure

subplot(1,2,1)
mesh(X,Y,Z1);
str=sprintf('Problem 2\n2-D Gaussian £g=(%f,%f)\nCovariance=%f %f\n                      %f %f',M,C);
title(str);
subplot(1,2,2)
mesh(X,Y,Z2);
str=sprintf('Problem 2 toolbox\n2-D Gaussian £g=(%f,%f)\nCovariance=%f %f\n                      %f %f',M,C);
title(str);

figure
hist3(Z3,[25 25])
A=hist3(Z3,[25 25]);
set(get(gca,'child'),'FaceColor','interp','CDataMode','auto');
str=sprintf('Problem 2\n Histogram of 2-D Gaussian');
title(str);
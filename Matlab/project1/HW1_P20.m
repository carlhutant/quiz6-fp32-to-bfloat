clear
figure
% load data
% http://ufldl.stanford.edu/wiki/index.php/MATLAB_Modules
Images = loadMNISTImages('t10k-images.idx3-ubyte');
% rand chose index

num=ceil(random('unif',1,length(Images),10,15));

for i=1:10
    for j=1:15
        img(28*(i-1)+1:28*i,28*(j-1)+1:28*j)=reshape(Images(:,num(i,j)),28,28);
    end
end

%imshow(uint8(256*reshape(Images(:,floor(random('unif',0,60000,1,1))),28,28)))
imshow(uint8(256*img))
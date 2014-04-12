global X Y Z
im=rand(5);
im=imresize(im,[300,300]);
[X,Y]=meshgrid(1:300,1:300);
Z=im;
X=X-150;
Y=Y-150;
%imagesc(im);
contour(X,Y,Z);


fn='billboard.jpg'
im=imread(fn);
image(im);
xy=ginput;
nFace=1;
nVertex=size(xy,1);
save billboard nFace nVertex xy


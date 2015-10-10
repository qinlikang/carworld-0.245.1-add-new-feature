%% before using this script, ensure saveconemushroom has been modified!!!
% inside the saveconemushroom.m, we should use:
% load billboard
% im=imread('billboard.png');
% fid=fopen('billboard.off','w');
% fprintf(fid,'OFF2\nbillboard.png\n');

nFace  =1;
nVertex=4;
xy = [0,0;640,0;640,960;0,960];% the image size is 960*640, so x ranges in [0,640],y ranges in [0,960]
save billboard.mat
saveconemushroom
copyfile billboard.off ./off_editor/_billboard.off
cd off_editor\

convert_off('_billboard.off',... % the input file (just created by saveconemushroom.m)
    'billboard.off',... % the output file that used in game
    1,...% the scale factor
    0,...% whether we flip the picture in left/right direction. just like fliplr
    1,...% whether we flip the picture in up/down direction. just like flipud
    [10,2,0]);% finally move the picture away from origin, so that when we
              % add it, we add it outside the road. this setting means moving 
              % 10 in right direction, and move 2 in up direction

copyfile billboard.off ../../data/landscape/billboard.off
cd .. 

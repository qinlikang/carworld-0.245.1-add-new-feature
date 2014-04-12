%load mushroom
%im=imread('mushroom.jpg');
load cone
im=imread('cone.jpg');
X=xy(:,1);
Y=xy(:,2);
%fid=fopen('mushroom.off','w');
%fprintf(fid,'OFF2\nmushroom.png\n');
fid=fopen('cone.off','w');
fprintf(fid,'OFF2\ncone.png\n');
nv=length(X);
nf=1;
nfv=nv
fprintf(fid,'%d %d 0\n',nv,nf);
for i=1:nv
    for j=1:2
        fprintf(fid,'%f ',xy(i,j));
    end
    fprintf(fid,'0 ');
    for j=1:2
        fprintf(fid,'%f ',xy(i,j)/size(im,j));
    end
    fprintf(fid,'\n');
end
for i=1:nf
    fprintf(fid,'%d ',nfv);
    for j=1:nfv
        fprintf(fid,'%d ',j-1);
    end
    fprintf(fid,'1 1 1\n');
end
fclose(fid);
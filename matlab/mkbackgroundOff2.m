[X,Y,Z]=cylinder(1,36);
[f,v,c]=surf2patch(X,Y,Z);
fid=fopen('Background.off','w');
fprintf(fid,'OFF2\n2006-02-26a.png\n');
nv=size(v,1);
nf=size(f,1);
nfv=size(f,2)
fprintf(fid,'%d %d 0\n',nv,nf+2);
v2=v;
%v2(v2==0)=0.001;
%v2(v2==1)=0.999;
pciturex=mod(cart2pol(v(:,1)-1,v(:,2)-1)+2*pi,2*pi)/2/pi;
picturey=v2(:,3);
%pciturex(pciturex==0)=0.001;
v(:,3)=v(:,3)*1.2-0.6;
for i=1:nv
    for j=1:3
        fprintf(fid,'%f ',v(i,j));
    end
    fprintf(fid,'%f %f \n',pciturex(i),1-picturey(i));
end
for i=1:nf
    fprintf(fid,'%d ',nfv);
    for j=1:nfv
        fprintf(fid,'%d ',f(i,j)-1);
    end
    fprintf(fid,'1 1 1\n');
end
for k=0:1
    fprintf(fid,'%d ',nv/2);
    for j=1:nv/2
        fprintf(fid, '%d ', (j-1)*2+k);
    end
    fprintf(fid,'1 1 1\n');
end
fclose(fid);
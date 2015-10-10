function write_block(c,fn)
% first two point sould be -5 0 +5 0
fid=fopen(fn,'w');
nV=size(c,1);
nF=nV-2;
fprintf(fid,'%d %d\n',nV,nF);
d=1/6;
for i=0:(nV-1)
    for j=1:2
        fprintf(fid,'%f ',c(i+1,j));
    end
    %fprintf(fid,'0\n%d %f\n',mod(i,2),d*mod(i/2,7));
    fprintf(fid,'0\n%d %f\n',c(i+1,3),c(i+1,4));
end
for i=0:2:(nF-1)
    fprintf(fid,'%d %d %d 1\n',i,i+2,i+1);
    fprintf(fid,'%d %d %d 1\n',i+1,i+2,i+3);
end
d=eye(3);
for i=1:3
    for j=1:3
        fprintf(fid,'%d ',d(i,j));
    end
    fprintf(fid,'\n');
end
fclose(fid);


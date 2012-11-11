function [t,xyz]=readRecord(fn)
fid=fopen(fn);
data=fscanf(fid,'%f');
data=reshape(data,4,length(data)/4);
t=data(1,:);
xyz=data([2:4],:)';

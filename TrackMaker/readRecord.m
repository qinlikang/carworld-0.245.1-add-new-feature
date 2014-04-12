function [t,xyz]=readRecord(fn)
fid=fopen(fn);
data=fscanf(fid,'%f');
data=reshape(data,5,length(data)/5);
t=data(2,:);
xyz=data([3:5],:)';

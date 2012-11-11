fn='../Records/road';
FV=read_off(fn);
[time,xyz]=readRecord('../Records/VehicleStateRecorder/2012-10-5_12-39-56.txt');
n=size(xyz,1);
for i=1:n-1
    d(i)= sqrt( sum( (xyz(i+1,:)-xyz(i,:)).^2))/(time(i+1)-time(i));
end

mymap=colormap(hsv);
for k=1:2
    hold on
    plot3(FV.Vertices(k:2:end,1),FV.Vertices(k:2:end,2),FV.Vertices(k:2:end,3),'-k','LineWidth',[2]);
end
m0=min(d);
m1=max(d);
d1=floor((d-m0)/(m1-m0)*63)+1;
for i=1:n-1
    plot3( xyz([i i+1],1),xyz([i i+1],2),xyz([i i+1],3),'Color',mymap(d1(i),:));
end
zlim(40*[-1 1])
return
fill3( vertcat( FV.Vertices(1:2:end,1),FV.Vertices(2:2:end,1)), ...
    vertcat( FV.Vertices(1:2:end,2),FV.Vertices(2:2:end,2)),...
    vertcat( FV.Vertices(1:2:end,3),FV.Vertices(2:2:end,3)),'w');
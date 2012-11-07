function FV=read_off(fn)
fid=fopen(fn,'r');
fgets(fid); % remove off
data=fscanf(fid,'%f');
i=0;
nV=data(i+1);i=i+1;
nS=data(i+1);i=i+1;
i=i+1;
for v=1:nV
	Vertex(:,v)=data(i+[1:3]);i=i+3;
end
FV.Vertices=Vertex';

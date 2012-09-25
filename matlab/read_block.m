function [FV,matrix]=read_block(fn)
fid=fopen(fn,'r');
data=fscanf(fid,'%f');
i=0;
nV=data(i+1);i=i+1;
nS=data(i+1);i=i+1;
for v=1:nV
	Vertex(:,v)=data(i+[1:3]);i=i+5;
end
m0=min(Vertex(3,:));
m1=max(Vertex(3,:));
for f=1:nS
	sq(:,f)=data(i+[1:3]);i=i+4;
	FaceVertexCData(f)=63*(Vertex(3,1+sq(1,f))-m0)/(m1-m0)+1;
end
FV.Vertices=Vertex';
FV.Faces=1+sq';
FV.EdgeColor=[0 0 0]+0.5;
FV.FaceVertexCData=FaceVertexCData';
matrix=reshape(data(i+[1:9]),3,3);

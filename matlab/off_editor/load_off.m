function offobj = load_off(filename)
fid=fopen(filename,'r');
% check file version
format = fgets(fid); 
format(find(isspace(format)))=[];
texture = '';
if strcmp(format,'OFF2')
    texture=fgets(fid);
    texture(find(isspace(texture)))=[];
end
% read data
data = fscanf(fid,'%f');
i=0;
nVertex=data(i+1);i=i+1;
nSurface=data(i+1);i=i+1;
i=i+1;

UV=[];
for v=1:nVertex
    Vertex(:,v)=data(i+[1:3]);
    if strcmp(format,'OFF')
        i=i+3;
    else
        UV(:,v)=data(i+[4:5]);
        i=i+5;
    end
end
i=i+1;
for f=1:nSurface
    npt = data(i);
    Surfaces{f}=data(i+[1:npt]);
    i=i+npt+1;
    if data(i)>=0 && data(i)<=1
        Colors(:,f)=data(i+[0:2]);
        i=i+3;
    else
        Colors(:,f)=[.5 .5 .5];
    end
end

offobj.Format=format;
offobj.Texture=texture;
offobj.nVertex=nVertex;
offobj.nSurface=nSurface;
offobj.Vertex=Vertex;
offobj.Surfaces=Surfaces;
offobj.Colors=Colors;
offobj.TextureCoord=UV;


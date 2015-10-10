function save_off(obj,filename)
fid=fopen(filename,'w');
% write file version
format = obj.Format;
fprintf(fid,'%s\n',format); 
texture = obj.Texture;
if texture
    fprintf(fid,'%s\n',texture);
end
% write data
nVertex=obj.nVertex;
nSurface=obj.nSurface;
fprintf(fid,'%d ',nVertex);
fprintf(fid,'%d 0\n',nSurface);

if strcmp(format,'OFF2')
    mat = [obj.Vertex;obj.TextureCoord];
else
    mat = [obj.Vertex];
end
write_matrix(fid,mat');

for i=1:nSurface
    npt = size(obj.Surfaces{i},1);
    fprintf(fid,'%d ',npt);
    fprintf(fid,'%d ',obj.Surfaces{i});
    fprintf(fid,'%.3f ',obj.Colors(:,i));
    fprintf(fid,'\n');
end

fclose(fid);

function write_matrix(fid,mat)
for i=1:size(mat,1)
    fprintf(fid,'%.6f ',mat(i,:));
    fprintf(fid,'\n');
end
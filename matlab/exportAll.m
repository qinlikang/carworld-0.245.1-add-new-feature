l=dir('*.txt');
for i=1:length(l)
[FV,matrix]=read_block(l(i).name);
[pathstr, name, ext] = fileparts(l(i).name);
vertface2obj(FV.Vertices,FV.Faces,sprintf('%s.obj',name));
end

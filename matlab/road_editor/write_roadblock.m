function write_roadblock(filename,block)
fid = fopen(filename,'wb');
if fid==-1
    msgbox(sprintf('%s, is occupied by other programs. saving error'));
    return
end
% first line
fprintf(fid,'%d %d\n',block.point_num,block.tri_num);
% points
for i=1:block.point_num
    pos = block.point_array(i).position;
    uv = block.point_array(i).uv;
    fprintf(fid,'%f %f %f\n',pos(1),pos(2),pos(3));
    fprintf(fid,'%f %f\n',uv(1),uv(2));
end
for i=1:block.tri_num
    tri = block.tri_array(i).index;
    fprintf(fid,'%d %d %d 1\n',tri(1),tri(3),tri(2));
end
% endpos, forward, upward
vec = block.end_position;
fprintf(fid,'%f %f %f\n',vec(1),vec(2),vec(3));

vec = block.endref(:,2);
fprintf(fid,'%f %f %f\n',vec(1),vec(2),vec(3));

vec = block.endref(:,3);
fprintf(fid,'%f %f %f\n',vec(1),vec(2),vec(3));
fclose(fid);

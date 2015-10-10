function block = read_roadblock(filename)
fhandle = fopen(filename);
data = fscanf(fhandle,'%g');
fclose(fhandle);
ptnum = data(1);
trinum = data(2);
block.point_num = ptnum;
block.tri_num = trinum;
% read all points
startpos = 3;
endpos = 2+ptnum*5;
block.point_array=makepts(data(startpos:endpos));
% read all triangles
startpos = endpos+1;
endpos = endpos+trinum*4;
block.tri_array=maketris(data(startpos:endpos));
% read end position
startpos = endpos+1;
endpos = endpos+3;
block.end_position = data(startpos:endpos);
% read end transform matrix
block.endref=zeros(3,3);
forward = data(startpos+3:endpos+3);
up = data(startpos+6:endpos+6);
block.endref(:,2) = forward';
block.endref(:,3) = up';
block.endref(:,1) = cross(forward,up)';

function pt = makepts(array)
len = length(array);
for i = 1:len/5
    start = (i-1)*5;
    pt(i).position = array(start+1:start+3);
    pt(i).uv = array(start+4:start+5);
end

function tri = maketris(array)
len = length(array);
for i = 1:len/4
    start = (i-1)*4;
    tri(i).index = array(start+1:start+3);
end
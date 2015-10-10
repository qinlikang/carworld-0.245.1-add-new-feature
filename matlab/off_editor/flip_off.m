function ret = flip_off(obj,axis_number)
v = obj.Vertex(axis_number,:);
maxv = max(v);
minv = min(v);
v=minv+maxv-v;
ret = obj;
ret.Vertex(axis_number,:)=v;

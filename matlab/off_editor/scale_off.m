function ret = scale_off(obj,fscale)
ret=obj;
ret.Vertex = eye(3)*fscale*obj.Vertex;
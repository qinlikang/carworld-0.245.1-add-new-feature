function convert_off(infile,outfile,scale,xflip,yflip,vmove)
if ~exist('vmove')
    vmove = [0 0 0];
else
    vmove = [vmove(1) vmove(3) vmove(2)];
end

obj = load_off(infile);
obj = process_default(obj,scale);
if exist('xflip') && xflip==1
    obj = flip_off(obj,1);
end
if exist('yflip') && yflip==1
    obj = flip_off(obj,3);
end

obj.Vertex = obj.Vertex+repmat(vmove',1,size(obj.Vertex,2));
save_off(obj,outfile);
system(['ShowOFF.exe ',outfile]);
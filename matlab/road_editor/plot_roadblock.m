function plot_roadblock(points,indexs,varargin)
if isempty(varargin)
    ref = eye(3);
else
    ref = varargin{1};
end
points = ref*points;

indexs = uint32(indexs)+1;
num=size(indexs,2);
for i=1:num
    pts = points(:,indexs(:,i));
    pts = [pts, pts(:,1)];
    plot3(pts(1,:),pts(2,:),pts(3,:));
    hold on;
end
hold off;

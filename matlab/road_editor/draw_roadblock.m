function draw_roadblock(left,right,ishittest)
len = size(left,1);
if ishittest
    flag = 'on';
else
    flag = 'off';
end
hold on;
for i=1:len-1
    xs = [left(i,1),left(i+1,1),right(i,1)];
    ys = [left(i,2),left(i+1,2),right(i,2)];
    zs = [left(i,3),left(i+1,3),right(i,3)];
    h = fill(xs',ys',zs');
    set(h,'hittest',flag);

    xs = [left(i+1,1),right(i+1,1),right(i,1)];
    ys = [left(i+1,2),right(i+1,2),right(i,2)];
    zs = [left(i+1,3),right(i+1,3),right(i,3)];
    h = fill(xs',ys',zs');
    set(h,'hittest',flag);
end
hold off;

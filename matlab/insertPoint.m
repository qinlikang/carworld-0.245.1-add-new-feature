function x1=insertPoint(x,n)
if( nargin < 2 )
	  selfTest();
	  return
end
m=size(x,1);
t(1)=0;
for i=2:m
	t(i)= sqrt(sum((x(i,:)-x(i-1,:)).^2));
end
indnono0=t>0;
t=t(indnono0);
x=x(indnono0,:);
m=size(x,1);
t2(1)=0;
for i=2:m
	t2(i)= t(i)+t2(i-1);
end
ind=[t2(1): (t2(end)-t2(1))/n:t2(end)];
for i=1:size(x,2)
    try
x1(:,i)=interp1(t2,x(:,i),ind, 'pchip');
    catch
        clf
        plot(t,x(:,i));
    end
end
x1=x1(1:n,:);

function selfTest()
x=[0:1:10]';
x(:,2)=x.^2;

x1=insertPoint(x,100);
clf
plot(x(:,1),x(:,2),'o-');
hold on
plot(x1(:,1),x1(:,2),'+r');

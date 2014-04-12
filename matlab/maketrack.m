clear
if ~exist('a')
    figure; hold on
    a=mousetrack2; % draw a track.
    a=a*300;
    n=length(a);
    l(1)=0;
    for i=1:n-1
        l(i+1)=l(i)+pdist(a(i+[0 1],:)); % l(i) save the distance between a(i) and a(1) along the track
    end
    clear b c b2
    ind=[0:(l(end)/1200.0):l(end)];
    % cal b: the interpolate points of a. function is l=Fun(a).
    for i=1:2
        b(:,i)=interp1(l,a(:,i),ind, 'spline');
    end
    % put b to the original point
    for i=1:2
        b(:,i)=b(:,i)-b(1,i);
    end
        
    startingAngle=cart2pol(b(2,1),b(2,2));
    da=1.5708-startingAngle; % the first point should in 90 degree

    %cal b2: rotate b by da (so that b2 walk along up direction?)
    [an,r]=cart2pol(b(:,1),b(:,2));
    [b2(:,1), b2(:,2)]=pol2cart(an+da,r);

    plot(b(:,1),b(:,2),'o');
    plot(b2(1:40,1),b2(1:40,2),'rx');
    plot(b(1:40,1),b(1:40,2),'o');
end

% a: mouse track * 300
% b: interploation points of a. num:1200, keep length unchanged, start at
% orginal point
% b2: rotate b so that it start in positive y-direction.
n=length(b);
pn=[1 -1];

% b3: smooth b2.
for i=1:2
    b3(:,i)=smooth(b2(:,i),30,'lowess');
end
 [an,r]=cart2pol(b3(:,1),b3(:,2));
 clear b4 c
n4=size(an,1);

% keepind: record the points that make a big change in angle
keepind=an*0;
 for i=1:(n4-1)
     for j=i:min([n4 i+200]); 
         da=an(j)-an(i);
         if abs(da)>.2;
             keepind(j)=1;
             i=j;
             break;
         end
     end
 end
 %b4=b3(keepind>0,:); n5=sum(keepind);
 
% cal c: two line by perpendicularly moving the points of b3.
for i=1:n-1
    [TH,R] = cart2pol(diff(b3(i+[0 1],1)),diff(b3(i+[0 1],2)));
    for k=1:2
        th1=TH+pn(k)*pi/2; % so that th1 is direction perpendicular to the b3 line
        [x,y]=pol2cart(th1,5);
        d((i-1)*2+k,:)=[x,y];
        c((i-1)*2+k,1)=x+b3(i,1);
        c((i-1)*2+k,2)=y+b3(i,2); 
    end
end

c(3:end,2)= c(3:end,2)-c(3,2)+10;

figure
hold on
plot(b2(1:40,1),b2(1:40,2)+10,'b.');
for i=0:40
    for k=1:2
        plot(c(i+k,1),c(i+k,2),'ro');
    end
end

write_block(c,'../data/Landscape/PeterDriving/road1.txt');%,keepind);


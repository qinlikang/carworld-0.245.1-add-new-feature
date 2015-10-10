function write_block_B(b3,fn)
pn=[1 -1];
[an,r]=cart2pol(b3(:,1),b3(:,2));
n4=size(an,1);
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
d=1/6;
n=1;
keepind(1:4)=1;
for i=1:n4-1
    if keepind(i)==0; continue; end
    [TH,R] = cart2pol(diff(b3(i+[0 1],1)),diff(b3(i+[0 1],2)));
    for k=1:2
        th1=TH+pn(k)*pi/2;
        [x,y]=pol2cart(th1,5);
        nn=(n-1)*2+k;
        c(nn,1)=x+b3(i,1);
        c(nn,2)=y+b3(i,2);
        c(nn,3)=k-1;
        c(nn,4)=d*mod(i,7);
    end
    n=n+1;
end
c(3:end,2)= c(3:end,2)-c(3,2)+10;
write_block(c,fn);%,keepind);

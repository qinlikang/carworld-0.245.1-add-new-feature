if ~exist('FV')
    fn='../Records/road';
    FV=read_off(fn);
    
    fns{1,1}='xian2012-10-13_14-21-53nofog';
    fns{2,1}='xian2012-10-13_14-22-59fog';
    fns{1,2}='Adam2012-10-12_16-1-49_nofog';
    fns{2,2}='Adam2012-10-12_16-7-51_fog';
    SubjectName={'X' 'A'};
    Cname={'nofog' 'fog'};
    
    for subject=1:1
        for condition=1:2
            [T{condition,subject},X{condition,subject}]=readRecord(sprintf('../Records/VehicleStateRecorder/%s.txt',fns{condition,subject}));
            time=T{condition,subject};
            xyz=X{condition,subject};
            n=size(xyz,1);
            for i=1:n-1
                d(i)= sqrt( sum( (xyz(i+1,:)-xyz(i,:)).^2))/(time(i+1)-time(i));
            end
            D{condition,subject}=d;
            M1(condition,subject)=max(d(d<100000));
            M0(condition,subject)=min(d(d<100000));
        end
    end
    m0=min(M0(:));
    m1=max(M1(M1<100000));
    fogst='-:';
    lw=[1 2];
    
    for k=1:2
        x0(:,:,k)=FV.Vertices(k:2:end,:);
    end
    nb=1;
    N=size(x0,1)/nb;
    ndot=1000;
    for k=1:2
        for i=1:nb
            ind=(i-1)*N+[1:N];
            ind0=(i-1)*ndot+[1:ndot];
            x1(ind0,:,k)=insertPoint(x0(ind,:,k),ndot);
        end
        plot3(x1(:,1,k),x1(:,2,k),x1(:,3,k),'.b');
    end
end

close all
for subject=1:1
    figure
    hold on
    mymap=colormap(hsv);
    for k=1:2
        hold on
        plot3(x1(:,1,k),x1(:,2,k),x1(:,3,k),'k');
    end
    for condition=1
        % figure
        clear dist1
        xyz=X{condition,subject};
        for k=1:2
            dist1(:,:,k)=pdist2(xyz,x1(:,:,k));
        end
        V=exp(-dist1);
        dv=diff(V,1,1);
        mdv=max(max(dv,[],3),[],2);
        n=size(mdv,1);
        d1=floor((mdv)/(max(mdv)-0)*63)+1;
        for i=1:n-1
            if (d1(i)<=100000)
                plot3( xyz([i i+1],1),xyz([i i+1],2),xyz([i i+1],3),fogst(condition), ...
                    'Color',mymap(d1(i),:),'LineWidth',lw(condition));
            end
        end
        title( sprintf('dv/dt %s %s',SubjectName{subject}, Cname{condition}));
    end
end
saveas(gcf,'dvdt.pdf','pdf');
return
%good and done
close all
for subject=1:1
    figure
    hold on
    mymap=colormap(hsv);
    hold on
    for k=1:2
        
        plot3(x1(:,1,k),x1(:,2,k),x1(:,3,k),'k');
    end
    for condition=1
        % figure
        d1=floor((D{condition,subject}-m0)/(m1-m0)*63)+1;
        xyz=X{condition,subject};
        n=size(xyz,1);
        for i=1:n-1
            if (d1(i)<=100000)
                plot3( xyz([i i+1],1),xyz([i i+1],2),xyz([i i+1],3),fogst(condition), ...
                    'Color',mymap(d1(i),:),'LineWidth',lw(condition));
            end
        end
        title( sprintf('%s %s',SubjectName{subject}, Cname{condition}));
    end
end
saveas(gcf,'speed.pdf','pdf');
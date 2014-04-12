% this program find out which road blocks are flat.
if ~exist('rawblockname')
    paramter
    nFlat=0;
    for i=1:nBlock
        [FV,matrix]=read_block(sprintf('%s/%s.txt',roadblockFolder,rawblockname{i}));
        if std(FV.Vertices(1:2:end,3))>0; continue; end
        nFlat=nFlat+1;
        FlatBlockName{nFlat}=rawblockname{i};
        subplot(4,4,nFlat);hold on
        for k=1:2
            plot(FV.Vertices(k:2:end,1),FV.Vertices(k:2:end,2),'-k');
        end
        axis(60*[-1 1 -1 1])
        title(rawblockname{i});
    end
    saveas(gcf,'Flatblocks.pdf','pdf');
end
n=0;
clf
for i=1:nFlat
    for j=1:nFlat
        temp=sprintf('tmp/%s_%s',FlatBlockName{i},FlatBlockName{j});
        temptxt=sprintf('tmp/%s_%s.txt',FlatBlockName{i},FlatBlockName{j});
        fid=fopen(temptxt,'w');
        fprintf(fid,'2\n');
        for k=[i j]
            fprintf(fid,'../%s/%s.txt\n',roadblockFolder,FlatBlockName{k});
        end
        temptxt=sprintf('tmp/%s_%s.off',FlatBlockName{i},FlatBlockName{j});
        if ~exist(temptxt); mergeRoadbloc(temp); end
        FV=read_off(temptxt);
        n=n+1;
        subplot(7,7,n);
        hold on
        
        for k=1:2
            plot(FV.Vertices(k:2:end,1),FV.Vertices(k:2:end,2),'-k');
             % plot(FV.Vertices(k:2:end,1),FV.Vertices(k:2:end,2),'r.');
        end
        axis(80*[-1 1 -1 1])
        axis off
        temp=strrep(temp,'tmp/','');
        temp=strrep(temp,'_','-');
        temp=strrep(temp,'Right','R');
        temp=strrep(temp,'Left','L');
        title(sprintf('%d %s',n,temp));
    end
end
saveas(gcf,'combRoadblock.pdf','pdf');


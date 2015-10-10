[FV,matrix]=read_block('EndDown.txt');
patch(FV);
material shiny; 
alpha('color'); 
alphamap('rampdown'); 
camlight(45,45); 
lighting phong 
view(30,30);

fns={'Flat' 'Left45' 'Left45B' 'Left180' 'Left180B' 'loop1' 'loop2'};
for i=1:4
    subplot(2,2,i);
    [FV,matrix]=read_block(sprintf('../data/Landscape/PeterDriving/%s.txt',fns{i}));
    for k=1:2
        hold on
 plot(FV.Vertices(k:2:end,1),FV.Vertices(k:2:end,2),'-k');
    end
 axis(60*[-1 1 -1 1])
end
saveas(gcf,'blocks.pdf','pdf');

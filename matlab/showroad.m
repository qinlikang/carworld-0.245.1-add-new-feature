fn= '../data/landscape/PeterDriving/Left180.txt';
fn='../Records/raod';
[FV,matrix]=read_block(fn);
plot(FV.Vertices(:,1),FV.Vertices(:,2),'o');

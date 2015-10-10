[FV,matrix]=read_block('EndDown.txt');
patch(FV,'FaceColor','flat','FaceVertexCData',FV.FaceVertexCData,'EdgeColor','none');
material shiny; 
alpha('color'); 
alphamap('rampdown'); 
camlight(45,45); 
lighting phong 
view(30,30);

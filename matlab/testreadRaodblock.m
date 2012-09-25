[FV,matrix]=read_block('EndDown.txt');
patch(FV);
material shiny; 
alpha('color'); 
alphamap('rampdown'); 
camlight(45,45); 
lighting phong 
view(30,30);

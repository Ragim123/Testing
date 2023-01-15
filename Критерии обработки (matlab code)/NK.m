 clear all;


  I = im2double(imread('house.png'));
 I2 = im2double(imread('house2.png'));

 
  

N=sum(sum(((I*I2))));
N=N/sum(sum(((I^2))))




















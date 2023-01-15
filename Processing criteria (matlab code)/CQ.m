 clear all;


  I = im2double(imread('house.png'));
 I2 = im2double(imread('house2.png'));

 
  

C=sum(sum(((I*I2))));
C=C/sum(sum(((I))))
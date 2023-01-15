
 clear all;


  I = im2double(imread('house.png'));
 I2 = im2double(imread('house2.png'));

 
  

F=sum(sum(((I^2))));
F=F/sum(sum(((I-I2)^2)))
F=10*log(10)


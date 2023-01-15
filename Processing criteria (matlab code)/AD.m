
 clear all;
 I=imread('house.png');
 I2=imread('house2.png');

%  I = im2double(imread('house.png'));
%  I2 = im2double(imread('house2.png'));
%  
 
 M = size(I,1);  % kolichestvo strok
 N = size(I,2);   % kolichestvo stolbcov
 
  
A=sum(sum(((I-I2)^2)));

A=A/(M*N)

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 





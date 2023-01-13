clc
clear all
close all

I = imread('1.png');
Igray = rgb2gray(I);

block_size = 40;
min_block_size = 0;
max_block_size = 8;

NewImageGray = Igray(1:((floor(size(Igray,1)/block_size))*block_size),1:((floor(size(Igray,2)/block_size))*block_size));

breakFunc = 0;

for i = 1:block_size:size(NewImageGray,1)
   for j = 1:block_size:size(NewImageGray,2)
      
      if (breakFunc==0)
        if (min_block_size<max_block_size)
          
          min(min(corrcoef(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)))))
          figure,imshow(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)));
          min_block_size = min_block_size + 1;
        
        else

          breakFunc = 1;

        end
      end
   end
end
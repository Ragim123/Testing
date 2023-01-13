clc
clear all
close all

I = imread('888.png');
Igray = rgb2gray(im2double(I));

block_size = 256;
min_block_size = 0;
max_block_size = 0; %если равно 0, то по всему изображению
CorrVal = [];

NewImageGray = Igray(1:((floor(size(Igray,1)/block_size))*block_size),1:((floor(size(Igray,2)/block_size))*block_size));

breakFunc = 0;

for i = 1:block_size:size(NewImageGray,1)
   for j = 1:block_size:size(NewImageGray,2)
      
        if (min_block_size<max_block_size) && (max_block_size~=0)
              
            figure,imshow(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)));
            Val = min(min(corrcoef(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)))));
            min_block_size = min_block_size + 1;
            CorrVal = cat(2,CorrVal,Val);
        
        end
        
        if (max_block_size==0)
            figure,imshow(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)));
            Val = min(min(corrcoef(NewImageGray(i:(i+block_size-1),j:(j+block_size-1)))));
            CorrVal = cat(2,CorrVal,Val);
        end
        
        clear Val

   end
end

disp(CorrVal);

II= imread ('888.png');
II = rgb2gray (im2double(II));

Imask = imread ('101010.png');
Imask = rgb2gray (im2double(Imask));
figure, imshow(Imask);

JJ = imread ('101010.png');
JJ = rgb2gray (im2double(JJ));
figure, imshow(JJ);

%поиск области восстановления
[ZoneRect]=findZone(Imask);

S1 = II(ZoneRect(1,1):ZoneRect(2,1),ZoneRect(1,2):ZoneRect(2,2));
figure, imshow(S1);
S2 = JJ(ZoneRect(1,1):ZoneRect(2,1),ZoneRect(1,2):ZoneRect(2,2));
figure, imshow(S2);

[error_criteria,Error_labels]=function_Errors(S1,S2)

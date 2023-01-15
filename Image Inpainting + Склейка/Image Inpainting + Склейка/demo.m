
% Reset Matlab environment.
clear all; 
close all;
% Select demo parameters.
imageIndex  = 2;  % {1 = step function, 2 = image sample}
maskIndex   = 6;  % {1 = circle, 2 = double-circle, 3 = square, 4 = crescent, 5- square multimask, 6- mask from file}
sourceIndex = 3;  % {1 = exclude mask, 2 = circle, 3 = border around mask}
borderSize  = 30; % border size (in pixels) for sourceIndex == 3

%%%%%%%%%%%%%%
koff=0
%koff=0  RMSE only
%koff=1  LBP only
%%%%%%%%%%%%%%
%%%%%%%%%%%%%%
CUT=0
%CUT=0  без сшивки
%CUT=1  с сшивкой
%%%%%%%%%%%%%%

%%%%%%%%%%%%%%
poisk=0
%poisk=0  EBM
%poisk=1  Markov
%poisk=2  STD
%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part I: Select the test image.

% Select the test image.
switch imageIndex

   % Step function.
   case 1
      nrows = 120;
      ncols = 120;
      [X,Y] = meshgrid(1:ncols,1:nrows);
      R = 0.2*ones(nrows,ncols,1);
      G = 0.65*ones(nrows,ncols,1);
      B = 0.2*ones(nrows,ncols,1);
      R(Y <= ncols/2) = 0.7;
      G(Y <= ncols/2) = 0.7;
      B(Y <= ncols/2) = 0.95;
      I = cat(3,R,G,B);
      I = I + repmat(0.01*randn(ncols,nrows),[1 1 3]);
      I(I < 0) = 0;
      I(I > 1) = 1;
      
   
   % Texture sample.  
   otherwise
      I = im2double(imread('texture (1).png'));
      II=I;
      nrows = size(I,1);
      ncols = size(I,2);
   
end

% Evaluate pixel coordinates.
x = 1:ncols;
y = 1:nrows;
[X,Y] = meshgrid(x,y);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part II: Select the inpainting mask.

% Select the inpainting mask.
switch maskIndex
   
   % Circular mask.
   case 1
      M = sqrt((X-(ncols/2)).^2 + (-45+Y-(nrows/2)).^2) < (1/10)*nrows;
      
   % Double-circle mask.
   case 2
      M = (sqrt((X-(ncols/2)).^2 + (Y-(1*nrows/4)).^2) < (1/6)*nrows) | ...
          (sqrt((X-(ncols/2)).^2 + (Y-(3*nrows/4)).^2) < (1/6)*nrows);
      
   % Square mask.
   case 3
      M = (abs(X-(ncols/2)) < ncols/6) & (abs(Y-(nrows/2))) < nrows/6;
      
   % Crescent mask.
   case 4
     M = (sqrt((X-(ncols/2)).^2 + (Y-(nrows/2)-0.20*size(Y,1)).^2) < 1.2*(1/3)*nrows) & ...
          (sqrt((X-(ncols/2)).^2 + (Y-(3*nrows/4)-0.30*size(Y,1)).^2) > 1.3*(2/5)*nrows);
      
   case 5
    M(1:ncols,1:nrows)=0;
    HSIZE=15;
%     Step_ncols=[ncols/8-HSIZE:ncols/8+HSIZE 2*ncols/8-HSIZE:2*ncols/8+HSIZE 3*ncols/8-HSIZE:3*ncols/8+HSIZE 4*ncols/8-HSIZE:4*ncols/8+HSIZE 5*ncols/8-HSIZE:5*ncols/8+HSIZE 6*ncols/8-HSIZE:6*ncols/8+HSIZE 7*ncols/8-HSIZE:7*ncols/8+HSIZE];
%     Step_nrows=[nrows/8-HSIZE:nrows/8+HSIZE 2*nrows/8-HSIZE:2*nrows/8+HSIZE 3*nrows/8-HSIZE:3*nrows/8+HSIZE 4*nrows/8-HSIZE:4*nrows/8+HSIZE 5*nrows/8-HSIZE:5*nrows/8+HSIZE 6*nrows/8-HSIZE:6*nrows/8+HSIZE 7*nrows/8-HSIZE:7*nrows/8+HSIZE];
   
    Step_ncols=[ncols/8-HSIZE:ncols/8+HSIZE 4*ncols/8-HSIZE:4*ncols/8+HSIZE  7*ncols/8-HSIZE:7*ncols/8+HSIZE];
    Step_nrows=[nrows/8-HSIZE:nrows/8+HSIZE 4*nrows/8-HSIZE:4*nrows/8+HSIZE  7*nrows/8-HSIZE:7*nrows/8+HSIZE];
    
    M(Step_ncols,Step_nrows)=1;
    M=logical(M);  
    
    otherwise
    MM = (imread('mask.png')); 
    M = logical(~MM(:,:,2));    
end

Schet1=sum(sum(M)); 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part III: Select the inpainting source region(s).

% Select the source region(s).
switch sourceIndex
   
   % Everything except the masked region(s).
   case 1
      S = ~M;
      
   % Circular region.
   case 2
      S = sqrt((X-(4*ncols/5)).^2 + (Y-(nrows/2)).^2) < (1/6)*nrows;
      
   % Border around masked region(s).
   otherwise
      S = bwdist(M) < borderSize;
      S(M) = 0;

end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part IV: Run exemplar-based image inpainting.

% Convert image to RGB (if grayscale).
if size(I,3) == 1
   I = repmat(I,[1 1 3]);
end

% Run exemplar-based image inpainting.
[aa bb cc]=size(M);

for ii=1:aa
    for jj=1:bb
        if M(ii,jj)==1
            I(ii,jj,1:3)=NaN;
        end
    end
end

[J,C] = inpaint(I,M,S,koff,CUT,Schet1,poisk);

 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part V: Display final inpainting results.

% Display original and inpainted images.
% figure(2); clf;
% set(gcf,'Name','Exemplar-based Image Inpainting Results');
% set(gcf,'NumberTitle','off','DoubleBuffer','on');
% subplot(1,2,1); imagesc(I);
% axis image; set(gca,'YDir','reverse','XTick',[],'YTick',[]);
% subplot(1,2,2); imagesc(J.rgb);
% axis image; set(gca,'YDir','reverse','XTick',[],'YTick',[]);
figure, imshow(II)% неискаженное изображение
figure, imshow(I)% искаженное изображение
figure, imshow(J.rgb)% обработанное изображение

imwrite(J.rgb,'rezult.png')


[error_criteria,Error_labels]=function_Errors(II,J.rgb)


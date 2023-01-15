function match = findmatch(I,S,block,mask,frame,Raz_x,Raz_y, koff, CUT)

% Extract block parameters.
pSize  = (size(block,1)-1)/2;
pIndex = 0:2*pSize;

block1=block;
mask1=mask;

% Store previously-assigned pixels in this block.
% Note: Don't compare past image border or within mask.
border = isnan(frame);
mask   = repmat(~mask & ~border,[1 1 3]);
block  = block(mask);

% Extract indices of blocks in each row and column.
% Note: Accelerates search for small source regions.
blockRows = (1+pSize):(size(I,1)-pSize);
blockCols = (1+pSize):(size(I,2)-pSize);
columnSum = sum(S,2)';

% Perform exhaustive search for closest-matching block.
% Note: Only use blocks entirely within source region(s).

for i = find(columnSum(blockRows))
   for j = find(S(i,blockCols))
      currSource = S(i+pIndex,j+pIndex);
      if all(currSource(:))
         currBlock = I(i+pIndex,j+pIndex,:);
                           
         currError_RMSE(i,j) = sum((block-currBlock(mask)).^2);
       end
   end
end

currError_RMSE=currError_RMSE./max(max(currError_RMSE));

Kr=currError_RMSE;

error = Inf;
for i = find(columnSum(blockRows))
   for j = find(S(i,blockCols))
      currSource = S(i+pIndex,j+pIndex);
       
      if all(currSource(:))
         currBlock = I(i+pIndex,j+pIndex,:);
         currError=Kr(i,j);
         if currError < error
            error = currError;
            match = currBlock;
         end
      end
   end
end


%% patch quilting
if CUT==1
   B1=block1;
   B2=match;
   MM=double(repmat(~mask1,[1 1 3]));
  B1(MM==0)=0;
[L p]=bwlabel((MM(:,:,1)),4);
  if p<2
    [BB,CCC] = mincut(double(B1),double(B2),double(MM));
   match=BB;
  end
   
  for ii=1:pSize*2+1
      for jj=1:pSize*2+1
          if (match(ii,jj,1)==0)&&(match(ii,jj,2)==0)&&(match(ii,jj,3)==0);
              match(ii,jj)=B2(ii,jj);
          end
      end
  end
end 
      
%% Return updated block (i.e., masked pixels from exemplar).
% % Note: Assign border pixels as "NaN".
if CUT==0
 match(mask) = block;
end
match(repmat(border,[1 1 3])) = NaN;


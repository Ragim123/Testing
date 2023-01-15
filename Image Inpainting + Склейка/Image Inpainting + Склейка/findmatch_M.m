function match3 = findmatch_M(I,S,block,mask,frame,Raz_x,Raz_y,CUT)

% Extract block parameters.
pSize  = (size(block,1)-1)/2;
pIndex = 0:2*pSize;

block1=block;
mask1=mask;
match3(1:pSize*2+1,1:pSize*2+1,1:3)=0;
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

for k=1:8
    step=(k-1)*(pSize*2+1)+1;
    error = Inf;
    
    for i = find(columnSum(blockRows))
        for j = find(S(i,blockCols))
            currSource = S(i+pIndex,j+pIndex);
            
            if all(currSource(:))
                currBlock = I(i+pIndex,j+pIndex,:);
                currError=Kr(i,j);
                
                if currError < error
                    error = currError;
                    match(step:pSize*2+step,1:pSize*2+1,1:3)= currBlock;
                    Koord_i(k)=i;
                    Koord_j(k)=j;
                end
                
            end
        end
    end
    
    Kr(Koord_i(k), Koord_j(k))=Inf;
    %      figure, imshow(match(step:pSize*2+step,1:pSize*2+1,1:3))
end




for k=1:8
    step=(k-1)*(pSize*2+1)+1;
    error = Inf;
    block2=match(step:pSize*2+step,1:pSize*2+1,1:3);
    
    for i = find(columnSum(blockRows))
        for j = find(S(i,blockCols))
            currSource =S(i+pIndex,j+pIndex,:);
            if all(currSource(:))
                currBlock = I(i+pIndex,j+pIndex,:);
                currError_RMSE2(i,j) =sum(sum(sum((block2-currBlock).^2)));
                
            end
        end
    end
    
    currError_RMSE2=currError_RMSE2./max(max(currError_RMSE2));
    
    Kr1=currError_RMSE2;
    
    Kr1(Koord_i(k), Koord_j(k))=Inf;
    
    for i = find(columnSum(blockRows))
        for j = find(S(i,blockCols))
            currSource = S(i+pIndex,j+pIndex,:);
            
            if all(currSource(:))
                currBlock = I(i+pIndex,j+pIndex,:);
                currError=Kr1(i,j);
                
                if currError < error
                    error = currError;
                    match2(step:pSize*2+step,1:pSize*2+1,1:3)= currBlock;
                    Koord_ii(k)=i;
                    Koord_jj(k)=j;
    
                end
                
            end
        end
    end
    
    
    %      figure, imshow(match2(step:pSize*2+step,1:pSize*2+1,1:3))
end

P(1:8)=1;

for k=1:8
    step=(k-1)*(pSize*2+1)+1;
    P1=match(step:pSize*2+step,1:pSize*2+1,1:3);
    P2=match2(step:pSize*2+step,1:pSize*2+1,1:3);
    
    for kk=1:8
        if kk~=k
        step2=(kk-1)*(pSize*2+1)+1;
        P22=match2(step2:pSize*2+step2,1:pSize*2+1,1:3);
        P(k)=P(k)*(sum(sum(sum(P2-P22).^2))).*(sum(sum(sum(P1-P2).^2)));
        end
        end
    
end

Max=inf;
K_Max=1;
for k=1:8
    if P(k)<Max
        Max=P(k);
        K_Max=k;
    end
    
end

K_Max
step=(K_Max-1)*(pSize*2+1)+1;

% step=1;

match3=match(step:pSize*2+step,1:pSize*2+1,1:3);


%% patch quilting

if CUT==1
    
    B1=block1;
    B2=match3;
    MM=double(repmat(~mask1,[1 1 3]));
    B1(MM==0)=0;
    [L p]=bwlabel((MM(:,:,1)),4);
    if p<2
        [BB,CCC] = mincut(double(B1),double(B2),double(MM));
        match3=BB;
    end
    
    for ii=1:pSize*2+1
        for jj=1:pSize*2+1
            if (match3(ii,jj,1)==0)&&(match3(ii,jj,2)==0)&&(match3(ii,jj,3)==0);
                match3(ii,jj)=B2(ii,jj);
            end
        end
    end
    
end

%% Return updated block (i.e., masked pixels from exemplar).
% % Note: Assign border pixels as "NaN".
if CUT==0
    %     block1  = block1(mask);
    match3(mask) = block;
    
    % match3 = mask.*block1+(~mask).*match3;
    
end
match3(repmat(border,[1 1 3])) = NaN;


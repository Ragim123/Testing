function [BB,CCC] = mincut(B1,B2,MM)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Part I: Evaluate overlap error and extract horizontal/vertical regions.

% Evaluate overlap error (using L2 norm).
E = MM(:,:,1).*sum((B1-B2).^2,3);
[a b c]=size(B2);
sred=(sum(sum(E)))/(a*b);
BB=B2;
for i=1:a
    for j=1:b
        if E(i,j)>sred
            BB(i,j,1:3)=B1(i,j,1:3);
        end 
    end
end
CCC=0;
% MM=~MM;
% % Extract horizontal and vertical overlap regions.
% if MM(1,end,1) == 1
%    blockOverlap(1) = find(MM(:,end,1),1,'last');
%    H = E(1:blockOverlap(1),:);
% else
%    H = [];
% end
% if MM(end,1,1) == 1
%    blockOverlap(2) = find(MM(end,:,1),1,'last');
%    V = E(:,1:blockOverlap(2));
% else
%    V = [];
% end
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % Part II: minimum-error horizontal boundary cut (if necessary).
% 
% % Evaluate minimum-error horizontal boundary cut.
% if ~isempty(H)
%    
%    % Recursively compute path costs.
%    CH = zeros(size(H));
%    CH(:,end) = H(:,end);
%    for j = (size(H,2)-1):-1:1
%       for i = 1:size(H,1)
%          if i == 1
%             CH(i,j) = H(i,j) + min(CH(i+(0:1),j+1));
%          elseif i == size(H,1)
%             CH(i,j) = H(i,j) + min(CH(i+(-1:0),j+1));
%          else
%             CH(i,j) = H(i,j) + min(CH(i+(-1:1),j+1));
%          end
%       end
%    end
%    
% end % End of horizontal boundary cut.
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % Part III: minimum-error vertical boundary cut (if necessary).
% 
% % Evaluate minimum-error vertical boundary cut.
% if ~isempty(V)
%    
%    % Recursively compute path costs.
%    CV = zeros(size(V));
%    CV(end,:) = V(end,:);
%    for i = (size(V,1)-1):-1:1
%       for j = 1:size(V,2)
%          if j == 1
%             CV(i,j) = V(i,j) + min(CV(i+1,j+(0:1)));
%          elseif j == size(V,2)
%             CV(i,j) = V(i,j) + min(CV(i+1,j+(-1:0)));
%          else
%             CV(i,j) = V(i,j) + min(CV(i+1,j+(-1:1)));
%          end
%       end
%    end
%    
% end % End of vertical boundary cut.
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % Part IV: Evaluate total minimum-error boundary cut (if necessary).
% 
% % Allocate storage for image mask (i.e., filled boundaries).
% CCC = false(size(E));
% MM = false(size(B1));
% 
% % Determine total minimum-error boundary cut. 
% % Case 1: Only horizonal overlap (e.g., along first scan column).
% if (~isempty(H) && isempty(V))
% 
%    % Trace cost matrix to determine boundary cut.
%    [ignore,i] = min(CH(:,1));
%    CCC(i,1) = 1; MM(i:end,1,:) = 1;
%    for j = 2:size(H,2)
%       if i == 1
%          [ignore,i] = min(CH(i+(0:1),j));
%       elseif i == size(H,1)
%          [ignore,di] = min(CH(i+(-1:0),j));
%          i = i+(di-2);
%       else
%          [ignore,di] = min(CH(i+(-1:1),j));
%          i = i+(di-2);
%       end
%      CCC(i,j) = 1; MM(i:end,j,:) = 1;
%    end
%    
% % Case 2: Only vertical overlap (e.g., along first scan row).
% elseif (isempty(H) && ~isempty(V))
% 
%    % Trace cost matrix to determine boundary cut.
%    [ignore,j] = min(CV(1,:));
%    CCC(1,j) = 1; MM(1,j:end,:) = 1;
%    for i = 2:size(V,1)
%       if j == 1
%          [ignore,j] = min(CV(i,j+(0:1)));
%       elseif j == size(V,2)
%          [ignore,dj] = min(CV(i,j+(-1:0)));
%          j = j+(dj-2);
%       else
%          [ignore,dj] = min(CV(i,j+(-1:1)));
%          j = j+(dj-2);
%       end
%       CCC(i,j) = 1; MM(i,j:end,:) = 1;
%    end
%    
% % Case 3: Both vertical and horizontal overlaps.
% else
%    
%    % Determine pixel where vertical/horizontal cuts intersect.
%    CC = CH(1:blockOverlap(1),1:blockOverlap(2)) + ...
%         CV(1:blockOverlap(1),1:blockOverlap(2)) - ...
%         E(1:blockOverlap(1),1:blockOverlap(2));
%      
%    % Invert pixel assignment mask.
%    MM = ~MM;
%    
%    % Trace back from intersection to determine horizontal boundary cut.
%    [ignore,p] = min(CC(:)); [i,j] = ind2sub(blockOverlap,p); 
%    CCC(i,j) = 1; MM(1:i,j,:) = 0;
%    for j = (j+1):size(H,2)
%       if i == 1
%          [ignore,i] = min(CH(i+(0:1),j));
%       elseif i == size(H,1)
%          [ignore,di] = min(CH(i+(-1:0),j));
%          i = i+(di-2);
%       else
%          [ignore,di] = min(CH(i+(-1:1),j));
%          i = i+(di-2);
%       end
%       CCC(i,j) = 1; MM(1:i,j,:) = 0;
%    end
%    
%    % Trace back from intersection to determine vertical boundary cut.
%    [ignore,p] = min(CC(:)); [i,j] = ind2sub(blockOverlap,p); 
%    CCC(i,j) = 1; MM(i,1:j,:) = 0;
%    for i = (i+1):size(V,1)
%       if j == 1
%          [ignore,j] = min(CV(i,j+(0:1)));
%       elseif j == size(V,2)
%          [ignore,dj] = min(CV(i,j+(-1:0)));
%          j = j+(dj-2);
%       else
%          [ignore,dj] = min(CV(i,j+(-1:1)));
%          j = j+(dj-2);
%       end
%       CCC(i,j) = 1; MM(i,1:j,:) = 0;
%    end
%    
%    % Fill boundary cut to determine per-pixel assignments.
%    [ignore,p] = min(CC(:)); [i,j] = ind2sub(blockOverlap,p);
%    MM(1:(i-1),1:(j-1),:) = 0;
%    
% end
% 
% % Assign values from input blocks using boundary cut.
% BB = B1; BB(MM) = B2(MM);
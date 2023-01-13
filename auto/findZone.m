function [ZoneRect]=findZone(X)

breakFunc = 0;
for i = 1:1:size(X,1)
    for j = 1:1:size(X,2)
        if (X(i,j)==0) && (breakFunc==0)
            PointXY1 = [i j]; %i
            breakFunc = 1;
        end
    end
end
breakFunc = 0;
for i = size(X,1):-1:1
    for j = size(X,2):-1:1
        if (X(i,j)==0) && (breakFunc==0)
            PointXY2 = [i j]; %i
            breakFunc = 1;
        end
    end
end
breakFunc = 0;
for i = 1:1:size(X,1)
    for j = 1:1:size(X,2)
        if (X(j,i)==0) && (breakFunc==0)
            PointXY3 = [i j]; %j
            breakFunc = 1;
        end
    end
end
breakFunc = 0;
for i = size(X,1):-1:1
    for j = size(X,2):-1:1
        if (X(j,i)==0) && (breakFunc==0)
            PointXY4 = [i j]; %j
            breakFunc = 1;
        end
    end
end

Vector1I = [PointXY1(1,1) PointXY2(1,1)];
Vector1J = [PointXY3(1,1) PointXY4(1,1)];

Vector2I = [PointXY1(1,2) PointXY2(1,2)];
Vector2J = [PointXY3(1,2) PointXY4(1,2)];

ZoneRectXY1_I = min(min(Vector1I));
ZoneRectXY1_J = min(min(Vector1J));
ZoneRectXY2_I = max(max(Vector1I));
ZoneRectXY2_J = max(max(Vector1J));

ZoneRect = [ZoneRectXY1_I ZoneRectXY1_J;ZoneRectXY2_I ZoneRectXY2_J];

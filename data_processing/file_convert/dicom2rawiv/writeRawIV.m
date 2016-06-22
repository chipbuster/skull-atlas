% function writeRawIV(data,filename)
%
% Takes 3d array and writes in rawiv format, for use in VolRover.
%   Author:     Louis Ly
%   Email:      louis@ices.utexas.edu
%   Created:    Aug 08, 2015
%   Version:    1.0


function writeRawIV(data,filename, zspacing)

fid = fopen(filename,'w');
[X,Y,Z] = size(data); %could be flipped


minX = 1;
minY = 1;
minZ = 1;
maxX = X;
maxY = Y;
maxZ = 1 + (Z-1)*zspacing;
dimX = X;
dimY = Y;
dimZ = Z;
numVerts = dimX * dimY *dimZ;
numCells = (dimX - 1) * (dimY - 1) * (dimZ - 1);
originX = 1;
originY = 1;
originZ = 1;
spanX = (maxX - minX)/(dimX - 1);
spanY = (maxY - minY)/(dimY - 1);
spanZ = (maxZ - minZ)/(dimZ - 1);

header = [minX; minY; minZ; maxX; maxY; maxZ; numVerts; numCells; ...
          dimX; dimY; dimZ; originX; originY; originZ; spanX; spanY; spanZ];
      
format = 'ieee-be';
fwrite(fid, header(1:6), 'single', 0, format);
fwrite(fid, header(7:11), 'uint32', 0, format);
fwrite(fid, header(12:17), 'single', 0, format);
fwrite(fid, data, 'single', 0, format);
fclose(fid);

end
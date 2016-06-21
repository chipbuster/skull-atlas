close all;
clear;
clc;

N = 920; %sample size
X = 512; %Xsize
Y = 512; %Ysize

% dataf = zeros(X,Y,N);
%     
% for i = 1:N
%     filename = ['/net/thalamus/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
%     data = dicomread(filename);
%     if(isempty(data))
%         data = zeros(X,Y);
%     end
%     dataf(:,:,1,i) = data;
% end    
%%

info = dicominfo('00000242');
image = dicomread('00000242');
imshow(image,[]);

%%

nRows = info.Rows;
nCols = info.Columns;
nPlanes = info.SamplesPerPixel;
nSliceNum = info.InstanceNumber;
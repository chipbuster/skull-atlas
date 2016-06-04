close all;
clear;
clc;

N = 920; %sample size
X = 512; %Xsize
Y = 512; %Ysize

dataf = zeros(X,Y,10);
    j=1; k=1;
for i = N:-1:1
    i
    filename = ['/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
    data = dicomread(filename);
    info = dicominfo(filename);
    if(isempty(data))
        data = zeros(X,Y);
    end
    
    dataf(:,:,j) = data;
    if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
        rawname = [int2str(k) 'female17.rawiv'];
        writeRawIV(dataf,rawname);
        j = 0;
        k = k+1
        dataf = zeros(X,Y,10);
    end
    j = j+1;
end    

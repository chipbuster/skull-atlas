close all;
clear;
clc;

N = 920; %sample size
X = 512; %Xsize
Y = 512; %Ysize

dataf = zeros(X,Y,10); %initialize slice storage matrix
j=1; %slice counter
k=1; %pose counter

%all slices for a pose start at 1 and goes forward
for i = N:-1:1 %iterate slices in backwards order
    i %check progress
    filename = ['/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
    data = dicomread(filename); %read data
    info = dicominfo(filename); %read metadata
    
    if(isempty(data)) %for slices whose data is missing
        data = zeros(X,Y); %replace them with zeros for now
    end
    
    dataf(:,:,j) = data;
    
    %if slice number exists and it is the first slice, store it as a pose
    if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
        rawname = [int2str(k) 'female17.rawiv'];
        writeRawIV(dataf,rawname); %write to rawiv
        j = 0; %reset slice counter
        k = k+1; %update pose counter
        dataf = zeros(X,Y,10); %reset slice storage matrix
    end
    j = j+1; %update slice counter
end    

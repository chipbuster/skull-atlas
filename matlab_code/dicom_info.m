close all;
clear;
clc;

N = 920; %sample size
X = 512; %Xsize
Y = 512; %Ysize

slice_var = zeros(N,2);

for i = 1:N
    filename = ['/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
    %data = dicomread(filename);
    info = dicominfo(filename);
    slice_var(i,1) = i;
    if(~isfield(info, 'InstanceNumber'))
        slice_var(i,2) = 1000;
        info.InstanceNumber
    else
        slice_var(i,2) = info.InstanceNumber;
    end
end    

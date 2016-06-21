close all;
clear;
clc;

load 'sampskull' %load data mat

X = samp;

figure; %histogram of original data
hist(reshape(X,numel(X),1),200);

%% do 3D wavelet decomposition

n = 1; %choose decomposition level
w = 'db1'; %choose wavelet type $XXX-Y$, X-wavelet family,Y-subfamily
WT = wavedec3(X,n,w); % Multilevel 3D wavelet decomposition.

%% if individual layers needs to be seen

A = cell(1,n);
D = cell(1,n);
for k = 1:n
    A{k} = waverec3(WT,'a',k); % Approximations (low-pass components)
    D{k} = waverec3(WT,'d',k); % Details (high-pass components)
end

%% save the lowest level decomposition into rawiv

% remove scaling factor of ((1/sqrt(2))^(3*n)) to preserve energy

imgx = ((1/sqrt(2))^(3*n)).*cell2mat(WT.dec(1,1)); %replace x with level
%writeRawIV(imgx,'waveletlvlx.rawiv'); %replace x with level

figure; %histogram of thumbnail data
hist(reshape(img2,numel(img2),1),200);

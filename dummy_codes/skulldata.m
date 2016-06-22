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
    filename = ['/net/thalamus/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
    data = dicomread(filename);
    info = dicominfo(filename);
    if(isempty(data))
        data = zeros(X,Y);
    end
    
    dataf(:,:,j) = data;
    if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
        rawname = [int2str(k) 'female17.rawiv'];
        %writeRawIV(dataf,rawname);
        samp = dataf;
        j = 0;
        k = k+1;
        dataf = zeros(X,Y,10);
    end
    j = j+1;
end    

%%

% figure;
% imshow(samp(:,:,100),[]);

% save('sampskull.mat','samp');

%load 'sampskull'

X = samp;

figure;
hist(X);

%%

n = 2; %choose decomposition level
w = 'sym4'; %choose wavelet type $XXX-Y$, X-wavelet family,Y-subfamily
WT = wavedec3(X,n,w); % Multilevel 3D wavelet decomposition.

%% if individual layers needs to be seen

A = cell(1,n);
D = cell(1,n);
for k = 1:n
    A{k} = waverec3(WT,'a',k); % Approximations (low-pass components)
    D{k} = waverec3(WT,'d',k); % Details (high-pass components)
end

%% save the lowest level decomposition into rawiv

img2 = cell2mat(WT.dec(1,1));
writeRawIV(img2,'waveletlvl2.rawiv');


%%
figure;
hist(img2);


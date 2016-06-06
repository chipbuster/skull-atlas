close all;
clear;
clc;

% N = 920; %sample size
% X = 512; %Xsize
% Y = 512; %Ysize
% 
% dataf = zeros(X,Y,10);
%     j=1; k=1;
% for i = N:-1:1
%     i
%     filename = ['/net/thalamus/workspace2/Cranial-DMC/Females/17 years old/DICOMOBJ/00000' dec2hex(i,3)];
%     data = dicomread(filename);
%     info = dicominfo(filename);
%     if(isempty(data))
%         data = zeros(X,Y);
%     end
%     
%     dataf(:,:,j) = data;
%     if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
%         rawname = [int2str(k) 'female17.rawiv'];
%         %writeRawIV(dataf,rawname);
%         samp = dataf;
%         j = 0;
%         k = k+1;
%         dataf = zeros(X,Y,10);
%     end
%     j = j+1;
% end    

%%

% figure;
% imshow(samp(:,:,100),[]);

% save('sampskull.mat','samp');

load 'sampskull'

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

%%
% map = pink(90);
% nbIMG = 6;
% idxImages_New = [1 7 10 16 19 25];
% for ik = 1:nbIMG
%     j = idxImages_New(ik);
%     figure('DefaultAxesXTick',[],'DefaultAxesYTick',[],...
%         'DefaultAxesFontSize',8,'Color','w')
%     colormap(map)
%     for k = 1:n
%         labstr = [int2str(k) ' - Z = ' int2str(j)];
%         subplot(2,n,k);
%         image(A{k}(:,:,j));
%         xlabel(['A' labstr]);
%         if k==2
%         	title(['Approximations and details at level 1 to 3 - Slice = ' num2str(j)]);
%         end
%         subplot(2,n,k+n);
%         imagesc(abs(D{k}(:,:,j)));
%         xlabel(['D' labstr]);
%     end
% end

% figure('DefaultAxesXTick',[],'DefaultAxesYTick',[],...
%         'DefaultAxesFontSize',8,'Color','w')
% XR = X;
% Ds = smooth3(XR);
% hiso = patch(isosurface(Ds,5),'FaceColor',[1,.75,.65],'EdgeColor','none');
% hcap = patch(isocaps(XR,5),'FaceColor','interp','EdgeColor','none');
% colormap(map)
% daspect(gca,[1,1,.4])
% lightangle(305,30);
% fig = gcf;
% fig.Renderer = 'zbuffer';
% lighting phong
% isonormals(Ds,hiso)
% hcap.AmbientStrength = .6;
% hiso.SpecularColorReflectance = 0;
% hiso.SpecularExponent = 50;
% ax = gca;
% ax.View = [215,30];
% ax.Box = 'On';
% axis tight
% title('Original Data');
% 
% figure('DefaultAxesXTick',[],'DefaultAxesYTick',[],...
%         'DefaultAxesFontSize',8,'Color','w')
% XR = A{2};
% Ds = smooth3(XR);
% hiso = patch(isosurface(Ds,5),'FaceColor',[1,.75,.65],'EdgeColor','none');
% hcap = patch(isocaps(XR,5),'FaceColor','interp','EdgeColor','none');
% colormap(map)
% daspect(gca,[1,1,.4])
% lightangle(305,30);
% fig = gcf;
% fig.Renderer = 'zbuffer';
% lighting phong
% isonormals(Ds,hiso)
% hcap.AmbientStrength = .6;
% hiso.SpecularColorReflectance = 0;
% hiso.SpecularExponent = 50;
% ax = gca;
% ax.View = [215,30];
% ax.Box = 'On';
% axis tight
% title('Approximation at level 2');
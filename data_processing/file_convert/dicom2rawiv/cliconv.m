function cliconv(slices,locpath,tag)

N = slices; %sample size
X = 512; %Xsize
Y = 512; %Ysize

dataf = zeros(X,Y,10);
    j=1; k=1;
    zspace_init = 0;
for i = N:-1:1
    i
    filename = [locpath '/00000' dec2hex(i,3)];
    data = dicomread(filename);
    info = dicominfo(filename);
    
    if(isfield(info, 'SliceThickness') && isfield(info,'PixelSpacing'))
        zspace = info.SliceThickness;
        zspacing = zspace/info.PixelSpacing(1,1);
    end
    %info.SliceThickness
    %info.SliceLocation
    
    if(isempty(data))
        data = zeros(X,Y);
    end
    %info.SeriesDescription
    dataf(:,:,j) = data;
    
    if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
        rawname = [int2str(k) tag '.rawiv'];
        writeRawIV(dataf,rawname,zspacing);
        j = 0;
        k = k+1;
        dataf = zeros(X,Y,10);
    end
    j = j+1;
end    

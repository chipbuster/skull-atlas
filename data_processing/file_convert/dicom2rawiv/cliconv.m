function cliconv(slices,locpath,tag)

N = slices; %sample size
X = 512; %Xsize
Y = 512; %Ysize

dataf = zeros(X,Y,10); %array for holding data of each pose
dataloc = zeros(3,1); %array for storing position of each pose
dataindex = zeros(3,N); %global array for position error check
j=1; k=1;

for i = N:-1:1
    %i
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

    %check for multichannel data and average them out
    if(isfield(info, 'PhotometricInterpretation'))
        if(strcmp(info.PhotometricInterpretation,'RGB'))
            temp_data = (data(:,:,1) + data(:,:,2) + data(:,:,3))/3;
            data(:,:,1)=temp_data;
            data(:,:,[2,3])=[];
        end
    end
        
    dataf(:,:,j) = data;
    %info.InstanceNumber;
    
    %store patient position
    if(isfield(info, 'ImagePositionPatient'))
        dataloc(:,j) = info.ImagePositionPatient;
        dataindex(:,i) = info.ImagePositionPatient;
    else %if field is not present, intepolate from previous slice
        %info.SliceThickness
        if(j==1)
            dataloc(:,j) = 1;
        else
            dataloc(:,j) = dataloc(:,j-1)+zspace;
        end
    end

    if(isfield(info, 'InstanceNumber') && info.InstanceNumber==1)
        [~,I]=max(var(dataloc,0,2)); %pick position with max variance
        %I
        [~,index] = sort(dataloc(I,:),'descend');  %sort data slices as per index
        dataf = dataf(:,:, index);
        rawname = [int2str(k) 'FC16.rawiv'];
        writeRawIV(dataf,rawname,zspacing);
        
        j = 0;
        k = k+1;
        dataf = zeros(X,Y,1);
        dataloc = zeros(3,1);
    end
    j = j+1;
end    

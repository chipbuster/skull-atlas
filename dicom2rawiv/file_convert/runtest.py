from slices2rawiv import *
from rawivutil import *
import dicomutil
import dicom

path = '/home/chipbuster/CVC/dataskulls/Females/Caucasian/14 years 7-12 months/7633464/DICOMUNC'

print("Reading and binning files...")

files = read_all_dicom_in_dirpath(path)
binned = sep_into_bins(files)

print("Done!")

count=1

for series in binned.values():
    if len(series) < 150:
        continue
    
    try:
        (img,space) = make_one_volimage_with_spacings(series)
        writeRawIV(img,"test%d.rawiv"%(count), space)
        count += 1
    except Exception as e:
        print(e)
        continue


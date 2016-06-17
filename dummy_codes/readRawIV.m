function data = readRawIV(filename)

fid = fopen(filename,'r');

% Here's how it was written:
%format = 'ieee-be';
%fwrite(fid, header(1:6), 'single', 0, format);
%fwrite(fid, header(7:11), 'uint32', 0, format);
%fwrite(fid, header(12:17), 'single', 0, format);
%fwrite(fid, data, 'single', 0, format);
%fclose(fid);

format = 'ieee-be';
header = zeros(17,1);

header(1:6) = fread(fid, 6, 'single', 0, format);
header(7:11) = fread(fid, 5, 'uint32', 0, format);
header(12:17) = fread(fid, 6, 'single', 0, format);
header(9:11)
data = zeros(header(9),header(10),header(11));
data = fread(fid, numel(data), 'single', 0, format);

end

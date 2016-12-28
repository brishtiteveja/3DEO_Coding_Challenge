function [x, y, z, varargout] = stlread(filename)
% This function reads an STL file in binary format into matrixes X, Y and
% Z, and C.  C is optional and contains color rgb data in 5 bits.  
%
% USAGE: [x, y, z, c] = stlread(filename);
%
% To plot use patch(x,y,z,c), or patch(x,y,z)
%
% Written by Doron Harlev modified and some comments added by Mahdi Yoozbashizadeh

if nargout>4
    error('Too many output arguments')
end
%use_color=(nargout==4);

fid=fopen(filename, 'r'); %Open the file, assumes STL Binary format,fid = fopen(filename) opens the file filename for read access. (On PCs, fopen opens files for binary read access.) .
if fid == -1 
    error('File could not be opened, check name or path.')
end

ftitle=fread(fid,80,'uchar=>schar'); % Read file title = fread(fid) reads data in binary format from the file specified by fid into matrix ftitle. 
                                    %Open the file using fopen before calling fread. The fid argument is the integer file identifier obtained 
                                    %from the fopen operation. MATLAB reads the file from beginning to end, and then positions the file pointer 
                                    %at the end of the file (see feof for details).
                                    %A = fread(fid, count) reads the number of elements specified by count. At the end of the fread, MATLAB sets the file pointer to the next byte to be read. A subsequent fread will begin at the location of the file pointer
num_facet=fread(fid,1,'int32'); % Read number of Facets

fprintf('\nTitle: %s\n', char(ftitle'));
fprintf('Num Facets: %d\n', num_facet);

% Preallocate memory to save running time
x=zeros(3,num_facet); y=zeros(3,num_facet); z=zeros(3,num_facet);


%if use_color
  %  c=uint8(zeros(3,num_facet));
%end

h = waitbar(0,'Please wait...');
for i=1:num_facet,
    norm=fread(fid,3,'float32'); % normal coordinates, ignored for now
    ver1=fread(fid,3,'float32'); % vertex 1( for facet i)
    ver2=fread(fid,3,'float32'); % vertex 2
    ver3=fread(fid,3,'float32'); % vertex 3
    col=fread(fid,1,'uint16'); % color bytes
    if (bitget(col,16)==1 & use_color)
        r=bitshift(bitand(2^16-1, col),-10);
        g=bitshift(bitand(2^11-1, col),-5);
        b=bitand(2^6-1, col);
        c(:,i)=[r; g; b];
    end
    x(:,i)=[ver1(1); ver2(1); ver3(1)]; % convert to matlab "patch" compatible format
    y(:,i)=[ver1(2); ver2(2); ver3(2)];
    z(:,i)=[ver1(3); ver2(3); ver3(3)];
    if mod(i,floor(num_facet/10))==0
        waitbar(i/num_facet,h);
    end
end
%if use_color
   % varargout(1)={c};
%end
fclose(fid);
close(h);



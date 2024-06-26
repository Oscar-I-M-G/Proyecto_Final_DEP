% MATLAB script designed to generate assembly files (.s) for implementing IIR filters.
% This script generates assembly code instructions to initialize the filter
% coefficients and structure for IIR filters.

close all
clear all

% Load the filter coefficients from a MATLAB file generated by the fdatool.
% Replace 'IIR_Filter_Coefficients.mat' with the actual filename.

% Variable containing the structure with filter coefficients.
% Replace 'IIR_Coefficients' with the name of the variable containing the filter coefficients.
aux = BP_IIR_60_Butter_5896_385_675

% Calculate the number of coefficients.
num_coef = length(aux.numerator);

% Calculate the amount of memory required to store the coefficients and samples.
% The amount of memory should be a power of 2 greater than num_coef.
c_mem_reserv = 0; % Amount of memory to reserve
while (2^c_mem_reserv) < num_coef
    c_mem_reserv = c_mem_reserv + 1;
end
mem_reserv = 2^(c_mem_reserv + 1);

% Convert the filter coefficients to fixed-point format.
% You may need to adjust the word length and fractional length based on your requirements.
sign = 1; % Sign bit
word_length = 16; % Total word length
fraction_length = 15; % Number of fractional bits
for cc = 1:num_coef
    fract_b_coef(cc) = fi(aux.numerator(cc), sign, word_length, fraction_length);
end

% Convert the fixed-point coefficients to hexadecimal format.
fract_b_coef
fract_b_coef.hex

% Define the path where the assembly file will be saved.
path = '/Users/oimg/Desktop/Filtros/';
file_name=input('Nombre para fichero FIR (sin el .s):','s');
file = strcat(file_name,'.s');
tm = menu('Escoge el tipo de mem. para coef.','X Data space','Program Space') 
fid=fopen(strcat(path,file),'w');
fprintf(fid,';.....\n;    File   %s \n;.....\n\n',file);
fprintf(fid,'    .equ %sNumTaps, %u\n\n',file_name,num_coef);
fprintf(fid,';.....\n; Allocate and initialize filter taps\n\n');
if (tm==1)
    fprintf(fid,'		.section .xdata\n');
else
    fprintf(fid,'		.section .%sconst, "x"\n',file_name);
end
fprintf(fid,'		.align %u\n',mem_reserv);
fprintf(fid,'%sTaps:',file_name);
for cc=1:num_coef
    if(rem(cc,9)==1)
        fprintf(fid,'\n.hword ');
    end
    if((rem(cc,9)==0)||(cc==num_coef))
        fprintf(fid,'0x%s ',hex(fract_b_coef(cc)));
    else    
        fprintf(fid,'0x%s, ',hex(fract_b_coef(cc)));
    end    
end
fprintf(fid,'\n\n;.....\n; Allocate delay line in (uninitialized) Y data space\n\n');
fprintf(fid,'		.section .ybss,  "b"\n');
fprintf(fid,'		.align %u\n\n',mem_reserv);
fprintf(fid,'%sDelay:\n',file_name);
fprintf(fid,'		.space %sNumTaps*2\n\n',file_name);
fprintf(fid,'; .....\n; Allocate and intialize filter structure\n\n');
fprintf(fid,'		.section .data\n');
fprintf(fid,'		.global _%sFilter\n\n',file_name);
fprintf(fid,'_%sFilter:\n',file_name);
fprintf(fid,'.hword %sNumTaps\n',file_name);
if (tm==1)
    fprintf(fid,'.hword %sTaps\n',file_name);
    fprintf(fid,'.hword %sTaps+%sNumTaps*2-1\n',file_name,file_name);
    fprintf(fid,'.hword 0xff00\n');
else
    fprintf(fid,'.hword psvoffset(%sTaps)\n',file_name);
    fprintf(fid,'.hword psvoffset(%sTaps)+%sNumTaps*2-1\n',file_name,file_name);
    fprintf(fid,'.hword psvpage(%sTaps)\n',file_name);
end   
fprintf(fid,'.hword %sDelay\n',file_name);
fprintf(fid,'.hword %sDelay+%sNumTaps*2-1\n',file_name,file_name);
fprintf(fid,'.hword %sDelay\n',file_name);
fprintf(fid,'\n; ....................................\n; ....................................\n');
fclose(fid);

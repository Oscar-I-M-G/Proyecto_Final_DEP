%SCRIPT de matlab realizado para generar archivos ".s", que ser�n usados
%para la implementaci�n de filtros tipo FIR.

close all
clear all

%Variable auxiliar igual al nombre del archivo generado por la fdatool
%Este script llama al archivo fpbanda2.m que contiene una funci�n, que
%devuelve una estructura en la que se incluyen los coeficientes
%calculados. Se encuentran en una variable llamada "num" en el campo
%"numerator"
aux= BP_FIR_50_Hamming_11792_710_750


%Se calcula el n�mero de coeficientes, que ha de ser (forder+1)
num_coef = length(aux.numerator)

%Se calcula la cantidad de memoria que hay que reservar para los
%coeficientes y para las muestras. Dicha cantidad ha de ser una potencia de
%2 mayor que num_coef
c_mem_reserv=0; %Cantidad de memoria a reservar
while (2^c_mem_reserv)<num_coef
    c_mem_reserv=c_mem_reserv+1;
end
mem_reserv=2^(c_mem_reserv+1);

%Ahora se construye el n�mero iq
%Para ello se usa una funci�n fi(v,s,w,f) donde se devuelve un objeto de punto fijo (fixed-point object)
%con valor v, de signo s, de longitud de palabra w, y de longitud fraccional f. 

% I1Q15
sign=1;
word_length=16;
fraction_length=15;
for cc=1:num_coef
    fract_b_coef(cc)=fi(aux.numerator(cc),sign,word_length,fraction_length);
end

%Ahora se deben convertir los valores a hexadecimal

fract_b_coef
fract_b_coef.hex

%A partir de aqu�, se establecen los paths para guardar el archivo .s
%generado, as� como tambi�n los fprintf necesarios para que el aspecto del
%archivo .s generado, coincida que el archivo .s que genera el dspicfdlite

%path = 'G:\Universidad\Dispositivos Electr�nicos Programables\DSPs\PL3\FIR_MatLab\';
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





    






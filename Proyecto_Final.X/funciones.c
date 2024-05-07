
#include <math.h>
#include <dsp.h>

float calculate_rms(int16_t* samples, uint8_t num_samples) {
    
    float sum_squared = 0.0;
    float mean_squared=0.0;
    float rms=0.0;
    // Calculate the sum of squares of each sample
    for (uint8_t i = 0; i < num_samples; i++) {
        sum_squared += (float)samples[i] * (float)samples[i];
    }
    
    // Calculate the mean of the squared values
    mean_squared = sum_squared / num_samples;
    
    // Calculate the square root to get the RMS value
    rms = sqrt(mean_squared);
    
    return rms;
}

int16_t calculate_max_voltage(int16_t* samples,uint8_t num_samples){
    int max_voltage = 0;
    for (uint8_t i = 0; i < num_samples;i++){
        if (samples[i] > max_voltage)
            max_voltage = samples[i];
    }
    return max_voltage;
}

int16_t calculate_min_voltage(int16_t* samples,uint8_t num_samples){
    int min_voltage = 4096;
    for (uint8_t i = 0; i < num_samples;i++){
        if (samples[i] < min_voltage)
            min_voltage = (int16_t)samples[i];
    }
    return min_voltage;
}

float calculate_average(int16_t* samples,uint8_t num_samples){
    float sum = 0.0;
    float average = 0.0;
    for (uint8_t i = 0; i < num_samples; i++) {
        sum += (float)samples[i];
    }
    average = sum / (float)num_samples;
    return average;
}

void aplicarFiltroFIR (fractional* buffer,fractional* output, uint8_t num_samples){
    extern FIRStruct PasaBandaFilter;
    FIRDelayInit (&PasaBandaFilter); 
    FIR(num_samples, &output[0], &buffer[0], &PasaBandaFilter);
    return; 
 }

 /*
 void aplicarFiltroIIR(fractional* buffer,fractional* output){
     extern IIRCanonicStruct //Nombre Filtro aqui;
     IIRCanonicInit //(& NOMBRE FILTRO AQUI);
     IIRCanonic(NUM_SAMPLES,&output[0],&buffer[0], ///&FILTER HERE);
 }
 */

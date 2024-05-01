
#include <math.h>

float calculate_rms(uint16_t* samples, uint8_t num_samples) {
    
    float sum_squared = 0.0;
    
    // Calculate the sum of squares of each sample
    for (uint8_t i = 0; i < num_samples; i++) {
        sum_squared += (float)samples[i] * (float)samples[i];
    }
    
    // Calculate the mean of the squared values
    float mean_squared = sum_squared / num_samples;
    
    // Calculate the square root to get the RMS value
    float rms = sqrt(mean_squared);
    
    return rms;
}

uint16_t calculate_max_voltage(uint16_t* samples,uint8_t num_samples){
    uint16_t max_voltage = 0;
    for (uint8_t i = 0; i < num_samples;i++){
        if (samples[i] > max_voltage)
            max_voltage = samples[i];
    }
    return max_voltage;
}

uint16_t calculate_min_voltage(uint16_t* samples,uint8_t num_samples){
    uint16_t min_voltage = 4096;
    for (uint8_t i = 0; i < num_samples;i++){
        if (samples[i] < min_voltage)
            min_voltage = samples[i];
    }
    return min_voltage;
}

float calculate_average(uint16_t* samples,uint8_t num_samples){
    float sum = 0.0;
    float average = 0.0;
    for (uint8_t i = 0; i < num_samples; i++) {
        sum += (float)samples[i];
    }
    average = sum / num_samples;
    return average;
}
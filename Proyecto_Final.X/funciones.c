
#include <math.h>

float calculate_rms(uint16_t* samples, int num_samples) {
    
    float sum_squared = 0.0;
    
    // Calculate the sum of squares of each sample
    for (int i = 0; i < num_samples; i++) {
        sum_squared += (float)samples[i] * (float)samples[i];
    }
    
    // Calculate the mean of the squared values
    float mean_squared = sum_squared / num_samples;
    
    // Calculate the square root to get the RMS value
    float rms = sqrt(mean_squared);
    
    return rms;
}


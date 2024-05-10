/* 
 * File:   funciones.h
 * Author: oimg
 *
 * Created on April 30, 2024, 12:37 PM
 */

#ifndef FUNCIONES_H
#define	FUNCIONES_H

float calculate_rms(int* samples, int num_samples);
uint16_t calculate_max_voltage(uint16_t* samples,uint8_t num_samples);
uint16_t calculate_min_voltage(uint16_t* samples,uint8_t num_samples);
float calculate_average(uint16_t* samples,uint8_t num_samples);
void aplicarFiltroFIR (fractional* buffer,fractional* output, uint8_t num_samples);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCIONES_H */


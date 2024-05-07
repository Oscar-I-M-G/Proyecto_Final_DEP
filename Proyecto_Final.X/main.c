/**
 * OSCAR IVAN MORENO GUTIERREZ
 * PROYECTO FINAL
 *
*/


/**
  Section: Included Files
*/

#include "mcc_generated_files/system.h"
#include "pintar_lcd.h"
#include "mcc_generated_files/pin_manager.h"
#include "stdbool.h"
#include "mcc_generated_files/tmr1.h"
#include "dsp.h"
#include "math.h"
#include "mcc_generated_files/adc1.h"
#include "funciones.h"
#define NUM_SAMPLES 128 //maximo 256 buffer samples entre mas alto mas lento 

// Variables importantes
uint16_t buffer1[NUM_SAMPLES];
fractional buffer1_frac[NUM_SAMPLES];
fractional buffer1_filter[NUM_SAMPLES];
uint16_t buffer2[NUM_SAMPLES];
fractional buffer2_frac[NUM_SAMPLES];
fractional buffer2_filter[NUM_SAMPLES];
uint8_t buffer_move=0;
uint8_t max_min_move =0;
float rms_value_1 = 0;
float rms_value_2 = 0;
uint16_t max_1[NUM_SAMPLES];
uint16_t max_2[NUM_SAMPLES];
uint16_t min_1[NUM_SAMPLES];
uint16_t min_2[NUM_SAMPLES];
float max_average_1=0.0;
float min_average_1=0.0;
float max_average_2=0.0;
float min_average_2=0.0;
float temp1 = 0.0;
float temp2 = 0.0;

 void aplicarFiltroFIR (fractional* buffer,fractional* output){
    extern FIRStruct PasaBandaFilter; //Filtro creado en archivo .s checar el archivooooooo ahi viene al final
    FIRDelayInit (&PasaBandaFilter); //Inicializamos el Filtro
    
    FIR(NUM_SAMPLES, &output[0], &buffer[0], &PasaBandaFilter);
    return; 
 }
 
/**
 * CALLBACKS
 */
/**
 * Timer
 */
char string1[16];
char string2[16];
bool flag_measurements = false;
bool flag_signal = false; // falso para señal 1 verdadero para señal 2
void __attribute__ ((weak)) TMR1_CallBack(void)
{
    static bool flag_menu = true;

    // MENU cambio cada 3 segundos
    if(!flag_measurements){
        if (flag_menu){
            updateStrings("Presiona        ","SWITCH3         ");
        
        }
        else{
            updateStrings("MUESTREO PWM    ","OSCAR I.M.G.    ");
        
        }
        flag_menu = !flag_menu; 
    }else{
        if (flag_signal){
            temp1 = max_average_1 * ((float)3.3/4096);
            temp2 = min_average_1 * ((float)3.3/4096);
            sprintf(string1,"V1:M=%.2f;m=%.2f",temp1,temp2);
            temp1 = rms_value_1 * ((float)3.3/4096);
            sprintf(string2,"RMS:%.2f        ",temp1);

        }else{
            temp1 = max_average_2 * ((float)3.3/4096);
            temp2 = min_average_2 * ((float)3.3/4096);
            sprintf(string1,"V2:M=%.2f;m=%.2f",temp1,temp2);
            temp1 = rms_value_2 * ((float)3.3/4096);
            sprintf(string2,"RMS:%.2f        ",temp1);
        }

        updateStrings(string1,string2);
    }

    GREEN_LED_Toggle();
}


/**
 * Switch 3
 */
void __attribute__ ((weak)) SWITCH_3_CallBack(void)
{
    //TMR1_Stop();
    GREEN_LED_Toggle();
    flag_measurements=true; //activamos la vizualizacion de las medidas
    
}
/**
 * Switch 1 para cambiar la señal medida
 */
void __attribute__ ((weak)) SWITCH_1_CallBack(void)
{
    GREEN_LED_Toggle();
    flag_signal = !flag_signal;
}
/**
 * Potenciometro controla el duty cycle
 */
static uint16_t conversion;
static uint16_t porcentaje;

void __attribute__ ((weak)) SCCP2_COMPARE_CallBack(void)
{
    conversion = ADC1_ConversionResultGet(channel_AN0);
    porcentaje = (uint16_t) (conversion * ((float)1355 / 4095)) ;
    if(porcentaje < 14) // 0.01 duty cycle min
        porcentaje = 14;
    if(porcentaje > 1342) // 0.99 duty cycle max
        porcentaje = 1342;
    CCP1RB = porcentaje;
}


void __attribute__ ((weak)) SCCP3_COMPARE_CallBack(void)
{
    
    buffer1[buffer_move] = ADC1_ConversionResultGet(channel_AN13);
    buffer1_frac[buffer_move] = buffer1[buffer_move]*0x8000;
    buffer2[buffer_move] = ADC1_ConversionResultGet(channel_AN14);
    buffer2_frac[buffer_move] = buffer2[buffer_move]*0x8000;
    if (buffer_move >= NUM_SAMPLES){
        // Aplicacion del filtro FIR
        //if(FILTER_FLAG == 0)
        aplicarFiltroFIR(buffer1_frac,buffer1_filter);
        aplicarFiltroFIR(buffer2_frac,buffer2_filter);
        
        
        
        rms_value_1 = calculate_rms(buffer1,NUM_SAMPLES);
        rms_value_2 = calculate_rms(buffer2,NUM_SAMPLES);
        max_1[max_min_move] = calculate_max_voltage(buffer1,NUM_SAMPLES);
        min_1[max_min_move] = calculate_min_voltage(buffer1,NUM_SAMPLES); 
        max_2[max_min_move] = calculate_max_voltage(buffer2,NUM_SAMPLES);
        min_2[max_min_move] = calculate_min_voltage(buffer2,NUM_SAMPLES);
        buffer_move = 0;
        if(max_min_move >= 16){
            max_average_1 = calculate_average(max_1,16);
            min_average_1 = calculate_average(min_1,16);           
            max_average_2 = calculate_average(max_2,16);
            min_average_2 = calculate_average(min_2,16);
            max_min_move = 0;
        }else{
            max_min_move ++;
        }
    }else{
        buffer_move++;
    }
}

int main(void)
{   
    
    
    SYSTEM_Initialize();
    
    
    //generaonda();
    //filtrar();
    
    
    contrastScreen();
    clearScreen();
    while (1)
    {
        
        drawScreen();
    }
    return 1; 
}



/**
 * OSCAR IVAN MORENO GUTIERREZ
 * PROYECTO FINAL
 *
*/


/**
  Section: Included Files
*/

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/pin_manager.h"
#include "math.h"
#include "dsp.h"
#include "stdbool.h"
#include "pintar_lcd.h"
#include "funciones.h"
#define NUM_SAMPLES 128 //maximo 256 buffer samples entre mas alto mas lento 

// Variables importantes
int imaInt;
float   imafloat;

int16_t buffer[NUM_SAMPLES];
fractional buffer_fractional[NUM_SAMPLES];
fractional buffer_filter[NUM_SAMPLES];
uint8_t buffer_move=0;
uint8_t max_min_move =0;
float rms_value;
int16_t max_values[NUM_SAMPLES];
int16_t min_values[NUM_SAMPLES];
float max_average=0.0;
float min_average=0.0;
float temp1 = 0.0;
float temp2 = 0.0;
char string1[16];
char string2[16];
bool flag_measurements = false;
bool flag_signal = true; // falso para señal 2  verdadero para señal 1
uint8_t filter_type = 0; // tipo de filtro, 0 = sin filtro, 1 = Filtro FIR, 2, Filtro IIR

/**
 * CALLBACKS
 */
/**
 * Timer
 */

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
        temp1 = max_average * ((float)3.3/4096);
        temp2 = min_average * ((float)3.3/4096);
        sprintf(string1,"M%.4f m%.4f",temp1,temp2);
        temp1 = rms_value * ((float)3.3/4096);
        switch(filter_type)
        {
            case 0:
                if(flag_signal)
                    sprintf(string2,"RMS:%.4f V1    ",temp1);
                else
                    sprintf(string2,"RMS:%.4f V2    ",temp1);
                break;
            case 1:
                if(flag_signal)
                    sprintf(string2,"RMS:%.4f V1FIR",temp1);
                else
                    sprintf(string2,"RMS:%.4f V2FIR",temp1); 
                break;
            case 2:
                if(flag_signal)
                    sprintf(string2,"RMS:%.4f V1IIR",temp1);
                else
                    sprintf(string2,"RMS:%.4f V2IIR",temp1);
                break;
            default:
                sprintf(string2,"------QUACK-----",temp1);
                break;
        }
        updateStrings(string1,string2);
    }
    GREEN_LED_Toggle();
}
/**
 * Switch 3 Para activar las medidas
 */
void __attribute__ ((weak)) SWITCH_3_CallBack(void)
{
    GREEN_LED_Toggle();
    flag_measurements=true; //activamos la vizualizacion de las medidas 
}
/**
 * Switch 2 para cambiar el filtro aplicado
 */
void __attribute__ ((weak)) SWITCH_2_CallBack(void)
{
    GREEN_LED_Toggle();
    if(filter_type == 2)
    {
        filter_type = 0;
    }else{
        filter_type ++;
    }
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
    if(flag_signal){
        buffer[buffer_move] = ADC1_ConversionResultGet(channel_AN13);
    }else{
        buffer[buffer_move] = ADC1_ConversionResultGet(channel_AN14);
    }
    if(filter_type > 0)
        buffer_fractional[buffer_move] = buffer[buffer_move] * 100; //
    
    if (buffer_move >= NUM_SAMPLES){
            if(filter_type == 1)
            {
                aplicarFiltroFIR(buffer_fractional,buffer_filter,NUM_SAMPLES);
                for(uint8_t i=0;i<NUM_SAMPLES;i++){
                    buffer[i] = buffer_filter[i] / 100;                    
                }
            }
            if(filter_type == 2)
            {
                //aplicarFiltroIIR
            }
            rms_value = calculate_rms(buffer,NUM_SAMPLES);
            max_values[max_min_move] = calculate_max_voltage(buffer,NUM_SAMPLES);
            min_values[max_min_move] = calculate_min_voltage(buffer,NUM_SAMPLES); 
       
        buffer_move = 0;
        if(max_min_move >= 16){
            max_average = calculate_average(max_values,16);
            min_average = calculate_average(min_values,16);    
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
    contrastScreen();
    clearScreen();
    while (1)
    {
        drawScreen();
    }
    return 1; 
}



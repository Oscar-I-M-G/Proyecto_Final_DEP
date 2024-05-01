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
#define NUM_SAMPLES 32 //maximo 256 buffer samples

// Variables importantes
uint16_t buffer1[NUM_SAMPLES];
uint16_t buffer2[NUM_SAMPLES];
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

/*
fractional entrada[NUM_SAMPLES]; //Buffer entrada
fractional filtrada[NUM_SAMPLES]; //Buffer salida
uint16_t fsample, freca, frecb, frecc;
float sinA[NUM_SAMPLES];
float sinB[NUM_SAMPLES];
float sinC[NUM_SAMPLES];
uint16_t apr =1;
void generaonda(void)
{
    uint16_t n = 0;
 
    fsample = 10000;    // Frecuencia de Muestreo de 10 KHz
    freca   = 	847;    // Senoide de 847 Hz 
    frecb	=	367;    // Senoide de 367 Hz
    frecc 	= 	123;    // Senoide de 123 Hz
    for (n=0; n < NUM_SAMPLES; n++)
    {
      sinA[n] = sin((2*PI*freca*n)/fsample);            // Creamos una senoide por puntos de la frecuencia A
      sinB[n] = sin((2*PI*frecb*n)/fsample);            // Creamos una senoide por puntos de la frecuencia B
      sinC[n] = sin((2*PI*frecc*n)/fsample);            // Creamos una senoide por puntos de la frecuencia C
      entrada[n] = ((sinA[n]+sinB[n]+sinC[n])/3)*0x8000;  // Escalamos el flotante a fraccional
     
    }
    return;
}

 void filtrar (void){
    extern FIRStruct filtroPAFilter; //Filtro creado en archivo .s checar el archivooooooo ahi viene al final
    FIRDelayInit (&filtroPAFilter); //Inicializamos el Filtro
    FIR(NUM_SAMPLES, &filtrada[0], &entrada[0], &filtroPAFilter);
    return;
}
*/

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
            updateStrings("Presiona        ","SWITCH1         ");
        
        }
        else{
            updateStrings("MUESTREO PWM    ","OSCAR I.M.G.    ");
        
        }
        flag_menu = !flag_menu; 
    }else{
        if (flag_signal){
            temp1 = max_average_1 * (3.3/4096);
            temp2 = min_average_1 * (3.3/4096);
            sprintf(string1,"V1:M=%.2f;m=%.2f",temp1,temp2);
            temp1 = rms_value_1 * (3.3/4096);
            sprintf(string2,"RMS:%.2f        ",temp1);

        }else{
            temp1 = max_average_2 * (3.3/4096);
            temp2 = min_average_2 * (3.3/4096);
            sprintf(string1,"V2:M=%.2f;m=%.2f",temp1,temp2);
            temp1 = rms_value_2 * (3.3/4096);
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
 * Switch 1 utilizar para el test
 */
void __attribute__ ((weak)) SWITCH_1_CallBack(void)
{
    //CCP1RB = 600; // no necesarimente tengo que indicar el numero hexadecimal //cambiar el prescaler
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
    buffer2[buffer_move] = ADC1_ConversionResultGet(channel_AN14);
    if (buffer_move >= NUM_SAMPLES){
        buffer_move = 0;
        //rms
        rms_value_1 = calculate_rms(buffer1,NUM_SAMPLES);
        rms_value_2 = calculate_rms(buffer2,NUM_SAMPLES);
        
        max_1[max_min_move] = calculate_max_voltage(buffer1,NUM_SAMPLES);
        min_1[max_min_move] = calculate_min_voltage(buffer1,NUM_SAMPLES); 
        max_2[max_min_move] = calculate_max_voltage(buffer2,NUM_SAMPLES);
        min_2[max_min_move] = calculate_min_voltage(buffer2,NUM_SAMPLES);
        if(max_min_move >= NUM_SAMPLES){

            max_average_1 = calculate_average(max_1,NUM_SAMPLES);
            min_average_1 = calculate_average(min_1,NUM_SAMPLES);           
            max_average_2 = calculate_average(max_2,NUM_SAMPLES);
            min_average_2 = calculate_average(min_2,NUM_SAMPLES);
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
    
    /*
    generaonda();
    filtrar();
    */
    
    contrastScreen();
    clearScreen();
    while (1)
    {
        
        drawScreen();
    }
    return 1; 
}



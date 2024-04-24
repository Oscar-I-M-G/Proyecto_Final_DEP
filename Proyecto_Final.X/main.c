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
/*#define NUM_SAMPLES 256 //Pontencia de 2 el numero de muestras

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
void __attribute__ ((weak)) TMR1_CallBack(void)
{
    static bool flag = true;
    // MENU cambio cada 3 segundos
    if (flag){
        updateStrings("Presiona        ","SWITCH1         ");
        
    }
    else{
        updateStrings("MUESTREO PWM    ","OSCAR I.M.G.    ");
        
    }
    flag = !flag;
}
/**
 * Switch 3
 */
void __attribute__ ((weak)) SWITCH_3_CallBack(void)
{
    TMR1_Stop();
    GREEN_LED_Toggle();
    updateStrings("V1:M=4.32;m=1.32V","V1:rms=4.32V    ");
    
}
/**
 * Switch 1 utilizar para el test
 */
void __attribute__ ((weak)) SWITCH_1_CallBack(void)
{
    CCP1RB = 600; // no necesarimente tengo que indicar el numero hexadecimal //cambiar el prescaler
    GREEN_LED_Toggle();
}
/**
 * Potenciometro controla el duty cycle
 */
uint16_t conversion = 0;
uint16_t porcentaje = 14;

void __attribute__ ((weak)) SCCP2_COMPARE_CallBack(void)
{
    conversion = ADC1_ConversionResultGet(channel_AN0);
    porcentaje = conversion*(1356/4096);
    if(porcentaje < 14) // 0.01 duty cycle
        porcentaje = 14;
    if(porcentaje > 1342) // 0.99 duty cycle
        porcentaje = 1342;
    CCP1RB = porcentaje;
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



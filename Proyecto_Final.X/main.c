/**
<<<<<<< HEAD
 * OSCAR IVAN MORENO GUTIERREZ
 * PROYECTO FINAL
=======
 *
 * OSCAR IVAN MORENO GUTIERREZ
 * Proyecto Final
>>>>>>> main
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "pintar_lcd.h"
#include "mcc_generated_files/pin_manager.h"
#include "stdbool.h"
#include "mcc_generated_files/tmr1.h"

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



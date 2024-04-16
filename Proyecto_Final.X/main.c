/**
 * OSCAR IVAN MORENO GUTIERREZ
 * PROYECTO FINAL
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "pintar_lcd.h"

int main(void)
{
    contrastScreen();
    SYSTEM_Initialize();
    clearScreen();
    while (1)
    {
        drawScreen();
    }
    return 1; 
}



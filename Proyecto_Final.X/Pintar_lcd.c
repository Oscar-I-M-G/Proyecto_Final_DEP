/**
 * Aqui tendremos nuestras funciones y variables para pintar el LCD
 * Here I have my functions and variables to draw on the LCD 
 */
#include "mcc_generated_files/LCDMiniDrivers/lcd.h"
// Row length
#define ROW_LENGTH 16

uint8_t row0_display[ROW_LENGTH] = "";
uint8_t row1_display[ROW_LENGTH] = "";

uint8_t* Prow0 = &row0_display;
uint8_t* Prow1 = &row1_display;

char row0[ROW_LENGTH];
char row1[ROW_LENGTH];

// This prints both lines of the screen 
void drawScreen(){
    lcd_writeString(Prow1, 1);
    lcd_writeString(Prow0, 0);
}
void updateStrings(){
    Prow0 = (uint8_t*) &row0;
    Prow1 = (uint8_t*) &row1;
}

void contrastAndClear(){
    lcd_setContrast(0x20);
    lcd_clearDisplay();
}



/* 
 * File:   pintar_lcd.h
 * Author: oimg
 *
 * Created on April 16, 2024, 11:42 AM
 */

#ifndef PINTAR_LCD_H
#define	PINTAR_LCD_H


void drawScreen(void);
void updateStrings(char row0[16],char row1[16]);
void contrastScreen(void);
void clearScreen(void);






#ifdef	__cplusplus
extern "C" {
#endif


    



#ifdef	__cplusplus
}
#endif

#endif	/* PINTAR_LCD_H */


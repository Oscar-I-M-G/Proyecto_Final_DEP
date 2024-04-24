/**
  SCCP2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    sccp2.c

  @Summary
    This is the generated driver implementation file for the SCCP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for SCCP2. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33CH512MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB             :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "sccp2_compare.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gSCCP2Mode;

/**
  Section: Driver Interface
*/


void SCCP2_COMPARE_Initialize (void)
{
    // CCPON enabled; MOD Dual Edge Compare, Buffered(PWM); CCSEL disabled; CCPSIDL disabled; T32 16 Bit; CCPSLP disabled; TMRPS 1:64; CLKSEL FOSC/2; TMRSYNC disabled; 
    CCP2CON1L = (0x80C5 & 0x7FFF); //Disabling CCPON bit
    //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each Time Base Period Match; SYNC None; OPSSRC Timer Interrupt Event; 
    CCP2CON1H = 0x00;
    //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled; 
    CCP2CON2L = 0x00;
    //ICGSM Level-Sensitive mode; ICSEL IC2; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled; 
    CCP2CON2H = 0x00;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP2CON3H = 0x00;
    //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP2STATL = 0x00;
    //TMR 0; 
    CCP2TMRL = 0x00;
    //TMR 0; 
    CCP2TMRH = 0x00;
    //PR 2498; 
    CCP2PRL = 0x9C2;
    //PR 0; 
    CCP2PRH = 0x00;
    //CMP 0; 
    CCP2RA = 0x00;
    //CMP 1249; 
    CCP2RB = 0x4E1;
    //BUF 0; 
    CCP2BUFL = 0x00;
    //BUF 0; 
    CCP2BUFH = 0x00;

    CCP2CON1Lbits.CCPON = 0x1; //Enabling CCP

    gSCCP2Mode = CCP2CON1Lbits.MOD;
    // Clearing IF flag before enabling the interrupt.
    IFS1bits.CCP2IF = 0;
    // Enabling SCCP2 interrupt.
    IEC1bits.CCP2IE = 1;

}
/*
void __attribute__ ((weak)) SCCP2_COMPARE_CallBack(void)
{
    // Add your custom callback code here
}
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCP2Interrupt ( void )
{
    if(IFS1bits.CCP2IF)
    {
		// SCCP2 COMPARE callback function 
		SCCP2_COMPARE_CallBack();
		
        IFS1bits.CCP2IF = 0;
    }
}
/*
void __attribute__ ((weak)) SCCP2_COMPARE_TimerCallBack(void)
{
    // Add your custom callback code here
}
*/

void SCCP2_COMPARE_TimerTasks( void )
{
    if(IFS1bits.CCT2IF)
    {
		// SCCP2 COMPARE Timer callback function 
		SCCP2_COMPARE_TimerCallBack();
	
        IFS1bits.CCT2IF = 0;
    }
}

void SCCP2_COMPARE_Start( void )
{
    /* Start the Timer */
    CCP2CON1Lbits.CCPON = true;
}

void SCCP2_COMPARE_Stop( void )
{
    /* Start the Timer */
    CCP2CON1Lbits.CCPON = false;
}

void SCCP2_COMPARE_SingleCompare16ValueSet( uint16_t value )
{   
    CCP2RA = value;
}


void SCCP2_COMPARE_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{

    CCP2RA = priVal;

    CCP2RB = secVal;
}

void SCCP2_COMPARE_DualEdgeBufferedConfig( uint16_t priVal, uint16_t secVal )
{

    CCP2RA = priVal;

    CCP2RB = secVal;
}

void SCCP2_COMPARE_CenterAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{

    CCP2RA = priVal;

    CCP2RB = secVal;
}

void SCCP2_COMPARE_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{

    CCP2RA = priVal;

    CCP2RB = secVal;
}

void SCCP2_COMPARE_VariableFrequencyPulseConfig( uint16_t priVal )
{
    CCP2RA = priVal;
}

bool SCCP2_COMPARE_IsCompareCycleComplete( void )
{
    return(IFS1bits.CCP2IF);
}

bool SCCP2_COMPARE_TriggerStatusGet( void )
{
    return( CCP2STATLbits.TRIG );
    
}

void SCCP2_COMPARE_TriggerStatusSet( void )
{
    CCP2STATLbits.TRSET = 1;
}

void SCCP2_COMPARE_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    CCP2STATLbits.TRCLR = 1;
}

bool SCCP2_COMPARE_SingleCompareStatusGet( void )
{
    return( CCP2STATLbits.SCEVT );
}

void SCCP2_COMPARE_SingleCompareStatusClear( void )
{
    /* Clears the trigger status */
    CCP2STATLbits.SCEVT = 0;
    
}
/**
 End of File
*/

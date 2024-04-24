/**
  SCCP1 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    sccp1.c

  @Summary
    This is the generated driver implementation file for the SCCP1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for SCCP1. 
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

#include "sccp1_compare.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gSCCP1Mode;

/**
  Section: Driver Interface
*/


void SCCP1_COMPARE_Initialize (void)
{
    // CCPON enabled; MOD Dual Edge Compare, Buffered(PWM); CCSEL disabled; CCPSIDL disabled; T32 16 Bit; CCPSLP disabled; TMRPS 1:4; CLKSEL FOSC/2; TMRSYNC disabled; 
    CCP1CON1L = (0x8045 & 0x7FFF); //Disabling CCPON bit
    //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each Time Base Period Match; SYNC None; OPSSRC Timer Interrupt Event; 
    CCP1CON1H = 0x00;
    //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled; 
    CCP1CON2L = 0x00;
    //ICGSM Level-Sensitive mode; ICSEL IC1; AUXOUT Disabled; OCAEN enabled; OENSYNC enabled; 
    CCP1CON2H = 0x8100;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP1CON3H = 0x00;
    //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP1STATL = 0x00;
    //TMR 0; 
    CCP1TMRL = 0x00;
    //TMR 0; 
    CCP1TMRH = 0x00;
    //PR 1356; 
    CCP1PRL = 0x54C;
    //PR 0; 
    CCP1PRH = 0x00;
    //CMP 0; 
    CCP1RA = 0x00;
    //CMP 678; 
    CCP1RB = 0x2A6;
    //BUF 0; 
    CCP1BUFL = 0x00;
    //BUF 0; 
    CCP1BUFH = 0x00;

    CCP1CON1Lbits.CCPON = 0x1; //Enabling CCP

    gSCCP1Mode = CCP1CON1Lbits.MOD;
    // Clearing IF flag before enabling the interrupt.
    IFS0bits.CCP1IF = 0;
    // Enabling SCCP1 interrupt.
    IEC0bits.CCP1IE = 1;

    // Clearing IF flag before enabling the interrupt.
    IFS0bits.CCT1IF = 0;
    // Enabling SCCP1 interrupt.
    IEC0bits.CCT1IE = 1;
}

void __attribute__ ((weak)) SCCP1_COMPARE_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCP1Interrupt ( void )
{
    if(IFS0bits.CCP1IF)
    {
		// SCCP1 COMPARE callback function 
		SCCP1_COMPARE_CallBack();
		
        IFS0bits.CCP1IF = 0;
    }
}

void __attribute__ ((weak)) SCCP1_COMPARE_TimerCallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT1Interrupt ( void )
{
    if(IFS0bits.CCT1IF)
    {
		// SCCP1 COMPARE Timer callback function 
		SCCP1_COMPARE_TimerCallBack();
	
        IFS0bits.CCT1IF = 0;
    }
}

void SCCP1_COMPARE_Start( void )
{
    /* Start the Timer */
    CCP1CON1Lbits.CCPON = true;
}

void SCCP1_COMPARE_Stop( void )
{
    /* Start the Timer */
    CCP1CON1Lbits.CCPON = false;
}

void SCCP1_COMPARE_SingleCompare16ValueSet( uint16_t value )
{   
    CCP1RA = value;
}


void SCCP1_COMPARE_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{

    CCP1RA = priVal;

    CCP1RB = secVal;
}

void SCCP1_COMPARE_DualEdgeBufferedConfig( uint16_t priVal, uint16_t secVal )
{

    CCP1RA = priVal;

    CCP1RB = secVal;
}

void SCCP1_COMPARE_CenterAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{

    CCP1RA = priVal;

    CCP1RB = secVal;
}

void SCCP1_COMPARE_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{

    CCP1RA = priVal;

    CCP1RB = secVal;
}

void SCCP1_COMPARE_VariableFrequencyPulseConfig( uint16_t priVal )
{
    CCP1RA = priVal;
}

bool SCCP1_COMPARE_IsCompareCycleComplete( void )
{
    return(IFS0bits.CCP1IF);
}

bool SCCP1_COMPARE_TriggerStatusGet( void )
{
    return( CCP1STATLbits.TRIG );
    
}

void SCCP1_COMPARE_TriggerStatusSet( void )
{
    CCP1STATLbits.TRSET = 1;
}

void SCCP1_COMPARE_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    CCP1STATLbits.TRCLR = 1;
}

bool SCCP1_COMPARE_SingleCompareStatusGet( void )
{
    return( CCP1STATLbits.SCEVT );
}

void SCCP1_COMPARE_SingleCompareStatusClear( void )
{
    /* Clears the trigger status */
    CCP1STATLbits.SCEVT = 0;
    
}
/**
 End of File
*/

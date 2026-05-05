//---------------------------------
// Title: ADC-LCD-Analog Testing w/ Microphone
//---------------------------------
// Author: Jason Downing
// Date: May 2nd, 2026
// Versions: 5
//   V1: LCD + ADC voltage display using potentiometer
//   V2: Added microphone sensor with sound level classification
//   V3: Added IOC interrupt to halt system for 10 seconds with LED indication
// -----------------------------------------------------------
// Setup:   PIC18F47K42 Curiosity Board
// Compiler: xc8, V6.30
// File Dependencies / Libraries: 
//       Config.h, xc8.h
// ------------------------------------------------------------
// Purpose:
//   Interface a microphone sensor with the PIC18F47K42 using the ADC,
//   classify sound levels (Quiet, Normal, Loud, Obnoxious), and display
//   results on an LCD. Implement an Interrupt-On-Change (IOC) button that
//   halts ADC operation for 10 seconds while a red LED blinks.
//---------------------------------------------------------------
// Inputs:
//   RA0 (AN0): Microphone analog signal
//   RA1 (AN1): Potentiometer (optional / previous stage)
//   RC2: Push button (IOC interrupt trigger)
// -----------------------------------------------------------
// Outputs:
//   PORTB: LCD data lines (D0?D7)
//   RD0: LCD RS (Register Select)
//   RD1: LCD EN (Enable)
//   RC3: Red LED (blinks during interrupt halt state)

#include <xc.h>
#include <stdio.h>

// ================= CONFIG BITS =================

#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINTOSC_1MHZ
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = ON
#pragma config CSWEN = ON
#pragma config FCMEN = ON

#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON
#pragma config IVT1WAY = ON
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS

#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF

#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC

#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF

#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config WRTSAF = OFF
#pragma config LVP = ON

#pragma config CP = OFF

// ================= DEFINETIONS =================

#define _XTAL_FREQ 1000000

// LCD connections
#define RS LATD0
#define EN LATD1
#define ldata LATB

#define LCD_Port TRISB
#define LCD_Control TRISD

#define ADC_MAX 4095UL
#define VREF_MV 5000UL

// 
#define QUIET_LIMIT_MV      200UL
#define NORMAL_LIMIT_MV     2300UL
#define LOUD_LIMIT_MV       4500UL

#define RED_LED LATCbits.LATC3
#define RED_LED_TRIS TRISCbits.TRISC3

// ================= GLOBALS =================

unsigned long sound_mv;
char display_data[17];
volatile unsigned char halt_request = 0;

// ================= FUNCTION PROTOTYPES =================

void LCD_Init(void);
void LCD_Command(char cmd);
void LCD_Char(char dat);
void LCD_String(const char *msg);
void LCD_String_xy(char row, char pos, const char *msg);
void LCD_Clear(void);
void MSdelay(unsigned int val);

void ADC_Init(void);
unsigned int ADC_Read(void);
unsigned long Get_Sound_Level_mV(void);
const char* Get_Sound_Category(unsigned long mv);

void __interrupt(irq(default), base(0x6008)) ISR(void);
void IOC_Init(void);
void Halt_State_10s(void);

// ================= MAIN =================

void main(void) {
    OSCFRQ = 0x02; // 4 MHz internal oscillator

    ANSELB = 0x00;
    ANSELD = 0x00;

    LCD_Init();
    ADC_Init();
    IOC_Init();

    while (1) {
        if (halt_request == 1) {
            halt_request = 0;
            Halt_State_10s();
            RED_LED = 0;
        }

        sound_mv = Get_Sound_Level_mV();

        LCD_Clear();

        LCD_String_xy(1, 0, "Sound: ");
        LCD_String(Get_Sound_Category(sound_mv));

        sprintf(display_data, "%lu.%02luV Sound",
                sound_mv / 1000UL,
                (sound_mv % 1000UL) / 10UL);

        LCD_String_xy(2, 0, display_data);

        MSdelay(100);
    }
}


// ================= ADC READ FUNCTION =================

unsigned int ADC_Read(void)
{
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);

    return ((unsigned int)ADRESH << 8) | ADRESL;
}

// ================= MICROPHONE FUNCTIONS =================

unsigned long Get_Sound_Level_mV(void)
{
    unsigned int sample;
    unsigned int min_val = 4095;
    unsigned int max_val = 0;
    unsigned int peak_to_peak;
    unsigned char i;

    ADPCH = 0x00;   // AN0 / RA0 = microphone

    for(i = 0; i < 150; i++)
    {
        sample = ADC_Read();

        if(sample > max_val)
            max_val = sample;

        if(sample < min_val)
            min_val = sample;

        __delay_us(200);
    }

    peak_to_peak = max_val - min_val;

    return ((unsigned long)peak_to_peak * VREF_MV) / ADC_MAX;
}

const char* Get_Sound_Category(unsigned long mv) 
{
    if (mv < QUIET_LIMIT_MV)
        return "Quiet";
    else if (mv < NORMAL_LIMIT_MV)
        return "Normal";
    else if (mv < LOUD_LIMIT_MV)
        return "Loud";
    else
        return "Obnoxious";
}

void IOC_Init(void) {
    INTCON0bits.GIE = 0;

    // RC3 = red LED output
    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 0;
    ODCONCbits.ODCC3 = 0;
    ANSELCbits.ANSELC3 = 0;

    // RC2 = interrupt button input
    TRISCbits.TRISC2 = 1;
    ANSELCbits.ANSELC2 = 0;
    WPUCbits.WPUC2 = 1;
    INLVLCbits.INLVLC2 = 1;
    SLRCONCbits.SLRC2 = 0;
    ODCONCbits.ODCC2 = 0;

    // Interrupt Vector Table base
    IVTBASEU = 0x00;
    IVTBASEH = 0x60;
    IVTBASEL = 0x08;

    // Disable IOC while configuring
    PIE0bits.IOCIE = 0;

    // Clear existing flag
    IOCCFbits.IOCCF2 = 0;

    // Button press/release detection
    IOCCPbits.IOCCP2 = 0;
    IOCCNbits.IOCCN2 = 1;

    // Enable IOC
    PIE0bits.IOCIE = 1;

    // Enable priority + global interrupts
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    INTCON0bits.GIE = 1;
}

void Halt_State_10s(void) {
    unsigned char i;
    unsigned char dummy;

    PIE0bits.IOCIE = 0; // Disable IOC during halt
    halt_request = 0;

    LCD_Clear();
    LCD_String_xy(1, 0, "SYSTEM HALTED");
    LCD_String_xy(2, 0, "ADC stopped");

    for (i = 0; i < 20; i++) {
        RED_LED = 1;
        __delay_ms(250);

        RED_LED = 0;
        __delay_ms(250);
    }

    dummy = PORTC;
    (void) dummy;

    IOCCFbits.IOCCF2 = 0; // Clear button flag
    PIE0bits.IOCIE = 1; // Re-enable IOC

    RED_LED = 0;
    LCD_Clear();
}

//================INTERRUPT FUNCTION=================

void __interrupt(irq(default), base(0x6008)) ISR(void) {
    if (PIR0bits.IOCIF) {
        if (IOCCFbits.IOCCF2) {
            halt_request = 1;

            RED_LED = 1; // immediate feedback
            PIE0bits.IOCIE = 0; // prevent repeated triggers

            IOCCFbits.IOCCF2 = 0;
        }
    }
}

// ================= LCD FUNCTIONS =================

void LCD_Init(void)
{
    MSdelay(15);

    LCD_Port = 0x00;
    LCD_Control = 0x00;

    LCD_Command(0x01);
    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x06);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
}

void LCD_Command(char cmd)
{
    ldata = cmd;
    RS = 0;
    EN = 1;
    NOP();
    EN = 0;
    MSdelay(3);
}

void LCD_Char(char dat)
{
    ldata = dat;
    RS = 1;
    EN = 1;
    NOP();
    EN = 0;
    MSdelay(1);
}

void LCD_String(const char *msg)
{
    while(*msg != '\0')
    {
        LCD_Char(*msg);
        msg++;
    }
}

void LCD_String_xy(char row, char pos, const char *msg)
{
    char location;

    if(row == 1)
        location = 0x80 + pos;
    else
        location = 0xC0 + pos;

    LCD_Command(location);
    LCD_String(msg);
}

// ================= ADC INIT =================

void ADC_Init(void)
{
    // RA0 = microphone input
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSELA0 = 1;

    // RA1 = potentiometer input, still analog
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSELA1 = 1;

    ADPCH = 0x00;          // Default channel AN0 / RA0

    ADCON0bits.FM = 1;     // Right justified
    ADCON0bits.CS = 1;
    ADCLK = 0x00;

    ADPREL = 0x00;
    ADPREH = 0x00;
    ADACQL = 0x00;
    ADACQH = 0x00;

    ADRESH = 0x00;
    ADRESL = 0x00;

    ADCON0bits.ON = 1;
}

// ================= DELAY =================

void MSdelay(unsigned int val)
{
    unsigned int i, j;

    for(i = 0; i < val; i++)
        for(j = 0; j < 165; j++);
}

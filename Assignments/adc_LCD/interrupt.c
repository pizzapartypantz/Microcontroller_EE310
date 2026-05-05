/*
 * The purpose of this program is demonstrate INT0 as an external interrupt. 
 * In this case when the signal changes state on RB0 (INT0) then D0 starts 
 * blinking for 4 seconds and then stops. The results can be simulated and
 * verified. 
 * 
 */


// PIC18F46K42 Configuration Bit Settings

// 'C' source line config statements
#include <xc.h>

#define _XTAL_FREQ 4000000
#define FCY (_XTAL_FREQ/4)

// Defining Interrupt ISR 

void __interrupt(irq(IRQ_INT0), base(0x4008)) INT0_ISR(void)
{
    if (PIR1bits.INT0IF == 1)  // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
    {            // if so, do something
                // e.g,blink an LED connected to  PORTDbits.RD0 for 10 times with a delay of __delay_ms(250)
        
        for (int i = 0; i < 5; i++) 
        {
            LATDbits.LATD0 = 1;
            __delay_ms(.02);
            LATDbits.LATD0 = 0;
            __delay_ms(.02);

        }

        PIR1bits.INT0IF = 0;   // always clear the interrupt flag for INT0 when done
        LATDbits.LATD0 = 0;    // turn off the led on PORTDbits.RD0 
        
    }
}

void INTERRUPT_Initialize(void)
{  // Enable interrupt priority bit in INTCON0 (check INTCON0 register and find the bit)
    INTCON0bits.IPEN = 1;      
    INTCON0bits.GIEH = 1;          // Enable high priority interrupts using bits in INTCON0
    INTCON0bits.GIEL = 1;          // Enable low priority interrupts using bits in INTCON0

    INTCON0bits.INT0EDG = 1;       // Interrupt on rising edge of INT0 pin using bits in INTCON0

    IPR1bits.INT0IP = 1;       // Set the interrup high priority (IP) for INT0 - INT0IP
    PIR1bits.INT0IF = 0; //Clear interrupt flag for INT0
    PIE1bits.INT0IE = 1;       // Enable the interrup (IE) for INT0

    // Change IVTBASE by doign the following
    IVTBASEU = 0x00;            // Set IVTBASEU to 0x00 
    IVTBASEH = 0x40;            // Set IVTBASEH to  0x40;
    IVTBASEL = 0x08;            // Set IVTBASEL to 0x08;
}


void main(void) {
    // Initialization  
    // set port B AS iNPUT and D as outputs 
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISBbits.TRISB0 = 1;

    // enable the weak pull-ups are enabled for port B
    WPUBbits.WPUB0 = 1;
    
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    
    // main code here 
    INTERRUPT_Initialize();  // blink an LED connected to RD1 every 2 seconds
    
    while (1)
    {
        LATDbits.LATD1 = 1;
        __delay_ms(.02);
        LATDbits.LATD1 = 0;
        __delay_ms(.02);
    }
    
}

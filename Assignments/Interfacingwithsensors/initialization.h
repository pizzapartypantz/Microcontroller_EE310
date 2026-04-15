#include <stdbool.h> 
bool correct; //flag control to check whether the system accepts the password

void initialization(void) //initializes variable values and ports 
{ 
    PORTA = 0;  
    LATA = 0;  
    ANSELA = 0; 
    TRISA = 0b00000111;  
    WPUA = 0b00000000;       
    PORTB = 0;  
    LATB = 0;  
    ANSELB = 0b00000000;  
    TRISB = 0b00000011;  
    WPUB = 0b00000000;      
    PORTC = 0;  
    LATC = 0;  
    ANSELC = 0;  
    TRISC = 0b00000000;         
    PORTD = 0;  
    LATD = 0;  
    ANSELD = 0;  
    TRISD = 0b00000000;    
     
    correct = false;              
    count = 0; 
    password = 0; 
    secretPassword = 11; 
     
    INTCON0bits.INT0EDG = 1;         
    INTCON0bits.IPEN = 1;            
    INTCON0bits.GIEH = 1;            
    INTCON0bits.GIEL = 1;            
    IPR1bits.INT0IP = 1;             
    PIE1bits.INT0IE = 1;             
    PIR1bits.INT0IF = 0;             
     
    IVTBASEU = 0x00; 
    IVTBASEH = 0x30; 
    IVTBASEL = 0x08; 
} 

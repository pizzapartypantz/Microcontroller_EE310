#include <stdbool.h> 
bool correct; //Boolean variable to check if input is correct
int count; //variable for digits for 7-segment display
 
//function to hold 7-segment digits
void update7SD(void) {            
    if (count == 0)               
        PORTD = num0;               
    else if (count == 1)
        PORTD = num1;
    else if (count == 2)
        PORTD = num2;
    else if (count == 2)
        PORTD = num2;
    else if (count == 3)
        PORTD = num3;
    else if (count == 4)
        PORTD = num4;
    else if (count == 5)
        PORTD = num5;
    else if (count == 6)
        PORTD = num6;
    else if (count == 7)
        PORTD = num7;
    else if (count == 8)
        PORTD = num8;
    else if (count == 9)
        PORTD = num9;
    return;
}
 
void passwordInput(void) //Get input from user 
{ 
    while(correct==false){ // do this while until correct password entry
        if(PORTAbits.RA0==1){ //If photoresistor is darkened...
            count++; // ... increment count
            update7SD(); // update display(reset)
            __delay_ms(500); // delay so you can see digit on 7-seg
        } 
        if(PORTBbits.RB1==1){   
            correct=true; //correct controls whether the system accepts the password, used as a flag
        } 
    } 
    return; 
} 
 
void passwordCheck(void) //check password 
{ 
    update7SD(); // update display
    if(password == secretPassword){ //if password is correct do this
        PORTAbits.RA5 = 1; // turn on motor                   
        __delay_ms(5000); // run motor for 5 seconds
        PORTAbits.RA5 = 0; // turn off motor  
    } 
    else{ // turn on buzzer 
        PORTBbits.RB2 = 1; // turn on PORT RB2  
        __delay_ms(3000); // Run buzzer for 3 seconds
        PORTBbits.RB2 = 0; // turn off buzzer
    } 
    return;     
} 
 
void __interrupt(irq(IRQ_INT0),base(0x3008)) INT0_ISR(void){ //interrupt function
    if(PIR1bits.INT0IF == 1){  
        PORTBbits.RB2 = 1; //turn on buzzer
        __delay_ms(2000); // run buzzer for 2 seconds
        PORTBbits.RB2 = 0; // turn off buzzer
    } 
    PIR1bits.INT0IF = 0;                 
    return; 
}

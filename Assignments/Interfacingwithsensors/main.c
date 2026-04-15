//-----------------------------
// Title: Interfacingwithsensors
//-----------------------------
// Purpose: Design a safebox with 2 touchless switches (photoresistors). The user must use the touchless switches to enter
//          a secret password and open the box. By shading the photoresistors, it will increment a display to give feedback to 
//          the user. When the desired code is displayed, the user will press a button to confirm code entry. If the code is correct,
//          it will turn on a motor. If the code is false, it will sound a buzzer. 
// Dependencies: 
// - stdbool.h - Boolean Comparisons
// - config.h - Configuration file for PIC18F47K42
// - xc.h - standard library for XC8 compiler
// - initialization.h - file containing initialization function
// - functions.h - file containing all program functions
// Compiler: XC8 v2.46
// Author: Jason Hernandez-Downing
// OUTPUTS: Buzzer(RB2) turns on or Fan motor (RA5) turns on. 7 segment display(RD0:6)
// INPUTS: Secret code via use of photoresistors (RA0:1). Button1(RB1) to confirm code entry. Button2(RB0) for interrupt.
// HARDWARE: PIC18F47K42 microcontroller, qty-2 power supplies, qty-2 photoresistors, qty-2 switches, qty-1 7seg display,
//           qty-1 5V buzzer, qty-1 motor, qty-1 relay module, qty-2 transistors(driver), qty-2 breadboards, various resistors,
//           various wiring. 
// Versions: First and final version

//Declaration of functions
void update7SD(void);
void passwordInput(void);
void passwordCheck(void);
void initialization(void);

//Declaration of variables
int password;
int count; 
int secretPassword = 11; 

#include <stdbool.h> 
#include <xc.h> 
#include "config.h" 
#include "initialization.h" 
#include "functions.h" 

bool correct; 
 
void main(void)  
{ 
    while(1) { //do this indefinitely 
        initialization(); //initialize ports            
        update7SD(); //update display
        while (correct == false) //do this until secretPassword is correct     
            passwordInput(); //call passwordInput function for password entry            
            __delay_ms(250); //key bounce delay
            password = count * 10; //take first input and place in tenths place        
            correct = false; // reset correct for next while loop               
            count = 0; //reset count
            update7SD(); //update display                    
        while (correct == false) //do this until secretPassword is correct       
            passwordInput(); //call passwordInput function for password entry              
            __delay_ms(250); //key bounce delay
            password = password+count; //place second input in 1's place
            passwordCheck(); //call passwordCheck function to check for secretPassword                
    }
}

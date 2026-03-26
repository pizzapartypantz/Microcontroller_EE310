//----------------------------- 
// Title: CounterDesign 
//----------------------------- 
// Dependencies: "MyConfigFile.inc" 
// Compiler: MPLAB X IDE v6.20 Pic-as (v3.0) 
// Author: Jason Hernandez-Downing
// Date: 03/26/2026
// OUTPUTS: RD0-RD6. 7-segment displays digits(0-9)/characters(A-F)
// INPUTS: Button A (RB0) and Button B (RB1)  
// Versions: V6.20: 3/26/2026 - First version  
// Equipment: PIC18F47K42 Curiosity Microcontroller, bread board (BB630), qty-2 100 ohm 
//	      resistors, qty-2 330 ohm resistors, qty-1 common cathode 7-segment
//	      display, power block (3.3V-5V), various wiring
//------------------------------------------------------------------------------
	
;---------------------
; Initialization
;---------------------
#include "./MyConfigFile.inc"
#include <xc.inc>
;---------------------
; Program Inputs
;---------------------
Inner_loop  equ 100 // in decimal
Middle_loop equ 50
Outer_loop  equ 50
 
;---------------------
; Program Constants
;---------------------
REG10   equ     0x14   
REG11   equ     0x15
REG12   equ     0x16
COUNT   equ     0x10

PSECT absdata,abs,ovrld	 ; Do not change
    
ORG          0	         ;Reset vector

   ORG          0x00     ; Store LED hex values starting at 0x00
   
// LOAD VALUES(0-F) IN REGISTERS FOR INDIRECT ADDRESSING(0-F)
   MOVLW 0x3F
   MOVWF 0x00
   
   MOVLW 0x06
   MOVWF 0x01
   
   MOVLW 0x5B
   MOVWF 0x02
   
   MOVLW 0x4F
   MOVWF 0x03
   
   MOVLW 0x66
   MOVWF 0x04
   
   MOVLW 0x6D
   MOVWF 0x05
   
   MOVLW 0x7D
   MOVWF 0x06
   
   MOVLW 0x07
   MOVWF 0x07
   
   MOVLW 0x7F
   MOVWF 0x08
   
   MOVLW 0x67
   MOVWF 0x09
   
   MOVLW 0x77
   MOVWF 0x0A
   
   MOVLW 0x7C
   MOVWF 0x0B
   
   MOVLW 0x39
   MOVWF 0x0C
   
   MOVLW 0x5E
   MOVWF 0x0D
   
   MOVLW 0x79
   MOVWF 0x0E
   
   MOVLW 0x71
   MOVWF 0x0F

 ORG		0040H	; Initiate PORTB and PORTD starting at 0040H
    
    BANKSEL	PORTD			     
    CLRF	PORTD	;Init PORTD
    BANKSEL	LATD	;Data Latch
    CLRF	LATD			     
    BANKSEL	ANSELD			     
    CLRF	ANSELD	;digital I/O
    BANKSEL	TRISD			     
    MOVLW	0b10000000 ;Set RD[6:0] as outputs
    MOVWF	TRISD
    BANKSEL	PORTB			     
    CLRF	PORTB   ;Init PORTB
    BANKSEL	LATB	;Data Latch
    CLRF	LATB			     
    BANKSEL	ANSELB			     
    CLRF	ANSELB	;digital I/O
    BANKSEL	TRISB			     
    MOVLW	0b00000011 ;Set RB1 and RB0 as inputs
    MOVWF	TRISB
      
; Initialize FSR0 to start address
    MOVLW 0x00         ; Start address in RAM
    MOVWF FSR0L
    CLRF FSR0H         ; Assuming address is within bank 0

    MOVLW 0x10         ; Number of bytes to read
    MOVWF COUNT        ; COUNT constant for loop/reset
    
    MOVLW 0x3F         
    MOVWF PORTD        ; Initilize 7-segment with 0 (0x3F is hex value to
                       ; display 0 on 7-segment)
Main_Loop:
  ; ==== Check if both RB0 and RB1 are HIGH ====
    BTFSS PORTB, 0      ; Is RB0 HIGH?
    GOTO Check_Buttons  ; If not, skip to Check_Buttons

    BTFSS PORTB, 1      ; Is RB1 HIGH?
    GOTO Check_Buttons  ; If not, skip to see if both buttons pressed

; If both buttons pressed reset 7-segment to 0
    MOVLW 0x00
    MOVWF FSR0L
    CLRF FSR0H
    MOVF INDF0, W
    MOVWF PORTD
    CALL loopDelay
    GOTO Main_Loop

Check_Buttons:
    BTFSS PORTB, 0       ; Check RB0 for COUNT_UP
    GOTO CHECK_B
    GOTO COUNT_UP
    
CHECK_B: 
    BTFSS PORTB, 1
    GOTO Main_Loop
    GOTO COUNT_DOWN
; If RB0 is HIGH (Button A pressed), continue here
COUNT_UP:
    INCF FSR0L, F        ; Next address
    BCF  FSR0L, 4	 ; Clear bit 4  
    BCF  FSR0L, 5        ; Clear bit 5 
    BCF  FSR0L, 6        ; Clear bit 6 
    BCF  FSR0L, 7        ; Clear bit 7. Clear bits so count stays in range (0-F)
    MOVF INDF0, W        ; Load from RAM using FSR0
    MOVWF PORTD          ; Output to 7-segment
    CALL loopDelay       ; Delay function
    DECFSZ COUNT, F      ; Decrement loop counter to maintain proper index
    GOTO Main_Loop       ; Go back to check RB0 and continue only if HIGH

; When COUNT reaches 0 reset FSR0L and COUNT to restart from beginning

    MOVLW 0x00
    MOVWF FSR0L         ; Reset pointer to beginning of RAM

    MOVLW 0x10
    MOVWF COUNT         ; Reset count to 16

    GOTO Main_Loop      ; Loop again

Done:
    GOTO Done          

COUNT_DOWN:
    DECF FSR0L, F        ; Next address
    BCF  FSR0L, 4        ; Clear bit 4  
    BCF  FSR0L, 5        ; Clear bit 5
    BCF  FSR0L, 6        ; Clear bit 6 
    BCF  FSR0L, 7        ; Clear bit 7. Clear bits so count stays in range (0-F)
    MOVF INDF0, W        ; Load from RAM using FSR0
    MOVWF PORTD          ; Output to 7-segment
    CALL loopDelay       ; Delay function
    INCFSZ COUNT, F      ; Increment loop counter to maintain proper index
    GOTO Main_Loop       ; Go back to check RB0 and continue only if HIGH

; When COUNT reaches 0 reset FSR0L and COUNT to restart from beginning

    MOVLW 0x00
    MOVWF FSR0L         ; Reset pointer to beginning of RAM

    MOVLW 0x10
    MOVWF COUNT         ; Reset count to 16

    GOTO Main_Loop      ; Loop again
 
CLEAR_DISP:
    MOVLW 0
    MOVWF FSR0L

; Delay Funciton    
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Middle_loop
    MOVWF       REG11
    MOVLW       Outer_loop
    MOVWF       REG12
           
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    MOVLW       Inner_loop ; Re-initialize the inner loop for when the outer loop decrements.
    MOVWF       REG10
    DECF        REG11,1 // middle loop
    BNZ        _loop1
    MOVLW       Middle_loop
    MOVWF       REG11
    DECF        REG12,1 // outer loop
    BNZ        _loop1
    RETURN
 

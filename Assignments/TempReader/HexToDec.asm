//-----------------------------
// Title: TempController
//-----------------------------
// Purpose: Automatically measures and adjusts temperature
// Dependencies: NONE
// Compiler: Pic-as (v3.0)
// Author: Jason hernandez-downing
// OUTPUTS: Turns on heating or cooling system; LATD<2> = HEATING; LATD<4> = COOLING;
// INPUTS: Reference temperature and measured temperature 
// Versions:
//  	V6.2: 3/10/2026 - First version 
//-----------------------------
#include ".\MyConfigFile.inc"
#include <xc.inc>
    
;----------------
; PROGRAM INPUTS
;----------------
;The DEFINE directive is used to create macros or symbolic names for values.
;It is more flexible and can be used to define complex expressions or sequences of instructions.
;It is processed by the preprocessor before the assembly begins.

#define  refTempInput 		15 ; this is the input value
#define  measuredTempInput 	-5 ; this is the input value

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	     PORTD,1
#define contReg	  0x22 
#define	num	0x1
#define i	0x2
 
;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
;It directly substitutes the defined value into the code during the assembly process.
    
REG10   equ     10h   // in HEX
REG11   equ     11h
REG01   equ     1h
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _start1

    ORG          0020H           ; Begin assembly at 0020H

_start1:
   MOVLW    0
   MOVWF    TRISD
   MOVLW    refTempInput    ;sets refTemp
   MOVWF    0x20
   MOVLW    measuredTempInput	;sets measuedTemp
   MOVWF    0x21
   MOVLW    measuredTempInput
   BRA	    HexToDec
Continue: 
   MOVLW    measuredTempInput
   SUBWF    0x20, w
   BZ	    LED_OFF
   BN	    LED_COOL
   BNN	    LED_HOT

   HexToDec:		    ;Converts Hex to Dec
	    MOVLW   refTempInput
	    MOVWF   num
            BTFSC   num, 7
	    NEGF    num
	    MOVLW   100
   A1:	    INCF    i, F  ;A1 loop converts the 100s place of ref to dec
	    SUBWF   num, F
	    BC	    A1
	    DECF    i, F
	    ADDWF   num, F
	    MOVFF   i, 0x62
	    CLRF    i
	    MOVLW   10
   B1:	    INCF    i, F ;B1 loop converts the 10s place of ref to dec
	    SUBWF   num, F
	    BC	    B1
	    DECF    i, F
	    ADDWF   num, F
	    MOVFF   i, 0x61
	    MOVFF   num, 0x60 ;Places 1s place of ref in Reg60
	    
	    CLRF    i
	    MOVLW   measuredTempInput
	    MOVWF   num
	    BTFSC   num, 7
	    NEGF    num
	    MOVLW   100
   A2:	    INCF    i, F ;A2 loop converts the 100s place of measured to dec
	    SUBWF   num, F
	    BC	    A2
	    DECF    i, F
	    ADDWF   num, F
	    MOVFF   i, 0x72
	    CLRF    i
	    MOVLW   10
   B2:	    INCF    i, F ;B2 loop converts the 10s place of measured to dec
	    SUBWF   num, F
	    BC	    B2
	    DECF    i, F
	    ADDWF   num, F
	    MOVFF   i, 0x71
	    MOVFF   num, 0x70 ;Places 1s place of measured in Reg62
	    GOTO    Continue
   
   LED_OFF:
    MOVLW   0
    MOVWF   contReg
    MOVWF   LATD        ; ← changed from PORTD
    NOP
    BRA     _end

   LED_HOT:
    MOVLW   1
    MOVWF   contReg
    MOVLW   2
    MOVWF   LATD        ; ← changed from PORTD
    NOP
    BRA     _end

   LED_COOL:
    MOVLW   2
    MOVWF   contReg
    MOVLW   4
    MOVWF   LATD        ; ← changed from PORTD
    NOP
    BRA     _end

_end:
    BRA     _end
   




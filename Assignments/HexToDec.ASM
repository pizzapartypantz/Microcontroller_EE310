;----------------
; PROGRAM INPUTS
;----------------
; The DEFINE directive is used to create macros or symbolic names for values.
; It is more flexible and can be used to define complex expressions or sequences of instructions.
; It is processed by the preprocessor before the assembly begins.

#define  measuredTempInput  0x6  ; Arbitrary input value
#define  refTempInput       0x5  ; Arbitrary input value

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1      PORTD,1

;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
; It is simpler and is typically used for straightforward assignments.
; It directly substitutes the defined value into the code during the assembly process.
    
REG10   equ     10h   ; in HEX
REG11   equ     11h
REG01   equ     1h   
TEMPVAL equ     20h   ; Define tempVal register
COUNTER equ     21h   ; Define counter register
PORTD   equ     08h   ; Define PORTD register
LATD    equ     08h   ; Define LATD register

;---------------------
; Hex to Decimal Conversion
;---------------------
HexToDec:
    MOVLW   refTempInput
    MOVWF   TEMPVAL
    MOVLW   100

A_100s:
    INCF    COUNTER, F  ; Loop for 100s place conversion
    SUBWF   TEMPVAL, F
    BC      A_100s
    DECF    COUNTER, F
    ADDWF   TEMPVAL, F
    MOVFF   COUNTER, 0x62  ; Store 100s place in REG62
    CLRF    COUNTER
    MOVLW   10

A_10s:
    INCF    COUNTER, F  ; Loop for 10s place conversion
    SUBWF   TEMPVAL, F
    BC      A_10s
    DECF    COUNTER, F
    ADDWF   TEMPVAL, F
    MOVFF   COUNTER, 0x61  ; Store 10s place in REG61
    MOVFF   TEMPVAL, 0x60  ; Store 1s place in REG60

    CLRF    COUNTER
    MOVLW   measuredTempInput
    MOVWF   TEMPVAL
    MOVLW   100

B_100s:
    INCF    COUNTER, F  ; Loop for 100s place conversion of measured value
    SUBWF   TEMPVAL, F
    BC      B_100s
    DECF    COUNTER, F
    ADDWF   TEMPVAL, F
    MOVFF   COUNTER, 0x72  ; Store 100s place in REG72
    CLRF    COUNTER
    MOVLW   10

B_10s:
    INCF    COUNTER, F  ; Loop for 10s place conversion of measured value
    SUBWF   TEMPVAL, F
    BC      B_10s
    DECF    COUNTER, F
    ADDWF   TEMPVAL, F
    MOVFF   COUNTER, 0x71  ; Store 10s place in REG71
    MOVFF   TEMPVAL, 0x70  ; Store 1s place in REG70

;---------------------
; Temperature Control Logic
;---------------------
    MOVF    measuredTempInput, W
    SUBWF   refTempInput, W
    BZ      LED_OFF     ; If measuredTemp == refTemp, turn off system
    BC      LED_HOT     ; If measuredTemp > refTemp, activate heating
    GOTO    LED_COOL    ; Otherwise, activate cooling

LED_HOT:
    BSF     LATD,0  ; Turn on hotAir
    BCF     LATD,1  ; Turn off coolAir
    GOTO Program_End

LED_COOL:
    BCF     LATD,0  ; Turn off hotAir
    BCF     LATD,1  ; Turn off coolAir
    GOTO Program_End

LED_OFF:
    BCF     LATD,0  ; Turn off all
    BCF     LATD,1
    GOTO Program_End

Program_End:
    NOP


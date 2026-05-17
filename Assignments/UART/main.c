// /*
// * MAIN Generated Driver File
// * 
// * @file main.c
// * 
// * @defgroup main MAIN
// * 
// * @brief This is the generated driver implementation file for the MAIN driver.
// *
// * @version MAIN Driver Version 1.0.2
// *
// * @version Package Version: 3.1.2
//*/
//
///*
//© [2025] Microchip Technology Inc. and its subsidiaries.
//
//    Subject to your compliance with these terms, you may use Microchip 
//    software and any derivatives exclusively with Microchip products. 
//    You are responsible for complying with 3rd party license terms  
//    applicable to your use of 3rd party software (including open source  
//    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
//    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
//    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
//    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
//    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
//    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
//    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
//    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
//    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
//    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
//    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
//    THIS SOFTWARE.
//*/
//#include "mcc_generated_files/system/system.h"
//#include <stdio.h>
//#include <time.h>
//
///*
//    Main application
//*/
//int random_number(int min_num, int max_num)
//{
//    int result = 0, low_num = 0, hi_num = 0;
//
//    if (min_num < max_num)
//    {
//        low_num = min_num;
//        hi_num = max_num + 1; // include max_num in output
//    } else {
//        low_num = max_num + 1; // include max_num in output
//        hi_num = min_num;
//    }
//
//    result = (rand() % (hi_num - low_num)) + low_num;
//    return result;
//}
//
//int main(void)
//{
//    SYSTEM_Initialize();
//    UART2_Initialize();
//    srand(time(NULL));
//
//    while(1)
//    {
//        
//        //printf("%d\r\n", random_number(1, 100));
//        __delay_ms(500);
//        PORTBbits.RB1 ^= 1; // RB1
//    }
//}

#include "mcc_generated_files/system/system.h"
#include <stdio.h>
#include <time.h>

/*
    Main application
*/
int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1;
    } 
    else {
        low_num = max_num + 1;
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

int main(void)
{
    SYSTEM_Initialize();
    UART2_Initialize();
    srand(time(NULL));

    while (1)
    {
        if (UART2_IsRxReady()) // if data is available
        {
            char received = UART2_Read(); // read character

            if (received == 'A') {
                LATBbits.LATB1 = 1; // Turn ON LED
            } else {
                LATBbits.LATB1 = 0; // Turn OFF LED
            }
        }

        // Optional: transmit random number every second
        // printf("Random: %d\r\n", random_number(1,100));
        __delay_ms(400);
    }
}

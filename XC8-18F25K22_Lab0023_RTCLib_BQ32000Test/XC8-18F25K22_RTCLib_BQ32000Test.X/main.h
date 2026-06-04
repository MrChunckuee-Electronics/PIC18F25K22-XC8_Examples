/* 
 * File:   main.h
 * Author: mrchu
 *
 * Created on 16 de mayo de 2026, 08:17 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif


   
    #include <xc.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdio.h>
    #include <math.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "i2c2.h"
    #include "LCDNokia5110.h"
    #include "RTCLib.h"


    #define _XTAL_FREQ  16000000

    /*********** P O R T   D E F I N E S ******************************************/
    #define pinCtrlOutput_LED2      LATCbits.LATC2
    #define pinCtrlOutput_LED3      LATCbits.LATC3
    #define pinCtrlOutput_LED4      LATCbits.LATC4

    #define LED2_Status_SetHigh()   do { pinCtrlOutput_LED2 = true; } while(0)
    #define LED2_Status_SetLow()    do { pinCtrlOutput_LED2 = false; } while(0)
    #define LED2_Status_Toggle()    do { pinCtrlOutput_LED2 = ~pinCtrlOutput_LED2; } while(0)

    #define LED3_Status_SetHigh()   do { pinCtrlOutput_LED3 = true; } while(0)
    #define LED3_Status_SetLow()    do { pinCtrlOutput_LED3 = false; } while(0)
    #define LED3_Status_Toggle()    do { pinCtrlOutput_LED3 = ~pinCtrlOutput_LED3; } while(0)

    #define LED4_Status_SetHigh()   do { pinCtrlOutput_LED4 = true; } while(0)
    #define LED4_Status_SetLow()    do { pinCtrlOutput_LED4 = false; } while(0)
    #define LED4_Status_Toggle()    do { pinCtrlOutput_LED4 = ~pinCtrlOutput_LED4; } while(0)
    

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */


/*******************************************************************************
 *
 *                  LCD Nokia 5110 - Counter and demo image
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description:     Probando libreria para LCD Nokia5110, para cambiar la
 *                  distribucion de pines modificar en PCD8544.h
 *******************************************************************************
 * Rev.         Date            Comment
 *   v0.0.1     01/05/2026      Creación del firmware
 ******************************************************************************/
#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "FUSESConfig.h" //Only here
#include "LCDNokia5110.h"

#define _XTAL_FREQ 8000000

extern const char EYR_ImageLogo[];
extern const char carita[];
uint8_t counterStr[20] = {'0'};
uint16_t counter = 0;

void MCU_Initialize(void);

// Definimos la configuración
LCDNokia5110_t miPantalla = {
    .TRIS = &TRISB,
    .PORT = &PORTB,
    .RST  = 0, // RB0
    .CE   = 1, // RB1
    .DC   = 2, // RB2
    .DIN  = 3, // RB3 (SDIN)
    .CLK  = 4  // RB4 (SCLK)
};

void main(void) {
    MCU_Initialize();
    
    LCDNokia5110_Initialize(miPantalla);
    LCDNokia5110_Clear();
    __delay_ms(10);
    
    //Mensaje de inicio
    LCDNokia5110_Image(EYR_ImageLogo, 0, 0);
    __delay_ms(2000);
    LCDNokia5110_Clear();
    LCDNokia5110_putStr("Libreria LCD", 0, 0);
    LCDNokia5110_putStr("Nokia 5110", 0, 1);
    LCDNokia5110_putStr("MrChunckuee", 0, 2);
    LCDNokia5110_putStr("MPLABX & XC8", 0, 3);
    __delay_ms(2000);
    LCDNokia5110_Clear();
    
    LCDNokia5110_Bitmap(carita, 40, 3, 16, 16);
    
    while(1){
        counter++;
        if(counter>=1234) 
            counter = 0;
        LCDNokia5110_putStr("Conteo:", 0, 0);
        sprintf(counterStr, "%d", counter);
        LCDNokia5110_putStr("    ", 20, 1); //Clear before print counterStr
        LCDNokia5110_putStr(counterStr, 20, 1);
        __delay_ms(200);
    }
}


void MCU_Initialize(void){
    ANSELA = 0x00; //all digital
    ANSELB = 0x00; //all digital
    ANSELC = 0x00; //all digital
    OSCCONbits.IRCF = 0b110; //Fosc = 8MHz
    TRISB = 0x00; //PORTB output
}


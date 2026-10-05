/*******************************************************************************
 *
 *      LCD - Creacion de caracteres personalizados en la CGRAM
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Probando la funcion para la creacion de caracteres
 *                  personalizados en la CGRAM de la LCD.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      21/09/2025      Creación del ejemplo
 ******************************************************************************/
#include <xc.h>
#include <stdbool.h>
#include "FUSES_Config.h"
#include "LiquidCrystalDisplay.h" 

#define _XTAL_FREQ  16000000

// Objeto global de la estructura LCD
LCD_t LCD = {
    .TRIS = &TRISB,
    .PORT = &PORTB,
    .RS   = 0,  // Pin RB0 para RS
    .EN   = 1,  // Pin RB1 para EN
    .D4   = 4,  // Pin RB2 para D4
    .D5   = 5,  // Pin RB3 para D5
    .D6   = 6,  // Pin RB4 para D6
    .D7   = 7   // Pin RB5 para D7
};

// Arreglo de 8 bytes para un carácter de corazón
const uint8_t heart_char[] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

// Arreglo de 8 bytes para una sombrilla
const uint8_t umbrella_char[] = {
    0b00100,
    0b01110,
    0b11111,
    0b11111,
    0b00100,
    0b00100,
    0b10100,
    0b01000
};

void MCU_Init(void);

void main(void) {
    MCU_Init();

    if (!LCD_Init(LCD)){ }

    // Crea el carácter personalizado CGRAM
    LCD_createChar(0, heart_char);
    LCD_createChar(1, umbrella_char);

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_puts("MrChunckuee!!");

    LCD_SetCursor(1, 0);
    LCD_putc(0);
    LCD_SetCursor(1, 1);
    LCD_putc(1);
    
    //Caracteres especiales de la CGRAM
    LCD_SetCursor(1, 4);
    LCD_putc(255);
    LCD_SetCursor(1, 5);
    LCD_putc(252);
    LCD_SetCursor(1, 6);
    LCD_putc(0b11110100);
    LCD_SetCursor(1, 7);
    LCD_putc(0b11101110);
    LCD_SetCursor(1, 8);
    LCD_putc(0b11110111);

    while (1) {
    }
}

void MCU_Init(void){
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    //Configura oscilador interno a 16MHz
    OSCCONbits.IRCF=0b111; //El ciclo de instruccion es de 0.25 us
}
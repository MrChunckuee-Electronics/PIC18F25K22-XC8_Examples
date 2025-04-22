/*******************************************************************************
 *
 *      Control de servomotores con el modulo PCA9685
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Control de servomotores Futaba con el modulo PCA9685
 *
 *******************************************************************************
 * Rev.         Date            Comment
 *   v1.0.0     10/04/2025      - Creación y prueba de funcionamiento 
 ******************************************************************************/

#include <xc.h>
#include "FUSES_Config.h"
#include "i2c.h"
#include "PCA9685.h"

#define _XTAL_FREQ  64000000

uint16_t duty;

//Servo VIGOR VS-2
//uint16_t maxPWM_Duty = 505; // ancho de pulso en cuentas para la pocicion 180°
//uint16_t minPWM_Duty = 130; // ancho de pulso en cuentas para pocicion 0°

//Servo FUTABA S3003
uint16_t maxPWM_Duty = 500;
uint16_t minPWM_Duty = 120;

void MCU_Init(void);

void main(void){
    MCU_Init();
    I2C_Init();
    PCA9685_Init();
    while(1){
        for (duty = minPWM_Duty; duty < maxPWM_Duty; duty=duty+5) {
            PCA9685_SetPWM(0,0,duty);
            PCA9685_SetPWM(1,0,duty);
            __delay_ms(50);
        }
        __delay_ms(200);
        for (duty = maxPWM_Duty; duty > minPWM_Duty; duty=duty-5) {
            PCA9685_SetPWM(0,0,duty);
            PCA9685_SetPWM(1,0,duty);
            __delay_ms(50);
        }
        __delay_ms(200);
        
    }
}

void MCU_Init(void){
    // Pines de entrada digitles
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    
    //Configura oscilador interno a 16MHz
    OSCCONbits.IRCF=0b111; //El ciclo de instruccion es de 0.25 us
    //PLL FOSCx4 64MHz, el ciclo de instruccion es de .0625 us
    OSCTUNEbits.PLLEN=1;
    
    //Configura entras/salidas
    TRISA=0b00000000;
    TRISB=0b00000000;
    TRISC=0b00000000;
  
    //Limpiamos los puertos
    LATA=0x00;
    LATB=0x00;
    LATC=0x00;
    PORTA=0x00;
    PORTB=0x00;
    PORTC=0x00;   
}


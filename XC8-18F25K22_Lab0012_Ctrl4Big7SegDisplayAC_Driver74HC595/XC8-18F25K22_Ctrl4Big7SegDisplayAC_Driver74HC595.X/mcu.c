/*
 * File:   mcu.c
 * Author: MrChunckuee
 *
 * Created on 1 de marzo de 2023, 11:48 PM
 */

#include "main.h"

uint8_t digitsPCBWay[]={
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_A|SEG_B|SEG_E|SEG_F|SEG_G),                  // P
    (uint8_t)~(SEG_A|SEG_D|SEG_E|SEG_F),                        // C
    (uint8_t)~(SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),                  // b
    (uint8_t)~(SEG_C|SEG_D|SEG_E|SEG_F),                        // W/2 
    (uint8_t)~(SEG_B|SEG_C|SEG_D),                              // W/2 
    (uint8_t)~(SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),            // A
    (uint8_t)~(SEG_B|SEG_C|SEG_F|SEG_G),                        //  Y
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G),                                          //  -
    (uint8_t)~(SEG_G)                                          //  -
    
    };

uint8_t countS = 0;

/*******************************************************************************
 * Function:        void SYSTEM_Initialize(void)
 * Description:     Funcion de inicializacion.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         Inicializa las funciones correspondientes para que el sitema 
 *                  comience de forma adecuada.
 ******************************************************************************/
void SYSTEM_Initialize(void){
    MCU_Initialize();
    OSCILLATOR_Initialize();
}

/*******************************************************************************
 * Function:        void SYSTEM_Process(void)
 * Description:     Funcion de procesos.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void SYSTEM_Process(void){
    // Clear display
    DISPLAY_Reset();
    __delay_ms(1000);
    
    // Shows Abcd
    DISPLAY_Set(DIG_A, DIG_B, DIG_C, DIG_D);
    __delay_ms(1000);

    // Shows cdEF
    DISPLAY_Set(DIG_C, DIG_D, DIG_E, DIG_F);
    __delay_ms(1000);
    
    //shows numbers from 0 to 9999
    for (uint16_t i=0; i<=9999; i++){
        DISPLAY_Write(i);
        __delay_ms(200); 
        }
}

void SPONSOR_Welcome(void){
    for(uint8_t x = 0; x<=55; x++){
        display_values[3] = digitsPCBWay[0 + countS];
        display_values[2] = digitsPCBWay[1 + countS];
        display_values[1] = digitsPCBWay[2 + countS];
        display_values[0] = digitsPCBWay[3 + countS];
        DISPLAY_Update();
        countS++;
        if(countS == 11) countS = 0;
        __delay_ms(500);
    }
}

/*******************************************************************************
 * Function:        void MCU_Initialize(void)
 * Description:     Inicializa puertos 
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void MCU_Initialize(void){
    //Puertos digitales
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    
    //Puertos como salida
    TRISB = 0x00;
    TRISC = 0x00;
    //Limpiamos puertos
    PORTB = 0x00;
    PORTC = 0x00;
    
}

/*******************************************************************************
 * Function:        void OSCILLATOR_Initialize(void)
 * Description:     Configuracion de oscilador
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void OSCILLATOR_Initialize(void){
    //Configura oscilador interno a 16MHz
    OSCCONbits.IRCF=0b111; //El ciclo de instruccion es de 0.25 us
}
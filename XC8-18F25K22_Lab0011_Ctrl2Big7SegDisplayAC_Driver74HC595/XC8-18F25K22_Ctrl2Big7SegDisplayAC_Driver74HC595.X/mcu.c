/*
 * File:   mcu.c
 * Author: MrChunckuee
 *
 * Created on 1 de marzo de 2023, 11:48 PM
 */

#include "main.h"

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
    
    // Shows Ab
    DISPLAY_Set(DIG_A, DIG_B);
    __delay_ms(1000);

    // Shows Cd
    DISPLAY_Set(DIG_C, DIG_D);
    __delay_ms(1000);

    // Shows EF
    DISPLAY_Set(DIG_E, DIG_F);
    __delay_ms(1000);
    
    //shows numbers from 0 to 99
    for (uint8_t i=0; i<=99; i++){
        DISPLAY_Write(i);
        __delay_ms(250); 
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
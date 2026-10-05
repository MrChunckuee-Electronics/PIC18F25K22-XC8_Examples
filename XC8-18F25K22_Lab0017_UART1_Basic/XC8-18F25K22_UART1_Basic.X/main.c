/*******************************************************************************
 *
 *      UART1 - Comunicacion basica de escritura y lectura
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Comunicacion basica de la UART1 con un monitor serial. 
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      15/02/2026      Creacion del ejemplo
 ******************************************************************************/
#include <xc.h>
#include <stdbool.h>
#include "FUSES_Config.h"
#include "uart1.h"

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

void MCU_Initialize(void);

void main(void){
    char datoRxTx;
    MCU_Initialize();
    
    // Inicializar UART a 9600 baudios
    UART1_Initialize(9600);
    __delay_ms(500); 
    
    // Enviar un mensaje de bienvenida
    UART1_PutStr("UART Inicializado...\r\n");
    UART1_PutStr("Escribe algo y lo devolvere: \r\n");

    while(1){
        // Leer un caracter (se queda esperando hasta recibir uno)
        datoRxTx = UART1_Read();
        UART1_Write(datoRxTx);
       
        if(datoRxTx == 'A'){
            LED2_Status_Toggle();
        }
        else if(datoRxTx == 'S'){
            LED3_Status_Toggle();
        }
        else if(datoRxTx == 'D'){
            LED4_Status_Toggle();
        }
        else{
            
        }
        
    }
}

void MCU_Initialize(void){
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x80; // RC7 como entrada (RX), RC6 como salida (TX)
    
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    // Configura oscilador interno a 16MHz
    OSCCONbits.IRCF = 0b111; 
    __delay_ms(10); 
}

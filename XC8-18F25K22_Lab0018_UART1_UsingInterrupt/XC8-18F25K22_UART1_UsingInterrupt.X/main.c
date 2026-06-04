/*******************************************************************************
 *
 *      UART1 con Interrupciones - Envio y recepcion de datos
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Uso de la UART1 con interrupciones para comunicacion con un 
 *                  monitor serial. 
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      28/02/2026      Creacion del ejemplo
 ******************************************************************************/

#include "FUSES_Config.h"
#include "main.h"

void MCU_Initialize(void);

void main(void){
    MCU_Initialize();
    UART1_Initialize(115200);
    
    INTERRUPT_Initialize();
    INTERRUPT_InterruptEnable();
    __delay_ms(1000);
    
    UART1_PutStr("Pruebas libreria UART1 con interrupciones...\r\n");
    UART1_PutStr("Uso de buffer circulares para atender TX/RX.\r\n");

    while(1){
        if(UART1_RxAvailable()){
            char c = UART1_ReadBuffer();
            
            // Procesar datos sin bloquear el envío
            if(c == 'A'){
                UART1_PutStr("LED 2 Toggle\r\n");
                LED2_Status_Toggle();
            }
            else if(c == 'B'){
                UART1_PutStr("LED 3 Toggle\r\n");
                LED3_Status_Toggle();
            }
            else if(c == 'C'){
                UART1_PutStr("LED 4 Toggle\r\n");
                LED4_Status_Toggle();
            }
            else{
                UART1_Write(c); // Eco no bloqueante
            }
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

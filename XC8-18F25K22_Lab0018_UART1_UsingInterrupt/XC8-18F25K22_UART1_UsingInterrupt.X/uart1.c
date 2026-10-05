/*******************************************************************************
 *
 *                  UART1
 *
 *******************************************************************************
 * FileName:        uart1.c
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez Ramírez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Library for used UART1 with interrups
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sánchez Ramírez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      28/02/2026      Created library.
 ******************************************************************************/

#include "main.h"

// Buffers Circulares
static volatile char rxBuffer[UART1_RX_BUFFER_SIZE];
static volatile uint8_t rxHead = 0;
static volatile uint8_t rxTail = 0;

static volatile char txBuffer[UART1_TX_BUFFER_SIZE];
static volatile uint8_t txHead = 0;
static volatile uint8_t txTail = 0;

/*******************************************************************************
 * Function:        void UART1_Initialize(long baudRate)
 * Description:     Inicializa la UART1
 * Precondition:    None
 * Parameters:      long baudRate = Valor del BaudRate 
 * Return Values:   Se debe definir _XTAL_FREQ para obtener brgValue.
 * Remarks:         La funcion inicializa la UART1 para atender Tx y Rx con 
 *                  interrupciones, tambien se seleeciona de forma autmatica el 
 *                  valor ve BRG.
 ******************************************************************************/
void UART1_Initialize(long baudRate){
    // Configuración de pines (RC6 TX, RC7 RX)
    TRISCbits.TRISC6 = 0; 
    TRISCbits.TRISC7 = 1; 
    ANSELCbits.ANSC6 = 0; 
    ANSELCbits.ANSC7 = 0; 

    uint16_t brgValue;

    if(baudRate >= 19200){
        // MODO ALTA VELOCIDAD (16-bit, BRGH=1)
        // Fórmula: SPBRG = (_XTAL_FREQ / (4 * baudRate)) - 1
        TXSTAbits.BRGH = 1;
        BAUDCON1bits.BRG16 = 1;
        brgValue = (uint16_t)((_XTAL_FREQ / (4 * baudRate)) - 1);
    } 
    else if(baudRate >= 2400){
        // MODO VELOCIDAD MEDIA (16-bit, BRGH=0)
        // Fórmula: SPBRG = (_XTAL_FREQ / (16 * baudRate)) - 1
        TXSTAbits.BRGH = 0;
        BAUDCON1bits.BRG16 = 1;
        brgValue = (uint16_t)((_XTAL_FREQ / (16 * baudRate)) - 1);
    }
    else{
        // MODO BAJA VELOCIDAD (8-bit, BRGH=0)
        // Fórmula: SPBRG = (_XTAL_FREQ / (64 * baudRate)) - 1
        TXSTAbits.BRGH = 0;
        BAUDCON1bits.BRG16 = 0;
        brgValue = (uint16_t)((_XTAL_FREQ / (64 * baudRate)) - 1);
    }

    SPBRG = (unsigned char)brgValue;         
    SPBRGH = (unsigned char)(brgValue >> 8); 

    TXSTAbits.SYNC = 0; // Asíncrono
    RCSTAbits.SPEN = 1; // Puerto serial habilitado
    TXSTAbits.TXEN = 1; // Transmisión habilitada
    RCSTAbits.CREN = 1; // Recepción habilitada

    // Configuración de Interrupciones
    IPR1bits.RC1IP = 1; // Alta prioridad RX
    IPR1bits.TX1IP = 1; // Alta prioridad TX
    PIE1bits.RC1IE = 1; // Habilitar RXIE
    PIE1bits.TX1IE = 0; // TXIE se activa al llamar a UART1_Write
}

// ISR de Recepción
void UART1_RxISR(void){
    if(RCSTAbits.OERR){ 
        RCSTAbits.CREN = 0; 
        RCSTAbits.CREN = 1;
    }
    char data = RCREG;
    uint8_t nextHead = (rxHead + 1) % UART1_RX_BUFFER_SIZE;
    if(nextHead != rxTail){
        rxBuffer[rxHead] = data;
        rxHead = nextHead;
    }
}

// ISR de Transmisión
void UART1_TxISR(void){
    if(txHead != txTail){
        TXREG = txBuffer[txTail]; 
        txTail = (txTail + 1) % UART1_TX_BUFFER_SIZE;
    }
    else{
        PIE1bits.TX1IE = 0; // Buffer vacío, deshabilitar interrupción
    }
}

void UART1_Write(char data){
    uint8_t nextHead = (txHead + 1) % UART1_TX_BUFFER_SIZE;
    
    // Esperar solo si el buffer está lleno
    while(nextHead == txTail); 
    
    txBuffer[txHead] = data;
    txHead = nextHead;
    PIE1bits.TX1IE = 1; // Iniciar transmisión por interrupción
}

bool UART1_RxAvailable(void){
    return (rxHead != rxTail);
}

char UART1_ReadBuffer(void){
    char data = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % UART1_RX_BUFFER_SIZE;
    return data;
}

void UART1_PutStr(const char* dataStr){
    while(*dataStr != '\0'){
        UART1_Write(*dataStr++);
    }
}

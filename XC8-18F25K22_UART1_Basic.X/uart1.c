/*
 * File:   uart1.c
 * Author: MrChunckuee
 *
 * Created on 12 de febrero de 2026, 12:07 AM
 */

#include <xc.h>
#define _XTAL_FREQ 16000000 // Definir Fosc para delays

void UART1_Initialize(long baudRate){
    // Configurar pines RC6 (TX) y RC7 (RX)
    TRISCbits.TRISC6 = 0; // TX como salida
    TRISCbits.TRISC7 = 1; // RX como entrada
    ANSELCbits.ANSC6 = 0; // Digital
    ANSELCbits.ANSC7 = 0; // Digital

    // Cálculo de Baud Rate (Asíncrono, Alta Velocidad)
    SPBRG = ((_XTAL_FREQ / 64) / baudRate) - 1;
    TXSTAbits.BRGH = 0; // Baja velocidad
    TXSTAbits.SYNC = 0; // Modo asíncrono
    RCSTAbits.SPEN = 1; // Habilitar puerto serial

    // Habilitar transmisión y recepción
    TXSTAbits.TXEN = 1; // Habilitar transmisión
    RCSTAbits.CREN = 1; // Habilitar recepción
}

void UART1_Write(char data){
    while(!TXSTAbits.TRMT); // Esperar a que el buffer esté vacío
    TXREG = data; // Enviar dato
}

char UART1_Read(void){
    if (RCSTAbits.OERR) { // Si hay error de desbordamiento
        RCSTAbits.CREN = 0; // Resetear recepción
        RCSTAbits.CREN = 1;
    }
    while(!PIR1bits.RCIF); // Esperar dato
    return RCREG;
}

void UART1_PutStr(const char* dataStr){
	// Loop until the end of string.
	while (*dataStr != '\0') {
		UART1_Write(*dataStr);		
		// Point to next character.
		dataStr++;
	}
}


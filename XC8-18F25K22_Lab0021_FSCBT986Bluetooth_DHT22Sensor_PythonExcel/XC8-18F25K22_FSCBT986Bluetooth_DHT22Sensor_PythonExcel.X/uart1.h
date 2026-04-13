/* 
 * File:   uart1.h
 * Author: MrChunckuee
 *
 * Created on 12 de febrero de 2026, 12:07 AM
 */

#ifndef UART1_H
#define	UART1_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    #include <xc.h>
    #include <stdbool.h>
    #include <stdint.h>

    #define UART1_RX_BUFFER_SIZE 64 
    #define UART1_TX_BUFFER_SIZE 64 

    void UART1_Initialize(long baudRate);
    void UART1_Write(char data);
    void UART1_PutStr(const char* dataStr);

    bool UART1_RxAvailable(void);
    char UART1_ReadBuffer(void);

    // Rutinas para INTERRUPTManager
    void UART1_RxISR(void);
    void UART1_TxISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UART1_H */


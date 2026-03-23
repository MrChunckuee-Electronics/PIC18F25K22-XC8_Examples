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


    void UART1_Initialize(long baudRate);
    void UART1_Write(char data);
    char UART1_Read(void);
    void UART1_PutStr(const char* dataStr);


#ifdef	__cplusplus
}
#endif

#endif	/* UART1_H */


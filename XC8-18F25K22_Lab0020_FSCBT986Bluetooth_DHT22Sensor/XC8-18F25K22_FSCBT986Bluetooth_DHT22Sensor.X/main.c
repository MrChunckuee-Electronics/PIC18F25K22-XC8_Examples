/*******************************************************************************
 *
 *      DHT22 Sensor - Mostrar datos en el monitor serial
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Muestra la temperatura y humedad relativa en el moitor serial, 
 *                  ademas se puestran posibles errores que se generan al momento 
 *                  de leer la informacion del sensor.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      22/03/2026      Creacion del ejemplo
 ******************************************************************************/

#include "FUSES_Config.h"
#include "main.h"

void MCU_Initialize(void);

DHT22_Sensor_t miDHT_Sensor;

void main(void){
    MCU_Initialize();
    DHT22_Initialize();
    UART1_Initialize(115200);
    INTERRUPT_Initialize();
    INTERRUPT_InterruptEnable();
    __delay_ms(1000);
    
    // Mensaje inicial
    UART1_PutStr("\r\n--- Sistema de monitoreo DHT22 ---\r\n");
    
    while(1){
        if(DHT22_Read(&miDHT_Sensor)){ // Lectura exitosa
            UART1_PutStr("Temperatura: ");
            UART1_PutStr((char*)miDHT_Sensor.str_temperature); 
            UART1_PutStr(" C  -  ");
            UART1_PutStr("Humedad: ");
            UART1_PutStr((char*)miDHT_Sensor.str_relativeHumidity);
            UART1_PutStr(" %\r\n"); // \r\n para salto de línea en terminal
        }
        else{ // Manejo de errores 
            UART1_PutStr("Error Sensor: ");
            switch(miDHT_Sensor.status){
                case DHT22_ERROR_TIMEOUT:
                    UART1_PutStr("No Responde (Timeout)\r\n");
                    break;
                case DHT22_ERROR_CHECKSUM:
                    UART1_PutStr("Checksum incorrecto\r\n");
                    break;
                case DHT22_ERROR_RANGE:
                    UART1_PutStr("Valores fuera de rango\r\n");
                    break;
                default:
                    UART1_PutStr("Error desconocido\r\n");
                    break;
            }
        }
        
        // El DHT22 requiere al menos 2 segundos entre lecturas
        __delay_ms(2000); 
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
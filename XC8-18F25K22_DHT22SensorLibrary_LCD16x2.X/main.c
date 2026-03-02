/*******************************************************************************
 *
 *      DHT22 Sensor - Mostrar datos en LCD
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Muestra la temperatura y humedad relativa en una pantalla 
 *                  LCD16x2, ademas se puestran posibles errores que se generan 
 *                  al momento de leer la informacion del sensor.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      01/03/2026      Creacion del ejemplo
 ******************************************************************************/

#include "FUSES_Config.h"
#include "main.h"

void MCU_Initialize(void);
void DISPLAY_Initialize(void);

DHT22_Sensor_t miDHT_Sensor;

void main(void){
    MCU_Initialize();
    DHT22_Initialize();
    DISPLAY_Initialize();
    
    while(1){
        if(DHT22_Read(&miDHT_Sensor)){ 
            // Lectura exitosa
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_puts("Temp: ");
            LCD_puts((char*)miDHT_Sensor.str_temperature); 
            LCD_putc(0xDF); // Caracter de grado '°'
            LCD_puts("C");
            
            LCD_SetCursor(1, 0);
            LCD_puts("Hume: ");
            LCD_puts((char*)miDHT_Sensor.str_relativeHumidity);
            LCD_puts(" %");
        }
        else{ // Manejo de errores 
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_puts("Sensor Error:");
            LCD_SetCursor(1, 0);
            switch(miDHT_Sensor.status){
                case DHT22_ERROR_TIMEOUT:
                    LCD_puts("No Responde");
                    break;
                case DHT22_ERROR_CHECKSUM:
                    LCD_puts("Bad Checksum");
                    break;
                case DHT22_ERROR_RANGE:
                    LCD_puts("Fuera de Rango");
                    break;
                default:
                    LCD_puts("Error Desconocido");
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

void DISPLAY_Initialize(void){
    LCD_t LCD = {
        .TRIS = &TRISB,
        .PORT = &PORTB,
        .RS   = 0,  // Pin RB0 para RS
        .EN   = 1,  // Pin RB1 para EN
        .D4   = 4,  // Pin RB2 para D4
        .D5   = 5,  // Pin RB3 para D5
        .D6   = 6,  // Pin RB4 para D6
        .D7   = 7   // Pin RB5 para D7
    };
    
    if(!LCD_Init(LCD)){ } // LCD Init
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_puts("MrChunckuee!!");
    LCD_SetCursor(1, 0);
    LCD_puts("DHT22Sensor Test");
    __delay_ms(2000);
}
/*******************************************************************************
 *
 *                  BQ32000 - Library test
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.32
 * Author:          Pedro Sanchez Ramirez (mrchunckuee_electronics)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description:     Probando libreria RTCLib, con el BQ32000
 *******************************************************************************
 * Rev.         Date            Comment
 *   v0.0.1     01/05/2026      Creación del firmware
 ******************************************************************************/

#include "FUSES_Config.h" //Only here
#include "main.h"

void MCU_Initialize(void);
void DISPLAY_Initialize(void);
void RELOJ_Initialize(void);

void main(void){
    MCU_Initialize();
    I2C2_Initialize(I2C2_FOSC_16MHZ, I2C2_SPEED_100KHZ);
    RELOJ_Initialize();
    DISPLAY_Initialize();
    
    char txtHora[12];
    char txtFecha[12];
    
    while(1){
        Time_t horaActual;
        Date_t fechaActual;
        
        // Intentar leer los datos del RTC 
        if (RTC_GetTime(&horaActual) && RTC_GetDate(&fechaActual)){
     
            // Formatear Hora: "HH:MM:SS"
            sprintf(txtHora, "%02d:%02d:%02d", horaActual.hours, horaActual.minutes, horaActual.seconds);
            
            // Formatear Fecha: "DD/MM/YYYY"
            sprintf(txtFecha, "%02d/%02d/%04d", fechaActual.day, fechaActual.month, fechaActual.year);
            
            LCDNokia5110_putStr("Hora:", 0, 0);
            LCDNokia5110_putStr(txtHora, 10, 1);
            LCDNokia5110_putStr("Fecha:", 0, 3);
            LCDNokia5110_putStr(txtFecha, 10, 4);
         
        } else {
            // Si el RTC falla o el bus I2C se congela, avisa en la pantalla
            LCDNokia5110_putStr("RTC Error!! ", 0, 1);
            LCDNokia5110_putStr("Check       ", 0, 3);
            LCDNokia5110_putStr("    I2C Bus ", 0, 4);
        }
        
        // Refrescar la pantalla aproximadamente cada 200ms
        __delay_ms(200);
    }
}

void MCU_Initialize(void){
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    // Configura oscilador interno a 16MHz
    OSCCONbits.IRCF = 0b111; 
    __delay_ms(10); 
}

void DISPLAY_Initialize(void){
    // Create an instance of the LCD structure
    LCDNokia5110_t myLCD = {
        .TRIS = &TRISB,
        .PORT = &PORTB,
        .RST  = 3, // RB3
        .CE   = 4, // RB4
        .DC   = 5, // RB5
        .DIN  = 6, // RB6 (SDIN)
        .CLK  = 7  // RB7 (SCLK)
    };
    
    // Initialize LCD with specific pins/settings
    LCDNokia5110_Initialize(myLCD);
    
    // Clear display buffer
    LCDNokia5110_Clear();
    
    // Display text
    LCDNokia5110_putStr("MrChunckuee!", 0, 0);
    LCDNokia5110_putStr("RTC Library", 0, 2);
    LCDNokia5110_putStr("BQ32000 Test", 0, 4);
    __delay_ms(2000);
    LCDNokia5110_Clear();
}


void RELOJ_Initialize(void){
    RTC_Initialize(RTC_MODEL_BQ32000, BQ32000_DEVICE_ADDRESS);
    
    // Configurar fecha y hora inicial
    // Descomenta estas líneas la primera vez para condfigurarlo.
    /*
    Time_t horaInicial = {.hours = 18, .minutes = 12, .seconds = 0, .timeMode = TwentyFourHoursMode};
    Date_t fechaInicial = {.year = 2026, .month = 5, .day = 17, .weekday = Sunday};
    RTC_SetTime(&horaInicial);
    RTC_SetDate(&fechaInicial);
    */
}


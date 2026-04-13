/*******************************************************************************
 *
 *  DHT22 Sensor - Mostrar datos en el monitor serial y almacenar en Excel/CSV
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Muestra la temperatura y humedad relativa en el moitor serial, 
 *                  ademas podemos exportar a u archivo de Excel o CSV.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      11/04/2026      Creacion del ejemplo
 ******************************************************************************/

#include "FUSES_Config.h"
#include "main.h"

void MCU_Initialize(void);
void RTCC_Initialize(void);
void RTCC_Update(void);
void SERIAL_TXProcess(void);
void SERIAL_RXProcessCommand(void);

#define TIME_DELAY 2000
uint16_t RTCC_Count = 0;
bool SENSOR_ProcessData = false;
bool SENSOR_SendInfo = false;

DHT22_Sensor_t miDHT_Sensor;

#define CMD_BUFFER_SIZE 10
char cmdReceiveBuffer[CMD_BUFFER_SIZE] = {0};
uint8_t dataSendBuffer[64] = {0};
uint8_t cmdIndex = 0;

void main(void){
    MCU_Initialize();
    DHT22_Initialize();
    UART1_Initialize(115200);
    INTERRUPT_Initialize();
    INTERRUPT_InterruptEnable();
    __delay_ms(1000);
    while(1){
        RTCC_Update(); //Actualizamos el contador de tiempo
        SERIAL_RXProcessCommand(); //Revisamos comando recibido
        SERIAL_TXProcess(); // Leemos sensor y enviamos datos
    }
}

void SERIAL_RXProcessCommand(void){
    if(UART1_RxAvailable()){
        char c = UART1_ReadBuffer();
        // Si no es un fin de línea y hay espacio, guardamos el carácter
        if(c != '\r' && c != '\n'){
            if(cmdIndex < (CMD_BUFFER_SIZE - 1)){
                cmdReceiveBuffer[cmdIndex++] = c;
            }
        }
        
        // Si llega un fin de línea, procesamos la cadena acumulada
        else if(cmdIndex > 0){
            cmdReceiveBuffer[cmdIndex] = '\0'; // Terminamos la cadena

            if(strcmp(cmdReceiveBuffer, "Start") == 0){
                SENSOR_SendInfo = true;
                //Limpiamos banderas para inicar contador
                TMR0_ovrFlag = false;
                SENSOR_ProcessData = false;
                RTCC_Count = 0;
                UART1_PutStr("Sistema de monitoreo sensor tempertura y humedad\r\n");
                UART1_PutStr("Guardar datos de Python en Excel\r\n");
                UART1_PutStr("Comando recibido: INICIANDO monitoreo...\r\n");
            }
            else if(strcmp(cmdReceiveBuffer, "Stop") == 0){
                SENSOR_SendInfo = false;
                //Limpiamos banderas
                TMR0_ovrFlag = false;
                SENSOR_ProcessData = false;
                RTCC_Count = 0;
                UART1_PutStr("Comando recibido: DETENIENDO monitoreo...\r\n");
            }
            else{
                SENSOR_SendInfo = false;
                UART1_PutStr("Comando recibido: NO valido!!! \r\n");
            }

            cmdIndex = 0; // Limpiamos el buffer para el siguiente comando
        }
    }
}

void SERIAL_TXProcess(void){
    if(SENSOR_ProcessData && SENSOR_SendInfo){ // El DHT22 requiere al menos 2 segundos entre lecturas
        if(DHT22_Read(&miDHT_Sensor)){ // Lectura exitosa
            // Datos separados por una "," para filtrar en el monito serial            
            sprintf(dataSendBuffer, "Temperatura: %.1f, Humedad R: %.1f \r\n", miDHT_Sensor.temperature, miDHT_Sensor.relativeHumidity);
            UART1_PutStr(dataSendBuffer);
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
    SENSOR_ProcessData = false;
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
    RTCC_Initialize();
}

void RTCC_Initialize(void){
    //Configuración del TMR0 = 1mS
    T0CONbits.TMR0ON = 1; //TMR0 On
    T0CONbits.T08BIT = 0; //16 bits
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b000; // Prescaler = 2
    TMR0H = 0xF8;
    TMR0L = 0x30;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 0; //Low priority
}

void RTCC_Update(void){ //Cada 2 seg permite enviar dats
    if(!TMR0_ovrFlag) return;
    TMR0_ovrFlag = false;

    if (++RTCC_Count >= TIME_DELAY){
        SENSOR_ProcessData = true;
        RTCC_Count = 0;
    }
    
}
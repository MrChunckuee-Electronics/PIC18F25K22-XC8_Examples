/*******************************************************************************
 *
 *  24CW1280 Library - Test Lectura, escritura y verificacion
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Test de funciones implementadas en la libreria EEPROM_24CWxx
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      20/06/2026      Creacion del ejemplo
 ******************************************************************************/

#include "FUSES_Config.h"
#include "main.h"

#define BUFFER_SIZE         50
uint8_t datosEscritura[BUFFER_SIZE];
uint8_t datosLectura[BUFFER_SIZE];
uint16_t direccionPrueba = 0x005A; 
uint8_t statusVerificacion = 0xFF;

#define CMD_BUFFER_SIZE     15
char cmdReceiveBuffer[CMD_BUFFER_SIZE] = {0};
uint8_t dataSendBuffer[64] = {0};
uint8_t cmdIndex = 0;

void MCU_Initialize(void);
void RTCC_Initialize(void);
void RTCC_Update(void);
void SERIAL_RXProcessCommand(void);
void SERIAL_SendBufferHex(const char* titulo, uint8_t *buffer, uint16_t len);
void EEPROM_DemoTest(void); 

void main(void){
    MCU_Initialize();
    UART1_Initialize(115200);
    I2C2_Initialize(I2C2_FOSC_16MHZ, I2C2_SPEED_100KHZ);
    INTERRUPT_Initialize();
    INTERRUPT_InterruptEnable();
    __delay_ms(1000);
    while(1){
        SERIAL_RXProcessCommand(); //Revisamos comando recibido
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

            if(strcmp(cmdReceiveBuffer, "Iniciar Test") == 0){
                UART1_PutStr(">> Comando aceptado <<\r\n");
                EEPROM_DemoTest(); // Ejecuta todo el proceso
            }
            else{
                UART1_PutStr(">> Comando desconocido << Intenta con 'Iniciar Test\r\n");
            }

            cmdIndex = 0; // Limpiamos el buffer para el siguiente comando
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
}

/*******************************************************************************
 * Función:         void EEPROM_DemoTest(void)
 * Descripción:     Contiene toda la lógica de lectura previa, escritura y test
 ******************************************************************************/
void EEPROM_DemoTest(void){
    UART1_PutStr("============== EEPROM TEST INICIADO ==============\r\n");
    
    //Leer datos actuales
    UART1_PutStr("Leyendo datos actuales en la EEPROM ... \r\n");
    EEPROMExt_ReadData(direccionPrueba, datosLectura, BUFFER_SIZE);
    SERIAL_SendBufferHex("ESTADO PREVIO EN LA EEPROM (ANTES DE ESCRIBIR)\r\n", datosLectura, BUFFER_SIZE);

    // Preparar y almacenar nuevos datos
    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
        datosEscritura[i] = i; //Preparamos datos a escribir
    }
    UART1_PutStr("Escribiendo 50 bytes nuevos en la EEPROM... \r\n");
    EEPROMExt_WriteData(direccionPrueba, datosEscritura, BUFFER_SIZE);
    SERIAL_SendBufferHex("BUFFER NUEVO PREPARADO EN RAM\r\n", datosEscritura, BUFFER_SIZE);

    // Leer los datos nuevos, previamente almacenados
    for(uint8_t i = 0; i < BUFFER_SIZE; i++) {
        datosLectura[i] = 0x00; //Limpiamos buffer antes de leer datos.
    }
    UART1_PutStr("Leyendo nuevos datos almacenados... \r\n");
    EEPROMExt_ReadData(direccionPrueba, datosLectura, BUFFER_SIZE);
    SERIAL_SendBufferHex("BUFFER LEIDO DESDE EEPROM (DESPUES DE ESCRIBIR)\r\n", datosLectura, BUFFER_SIZE);

    //Verificar datos almacenados
    statusVerificacion = EEPROMExt_Verify(direccionPrueba, datosEscritura, BUFFER_SIZE);
    UART1_PutStr("-------------------------------------------------\r\n");
    if (statusVerificacion == 0) {
        UART1_PutStr("RESULTADO DEL TEST: [ EXITO ]\r\n");
    } else {
        UART1_PutStr("RESULTADO DEL TEST: [ FALLO ]\r\n");
    }
    UART1_PutStr("-------------------------------------------------\r\n");
    UART1_PutStr("Escribe 'Iniciar Test' para repetir la prueba...\r\n");
}

void SERIAL_SendBufferHex(const char* titulo, uint8_t *buffer, uint16_t len){
    char txtMsg[10];
    UART1_PutStr("---\r\n");
    UART1_PutStr(titulo);
    UART1_PutStr("---\r\n");
    for (uint16_t i = 0; i < len; i++) {
        sprintf(txtMsg, "%02X ", buffer[i]);
        UART1_PutStr(txtMsg);
        if ((i + 1) % 15 == 0) UART1_PutStr("\r\n");
    }
    UART1_PutStr("---\r\n");
}
/*******************************************************************************
 *
 *      IO - Control display 7 segmentos anodo comun con 74HC595
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description: 
 *
 *******************************************************************************
 * Rev.         Date            Comment
 *   v1.0.0     02/03/2023      - Creación y prueba de funcionamiento 
 ******************************************************************************/

#include "device_config.h" //Config fuses
#include "main.h"

void main(void) {
    SYSTEM_Initialize();
    while(1){
        SYSTEM_Process(); //Show digits
    }
}

/* 
 * File:   i2c2.h
 * Author: mrchunckuee_electronics
 *
 * Created on 3 de junio de 2019, 05:39 PM
 */

#ifndef I2C2_H
#define	I2C2_H

#ifdef	__cplusplus
extern "C" {
#endif


    #include <xc.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdio.h>
    #include <math.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    
    /*********** G E N E R A L   D E F I N E S ************************************/
    // Lista de frecuencias FOSC (agregar las que sean necesarias)
    #define I2C2_FOSC_4MHZ       4000000UL
    #define I2C2_FOSC_8MHZ       8000000UL
    #define I2C2_FOSC_16MHZ     16000000UL
    #define I2C2_FOSC_32MHZ     32000000UL
    #define I2C2_FOSC_64MHZ     64000000UL

    // Lista de velocidades de bus disponibles
    #define I2C2_SPEED_100KHZ   100000UL   // Standard Mode
    #define I2C2_SPEED_400KHZ   400000UL   // Fast Mode

    /*********** P O R T   D E F I N E S ******************************************/
    #if defined __18F46K22
        #define SCL2_Sensor_TRIS               TRISDbits.TRISD0                         // Clock pin direction
        #define SCL2_Sensor_LAT                LATDbits.LATD0                           // Clock pin for i2c
        #define SCL2_Sensor_SetDigitalInput()  do { SCL2_Sensor_TRIS = 1; } while(0)    

        #define SDA2_Sensor_TRIS               TRISDbits.TRISD1                         // Data pin direction
        #define SDA2_Sensor_LAT                LATDbits.LATD1                           // Data pin for i2c
        #define SDA2_Sensor_SetDigitalInput()  do { SDA2_Sensor_TRIS = 1; } while(0)    
    #elif defined __18F25K22 
        #define SCL2_Sensor_TRIS               TRISBbits.TRISB1                         // Clock pin direction
        #define SCL2_Sensor_LAT                LATBbits.LATB1                           // Clock pin for i2c
        #define SCL2_Sensor_SetDigitalInput()  do { SCL2_Sensor_TRIS = 1; } while(0)    

        #define SDA2_Sensor_TRIS               TRISBbits.TRISB2                         // Data pin direction
        #define SDA2_Sensor_LAT                LATBbits.LATB2                           // Data pin for i2c
        #define SDA2_Sensor_SetDigitalInput()  do { SDA2_Sensor_TRIS = 1; } while(0)    
    #endif


    /*********** P R O T O T Y P E S **********************************************/
    void I2C2_Initialize(uint32_t fosc_hz, uint32_t i2c_speed_hz);
    void I2C2_Start(void);
    void I2C2_ReStart(void);
    void I2C2_Stop(void);
    void I2C2_Send_ACK(void);
    void I2C2_Send_NACK(void);
    uint8_t I2C2_Send(uint8_t datat);
    uint8_t I2C2_Read(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* I2C2_H */


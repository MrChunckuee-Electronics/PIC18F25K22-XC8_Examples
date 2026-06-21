/*******************************************************************************
 *
 *                 i2c2 Library
 *
 *******************************************************************************
 * FileName:        i2c2.c
 * Complier:        XC8 v2.31
 * Author:          Pedro Sanchez Ramirez (@mrchunckuee_electronics)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Library for i2c2 use in PIC18
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sanchez Ramirez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v0.0.1      03/06/2019      Created library.
 *  v0.0.2      28/06/2020      Implementing for second I2C on PIC18F25K22.
 *  v0.0.3      14/04/2020      Suport for I2C on PIC18F46K22
 *  v0.0.4      17/05/2026      Changed I2C2_Initialize(), now accept fosc_hz and i2c_speed_hz
 ******************************************************************************/

#include "i2c2.h"

/*******************************************************************************
 * Function:        void I2C2_Initialize(uint32_t fosc_hz, uint32_t i2c_speed_hz)
 * Description:     Configure I2C module 
 * Precondition:    None
 * Parameters:      fosc_hz = System oscillator frequency in Hz (e.g., 64000000UL)
 *                  i2c_speed_hz = Desired I2C clock in Hz (e.g., 100000UL o 400000UL)
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_Initialize(uint32_t fosc_hz, uint32_t i2c_speed_hz){	
    SCL2_Sensor_SetDigitalInput();        
    SDA2_Sensor_SetDigitalInput();	
    
    SSP2STAT = 0b10000000;		// Slew Rate control is disabled
    SSP2CON1 = 0b00101000;		// Select and enable I2C in master mode
    
    /*
     * CClculo: SSP2ADD = (fosc_hz / (4 * i2c_speed_hz)) - 1
     * Se usa casting explícito para asegurar que la operación no desborde.
     */
    SSP2ADD = (uint8_t)((fosc_hz / (4UL * i2c_speed_hz)) - 1UL);	
}

/*******************************************************************************
 * Function:        void I2C_Start(void)
 * Description:     Sends start bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_Start(void){
	SSP2CON2bits.SEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;	
}

/*******************************************************************************
 * Function:        void I2C_ReStart(void)
 * Description:     Sends restart bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_ReStart(void){
	SSP2CON2bits.RSEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;			
}

/*******************************************************************************
 * Function:        void I2C_Stop(void)
 * Description:     Sends stop bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_Stop(void){
	SSP2CON2bits.PEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;				
}

/*******************************************************************************
 * Function:        void I2C_Send_ACK(void)
 * Description:     Sends ACK bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_Send_ACK(void){
	SSP2CON2bits.ACKDT = 0;			
	SSP2CON2bits.ACKEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;			
}

/*******************************************************************************
 * Function:        void I2C_Send_NACK(void)
 * Description:     Sends NACK bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C2_Send_NACK(void){
	SSP2CON2bits.ACKDT = 1;			
	SSP2CON2bits.ACKEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;	
}

/*******************************************************************************
 * Function:        unsigned char I2C_Write(unsigned char BYTE)
 * Description:     Transfers one byte
 * Precondition:    None
 * Parameters:      data = Value for slave device
 * Return Values:   Return ACK/NACK from slave
 * Remarks:         None
 ******************************************************************************/
uint8_t I2C2_Send(uint8_t data){
	SSP2BUF = data;                 
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;			
	return SSP2CON2bits.ACKSTAT;    
}

/*******************************************************************************
 * Function:        unsigned char I2C_Read(void)
 * Description:     Reads one byte
 * Precondition:    None
 * Parameters:      None
 * Return Values:   Return received byte
 * Remarks:         None
 ******************************************************************************/
uint8_t I2C2_Read(void){
	SSP2CON2bits.RCEN = 1;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = 0;			
    return SSP2BUF;      
}

/*******************************************************************************
 *
 *                 i2c Library
 *
 *******************************************************************************
 * FileName:        i2c.c
 * Complier:        XC8 v1.42
 * Author:          Pedro Sánchez Ramírez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description:     Library for i2c use in PIC18
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sánchez Ramírez
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
 *  v1.0.0      03/06/2019      Created library.
 *  v1.0.1      25/05/2020      Implementing for second I2C on PIC18F22K22.
 ******************************************************************************/
#include <xc.h>
#include "i2c.h"

#define _XTAL_FREQ  64000000    // Fosc

/*******************************************************************************
 * Function:        void I2C_Init(void)
 * Description:     Configure I2C module
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_Init(void){	
	SDA2_DIR = I2C_INPUT;        
	SCK2_DIR = I2C_INPUT;		
	SSP2ADD  = ((_XTAL_FREQ/4000)/I2C_SPEED) - 1;	
	SSP2STAT = 0b10000000;		// Slew Rate control is disabled
	SSP2CON1 = 0b00101000;		// Select and enable I2C in master mode
}

/*******************************************************************************
 * Function:        void I2C_Start(void)
 * Description:     Sends start bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_Start(void){
	SSP2CON2bits.SEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
}

/*******************************************************************************
 * Function:        void I2C_ReStart(void)
 * Description:     Sends restart bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_ReStart(void){
	SSP2CON2bits.RSEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;		
}

/*******************************************************************************
 * Function:        void I2C_Stop(void)
 * Description:     Sends stop bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_Stop(void){
	SSP2CON2bits.PEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
}

/*******************************************************************************
 * Function:        void I2C_Send_ACK(void)
 * Description:     Sends ACK bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_Send_ACK(void){
	SSP2CON2bits.ACKDT = I2C_LOW;			
	SSP2CON2bits.ACKEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
}

/*******************************************************************************
 * Function:        void I2C_Send_NACK(void)
 * Description:     Sends NACK bit sequence
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void I2C_Send_NACK(void){
	SSP2CON2bits.ACKDT = I2C_HIGH;			
	SSP2CON2bits.ACKEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
}

/*******************************************************************************
 * Function:        unsigned char I2C_Write_Byte(unsigned char BYTE)
 * Description:     Transfers one byte
 * Precondition:    None
 * Parameters:      BYTE = Value for slave device
 * Return Values:   Return ACK/NACK from slave
 * Remarks:         None
 ******************************************************************************/
unsigned char I2C_Write_Byte(unsigned char BYTE){
	SSP2BUF = BYTE;                 
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
	return SSP2CON2bits.ACKSTAT;    
}

/*******************************************************************************
 * Function:        unsigned char I2C_Read_Byte(void)
 * Description:     Reads one byte
 * Precondition:    None
 * Parameters:      None
 * Return Values:   Return received byte
 * Remarks:         None
 ******************************************************************************/
unsigned char I2C_Read_Byte(void){
	SSP2CON2bits.RCEN = I2C_HIGH;			
	while(!PIR3bits.SSP2IF);		
	PIR3bits.SSP2IF = I2C_LOW;			
    return SSP2BUF;                 
}

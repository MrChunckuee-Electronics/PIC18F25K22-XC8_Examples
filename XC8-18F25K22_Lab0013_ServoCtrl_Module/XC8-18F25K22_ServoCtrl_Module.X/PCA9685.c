/*******************************************************************************
 *
 *                 PCA9685 Library
 *
 *******************************************************************************
 * FileName:        PCA9685.c
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez Ramírez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description:     Library PCA9685 use in PIC18 for servo controller
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
 *  v1.0.0      10/04/2019      Created library.
 *                              Based on https://forum.mikroe.com/viewtopic.php?f=88&t=69437
 *  v1.0.1      25/05/2020      Implementing on PIC18F22K22.
 ******************************************************************************/
#include "PCA9685.h"
#include "i2c.h"

#define _XTAL_FREQ  64000000

float _oscillator_freq;

/*******************************************************************************
 * Function:        void PCA9685_Init(void)
 * Description:     Init the chip with pwm frequency and MODE2 settings
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void PCA9685_Init(void){
     I2C_Start();    // Start
     I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS); // Slave Write_address
     I2C_Write_Byte(PCA9685_MODE1);           // Mode 1 ADDRESS
     I2C_Write_Byte(0b00110001);      // Sleep and change default PWM frequency
     I2C_Stop();              // Stop
     __delay_ms(1);           // Required 50 us delay
     
     I2C_Start();             // Start
     I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);   // Slave Write_address
     I2C_Write_Byte(0xFE);            // PWM frequency PRE_SCALE ADDRESS to set pwm at 50Hz
     I2C_Write_Byte(0x7A);            // Osc_clk/(4096*update_rate)=25000000/(4096*50)=122=0x7A
     I2C_Stop();               // Stop
     __delay_ms(1);            // delay at least 500 us
     
     I2C_Start();              // Start
     I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);   // Slave Write_address
     I2C_Write_Byte(PCA9685_MODE1);           // Mode 1 register ADDRESS
     I2C_Write_Byte(0b10100001);      // Set MODE1
     I2C_Stop();               // Stop
     __delay_ms(1);            // delay at least 500 us
     
     I2C_Start();             // Start
     I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);   // Slave Address
     I2C_Write_Byte(PCA9685_MODE2);           // Mode2 register ADDRESS
     I2C_Write_Byte(0b00000100);      // Set MODE2
     I2C_Stop();              //
}

/*******************************************************************************
 * Function:        void PCA9685_SetPWM(unsigned char CHANNEL, unsigned int DATA_ON, unsigned int DATA_OFF)
 * Description:     Sets the PWM output of one of the PCA9685 pins
 * Precondition:    None
 * Parameters:      CHANNEL One of the PWM output pins, from 0 to 15
 *                  DATA_ON At what point in the 4096-part cycle to turn the PWM output ON
 *                  DATA_OFF At what point in the 4096-part cycle to turn the PWM output OFF
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void PCA9685_SetPWM(unsigned char CHANNEL, unsigned int DATA_ON, unsigned int DATA_OFF){
    I2C_Start();    // Start
    I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);  // Address of selected pca9685
    I2C_Write_Byte(PCA9685_LED0_ON_L + 4 * CHANNEL);// Select slected LED ADDRESS
    I2C_Write_Byte(DATA_ON);
    I2C_Write_Byte(DATA_ON>>8);
    I2C_Write_Byte(DATA_OFF);
    I2C_Write_Byte(DATA_OFF>>8);
    I2C_Stop();     // stop
}



/*******************************************************************************
 * Function:        void PCA9685_WriteByte(unsigned char ADDR, unsigned char DATA)
 * Description:     Transfers one byte
 * Precondition:    None
 * Parameters:      ADDR is addres to write 
 *                  DATA is bayte to write
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void PCA9685_WriteByte(unsigned char ADDR, unsigned char DATA){
     I2C_Start();
     I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);
     I2C_Write_Byte(ADDR);
     I2C_Write_Byte(DATA);
     I2C_Stop();
}

/*******************************************************************************
 * Function:        unsigned char PCA9685_ReadByte(unsigned char addr)
 * Description:     Init the chip with pwm frequency and MODE2 settings
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
// Read a byte and return it's value
unsigned char PCA9685_ReadByte(unsigned char ADDR){
    unsigned char temp = 0x00;
    I2C_Start();
    I2C_Write_Byte(PCA9685_I2C_WRITE_ADDRESS);
    I2C_Write_Byte(ADDR);
    I2C_Start();
    I2C_Write_Byte(PCA9685_I2C_READ_ADDRESS);
    temp = I2C_Read_Byte();
    I2C_Stop();
    return temp;
}

/*!
 *  @brief  Sets the PWM frequency for the entire chip, up to ~1.6 KHz
 *  @param  freq Floating point frequency that we will attempt to match
 */
void PCA9685_SetPWMFreq(float freq) {
    
  // Range output modulation frequency is dependant on oscillator
    if (freq < 1)
      freq = 1;
    if (freq > 3500)
        freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)
    
    float prescaleval = ((_oscillator_freq / (freq * 4096.0)) + 0.5) - 1;
    if (prescaleval < PCA9685_PRESCALE_MIN)
        prescaleval = PCA9685_PRESCALE_MIN;
    if (prescaleval > PCA9685_PRESCALE_MAX)
        prescaleval = PCA9685_PRESCALE_MAX;
    unsigned char prescale = (unsigned char)prescaleval;


  unsigned char oldmode =  PCA9685_ReadByte(PCA9685_MODE1);
  unsigned char newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
  PCA9685_WriteByte(PCA9685_MODE1, newmode);                             // go to sleep
  PCA9685_WriteByte(PCA9685_PRESCALE, prescale); // set the prescaler
  PCA9685_WriteByte(PCA9685_MODE1, oldmode);
  __delay_ms(5);
  // This sets the MODE1 register to turn on auto increment.
  PCA9685_WriteByte(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);

}




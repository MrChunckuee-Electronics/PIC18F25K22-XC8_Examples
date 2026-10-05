/*******************************************************************************
 *
 *                  LCD Nokia5110 Library
 *
 *******************************************************************************
 * FileName:        LCDNokia5110.c , LCDNokia5110.h and font_pattern.h
 * Processor:       PICxxxxxx
 * Complier:        XC8
 * Author:          Pedro Sánchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Library to control a Nokia 5110 display with the PCD8544 
 *                  driver. The library has a function that simulates SPI 
 *                  communication, therefore any pin of the MCU can be used.
 *******************************************************************************
 * Rev.         Date            Comment (Full info on CHANGELOG)
 *  v0.0.1      20/02/2016      Creation of the library.
 *  v0.2.0      23/04/2026      Library restructuring, function renaming, and 
 *                              documentation.
 ******************************************************************************/
#include <xc.h>
#include "LCDNokia5110.h"
#include "font_pattern.h"

LCDNokia5110_t Nokia5110;

// Macros internos para manipular bits
#define SET_BIT(reg, bit)    (*(reg) |= (1 << (bit)))
#define CLR_BIT(reg, bit)    (*(reg) &= ~(1 << (bit)))

/*******************************************************************************
 * Function:        void LCDNokia5110_Initialize(LCDNokia5110_t lcd)
 * Description:     This function init the LCD, sends the magical commands to the PCD8544
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_Initialize(LCDNokia5110_t lcd){
    Nokia5110 = lcd;
    // Config output pin (clear TRIS bits)
    CLR_BIT(Nokia5110.TRIS, Nokia5110.RST);
    CLR_BIT(Nokia5110.TRIS, Nokia5110.CE);
    CLR_BIT(Nokia5110.TRIS, Nokia5110.DC);
    CLR_BIT(Nokia5110.TRIS, Nokia5110.DIN);
    CLR_BIT(Nokia5110.TRIS, Nokia5110.CLK);

    SET_BIT(Nokia5110.PORT, Nokia5110.CE); // Disable Chip Select (Idle High)

    CLR_BIT(Nokia5110.PORT, Nokia5110.RST);
    Nop(); Nop(); //Delay = 1uS when FOSC=8MHz, Datasheet twl_min=100nS
    SET_BIT(Nokia5110.PORT, Nokia5110.RST);

    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x21); // Extended commands
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0xB0); // Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x04); // Set Temp coefficient
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x14); // LCD bias mode 1:48: Try 0x13 or 0x14
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x20); // Basic commands
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x0C); // Set display control, normal mode. 0x0D for inverse
}

/*******************************************************************************
 * Function:        void LCDNokia5110_Write(uint8_t mode, char data)
 * Description:     There are two memory banks in the LCD, data/RAM and commands.
 *                  This function sets the DC pin high or low depending,
 *                  and then sends the data byte
 * Precondition:    None
 * Parameters:      mode = Say to the LCD that we are writing either data or commands
 *                  data = data for LCD
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_Write(uint8_t mode, uint8_t data){
    if(mode == NOKIA5110_isDATA) 
        SET_BIT(Nokia5110.PORT, Nokia5110.DC);
    else 
        CLR_BIT(Nokia5110.PORT, Nokia5110.DC);
    
    //Send the data or a command
    CLR_BIT(Nokia5110.PORT, Nokia5110.CE);
    LCDNokia5110_WriteSPI(data);
    SET_BIT(Nokia5110.PORT, Nokia5110.CE);
}

/*******************************************************************************
 * Function:        void LCDNokia5110_WriteSPI(char outData)
 * Description:     This function emulates the SPI communication and writes
 *                  data to the LCD
 * Precondition:    None
 * Parameters:      uint8_t outData = data for LCD
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_WriteSPI(uint8_t outData){
    for(uint8_t i = 0; i < 8; i++){
        CLR_BIT(Nokia5110.PORT, Nokia5110.CLK);
        if ((outData & 0x80) == 0) 
            CLR_BIT(Nokia5110.PORT, Nokia5110.DIN);
        else 
            SET_BIT(Nokia5110.PORT, Nokia5110.DIN);
        SET_BIT(Nokia5110.PORT, Nokia5110.CLK);
        outData <<= 1;
    }
}

/*******************************************************************************
 * Function:        void LCDNokia5110_Clear(void)
 * Description:     This function clears the LCD by writing zeros to the entire screen
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_Clear(void){
    for(int index = 0; index < (NOKIA5110_LCDWIDTH * NOKIA5110_LCDHEIGHT / 8); index++)
        LCDNokia5110_Write(NOKIA5110_isDATA, 0x00);
    LCDNokia5110_GotoXY(0, 0); //After we clear the display, return to the home position
}

/*******************************************************************************
 * Function:        void LCDNokia5110_GotoXY(uint8_t x, uint8_t y)
 * Description:     This function set x & y value
 * Precondition:    None
 * Parameters:      uint8_t x, Column pos - 0 to 83px
 *                  uint8_t y, Row pos - 0 to 5 (bank 0 to 5), 6*8 = 48px
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_GotoXY(uint8_t x, uint8_t y){
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x80 | x); // Column.
    LCDNokia5110_Write(NOKIA5110_isCOMMAND, 0x40 | y); // Row.
}

/*******************************************************************************
 * Function:        void LCDNokia5110_putC(char data)
 * Description:     This function takes in a character, looks it up in the font
 *                  table/array. And writes it to the screen. Each character
 *                  is 8 bits tall and 5 bits wide. We pad one blank column of
 *                  pixels on each side of the character for readability.
 * Precondition:    None
 * Parameters:      char data
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_putC(char data){
    LCDNokia5110_Write(NOKIA5110_isDATA, 0x00); //Blank vertical line padding
    for (int index = 0; index < 5; index++)
        LCDNokia5110_Write(NOKIA5110_isDATA, FONT_NORMAL[data - 0x20][index]);
        //0x20 is the ASCII character for Space (' '). The font table starts with this character
    LCDNokia5110_Write(NOKIA5110_isDATA, 0x00);  //Blank vertical line padding
}

/*******************************************************************************
 * Function:        void LCDNokia5110_putStr(char *dataStr, uint8_t x, uint8_t y)
 * Description:     Given a string of characters, one by one is passed to the LCD
 * Precondition:    None
 * Parameters:       uint8_t x, uint8_t y = pos init
 * Return Values:   None
 * Remarks:         None
 * ****************************************************************************/
void LCDNokia5110_putStr(char *dataStr, uint8_t x, uint8_t y){
    LCDNokia5110_GotoXY(x, y);
    while(*dataStr)
        LCDNokia5110_putC(*dataStr++);
}

/*******************************************************************************
 * Function:        void LCDNokia5110_Image(const char IMAGE[], uint8_t x, uint8_t y)
 * Description:     This takes a large array of bits and sends them to the LCD
 * Precondition:    None
 * Parameters:      uint8_t x and uint8_t y: This function assumes that the image is 84x48 px, so x&y = 0
 *                  IMAGE[] contains the bitmap to send (full screen)
 * Return Values:   None
 * Remarks:         The bit array must be vertically oriented.
 * ****************************************************************************/
void LCDNokia5110_Image(const char IMAGE[], uint8_t x, uint8_t y){
    LCDNokia5110_GotoXY(x, y);
    for (int index = 0; index < (NOKIA5110_LCDWIDTH * NOKIA5110_LCDHEIGHT / 8); index++)
        LCDNokia5110_Write(NOKIA5110_isDATA, IMAGE[index]);
}

/*******************************************************************************
 * Function:        void LCDNokia5110_Bitmap(const char BITMAP[], uint8_t x, uint8_t y, uint8_t width, uint8_t height)
 * Description:     This takes a array of bits and sends them to the LCD
 * Precondition:    None
 * Parameters:      uint8_t x & uint8_t y: Initial image position
 *                  uint8_t width & uint8_t height: Image size
 *                  IMAGE[] contains the bitmap to send
 * Return Values:   None
 * Remarks:         The bit array must be vertically oriented.
 *                  The function works correctly if "height" is a multiple of 8 (8, 16, 24, 32 or 40).
 * ****************************************************************************/
void LCDNokia5110_Bitmap(const char BITMAP[], uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    for (uint8_t p = 0; p < (height / 8); p++){
        LCDNokia5110_GotoXY(x, y + p);
        for (uint8_t i = 0; i < width; i++){
            LCDNokia5110_Write(NOKIA5110_isDATA, BITMAP[i + (p * width)]);
        }
    }
}

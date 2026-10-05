#ifndef LCDNOKIA5110_H
#define	LCDNOKIA5110_H

#ifdef	__cplusplus
extern "C" {
#endif


/********** L C D * S T R U C T **********/
typedef struct{
    volatile uint8_t* TRIS;  // Puntero al registro TRIS (e.g., &TRISB)
    volatile uint8_t* PORT;  // Puntero al registro PORT (e.g., &PORTB)
    uint8_t RST;             // Pin Reset
    uint8_t CE;              // Pin Chip Enable
    uint8_t DC;              // Pin Data/Command
    uint8_t DIN;             // Pin Data In (SDOUT)
    uint8_t CLK;             // Pin Serial Clock
} LCDNokia5110_t;

/********** L C D * D E F I N E S **********/
#define NOKIA5110_isCOMMAND     0
#define NOKIA5110_isDATA        1
#define NOKIA5110_LCDWIDTH      84
#define NOKIA5110_LCDHEIGHT     48

/********** P R O T O T Y P E S **********/
void LCDNokia5110_Initialize(LCDNokia5110_t lcd);
void LCDNokia5110_Write(uint8_t mode, uint8_t data);
void LCDNokia5110_WriteSPI(uint8_t outData);
void LCDNokia5110_Clear(void);
void LCDNokia5110_GotoXY(uint8_t x, uint8_t y);
void LCDNokia5110_putC(char data);
void LCDNokia5110_putStr(char *dataStr, uint8_t x, uint8_t y);
void LCDNokia5110_Image(const char IMAGE[], uint8_t x, uint8_t y);
void LCDNokia5110_Bitmap(const char BITMAP[], uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    

#ifdef	__cplusplus
}
#endif

#endif	/* LCDNOKIA5110_H */


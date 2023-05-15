/*******************************************************************************
 *
 *      Driver para control de 7 segmentos usando el 74HC595
 *
 *******************************************************************************
 * Rev.:        v1.0.0         
 * Date:        02/03/2023
 * Author:      Pedro S�nchez (MrChunckuee)  http://mrchunckuee.blogspot.com/     
 * Description: - Creaci�n de la libreria y prueba de las funciones implementadas 
 *                por @Cyb3rn0id https://www.settorezero.com/wordpress/pilotare-display-led-7-segmenti-mediante-74hc595-esempio-con-pic-e-arduino/
 ******************************************************************************/

#include "main.h"


/*******************************************************************************
 * Function:        void DISPLAY_Set(uint8_t D1, uint8_t D2)
 * Description:     Carga los valores al array que controla todo el modulo de displays, 
 *                  esta pensado para un maximo de 2 digitos.
 * Precondition:    Asiganar el valor correcto a la variable NUMBERS_OF_DISPLAYS en Display7seg_74HC595.h
 * Parameters:      Los parametros recibidos son el valor independiente de cada digito,
 *                  el orden de estos es de izquierda a derecha.
 *                  D1 | D2
 * Return Values:   None
 * Remarks:         Ejemplo de uso, si quieres mostrar 5E en los displays
 *                  Tenemos que NUMBERS_OF_DISPLAYS = 3
 *                  Entonces debes DISPLAY_set(5, DIG_E);
 *                  o bien puedes usar DISPLAY_set(5, 14); ya que en el array de digitos 14 esla letra E
 ******************************************************************************/
void DISPLAY_Set(uint8_t D1, uint8_t D2){
    switch(NUMBERS_OF_DISPLAYS){
        case 1:
            display_values[1]=digits[DIG_OFF];
            display_values[0]=digits[D1];
            break;
        case 2:
            display_values[1]=digits[D1];
            display_values[0]=digits[D2];
            break;
        default:
            break;
    }
    DISPLAY_Update();
}

/*******************************************************************************
 * Function:        void DISPLAY_Reset(void)
 * Description:     Clean all displays.
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void DISPLAY_Reset(void){
    DISPLAY_Set(DIG_OFF, DIG_OFF); 
}
 
/*******************************************************************************
 * Function:        void DISPLAY_Write(uint8_t num)
 * Description:     Writes an unsigned integer to display.
 * Precondition:    None
 * Parameters:      uint8_t num
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void DISPLAY_Write(uint8_t num){
    uint8_t digit1=DIG_OFF;
    uint8_t digit2=DIG_OFF;
//    uint8_t digit3=DIG_OFF;
    
    if (num<10){
        digit2=num;
    }
    else if (num<100){
        digit2=num%10;
        digit1=num/10;  
    }
//  else
//    {
//    digit3=num%10;
//    digit2=(num%100)/10;
//    digit1=num/100;  
//    }
  DISPLAY_Set(digit1, digit2);
  }

/*******************************************************************************
 * Function:        void Driver_74HC595_shiftOut(uint8_t data)
 * Description:     Transfer datas from array in ram to shift registers
 * Precondition:    Need set value in "display_values" array
 * Parameters:      none
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void DISPLAY_Update(void){ 
    if((NUMBERS_OF_DISPLAYS % 2) == 0){
        for (int8_t i=0; i<NUMBERS_OF_DISPLAYS; i++){
            DRIVER_74HC595_ShiftOut(display_values[i]);
        }
    }
    else{
        for (int8_t i=0; i<=NUMBERS_OF_DISPLAYS; i++){
            DRIVER_74HC595_ShiftOut(display_values[i]);
        }
    }
    // strobe signal on latch pin 
    LAT_74HC595_SetHigh();
    LAT_74HC595_SetLow();
  }

/*******************************************************************************
 * Function:        void DRIVER_74HC595_ShiftOut(uint8_t data)
 * Description:     Transmit an integer one bit at time, from MSB to LSB
 * Precondition:    None
 * Parameters:      uint8_t data
 * Return Values:   None
 * Remarks:         
 ******************************************************************************/
void DRIVER_74HC595_ShiftOut(uint8_t data){
    // scan bits from left to right
    for (uint8_t i=0; i<8; i++){
        if (data & (0x80>>i)){
            DATA_74HC595_SetHigh();
            }
        else{
            DATA_74HC595_SetLow();
            }
        // clock strobe
        CLK_74HC595_SetHigh();
        CLK_74HC595_SetLow();
    }
}
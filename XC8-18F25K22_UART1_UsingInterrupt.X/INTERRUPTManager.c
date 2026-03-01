/*
 * File:   INTERRUPTManager.c
 * Author: MrChunckuee
 *
 * Created on 23 de febrero de 2026, 11:36 PM
 */

#include "main.h"

/*******************************************************************************
 * Function:        void INTERRUPT_Initialize(void)
 * Description:     This function initializes the interrupt service routines
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:
 ******************************************************************************/
void INTERRUPT_Initialize(void)
{
    RCONbits.IPEN = 1;  //Interrupt Priority Enable bit, Enable priority level on interrupt
}

/*******************************************************************************
 * Function:        void INTERRUPT_InterruptEnable(void)
 * Description:     This function enable the interrupt service routines
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:
 ******************************************************************************/
void INTERRUPT_InterruptEnable(void)
{
    //INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable(); 
}

/*******************************************************************************
 * Function:        void INTERRUPT_InterruptDisable(void)
 * Description:     This function disabled the interrupt service routines
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:
 ******************************************************************************/
void INTERRUPT_InterruptDisable(void)
{
    //INTERRUPT_GlobalInterruptDisable();
    INTERRUPT_GlobalInterruptHighDisable();
    INTERRUPT_GlobalInterruptLowDisable();
}

/*******************************************************************************
 * Function:        void __interrupt(high_priority) INTERRUPT_HighManager(void)
 * Description:     Rutina de atencion para las interrupciones de alta prioridad
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:
 ******************************************************************************/
void __interrupt(high_priority) INTERRUPT_HighManager(void)
{
    // Interrupción de Recepción
    if(PIE1bits.RC1IE == 1 && PIR1bits.RC1IF == 1)
    {
        UART1_RxISR();
    }
    // Interrupción de Transmisión
    if(PIE1bits.TX1IE == 1 && PIR1bits.TX1IF == 1)
    {
        UART1_TxISR();
    }
}

/*******************************************************************************
 * Function:        void __interrupt(low_priority) INTERRUPT_LowManager(void)
 * Description:     Rutina de atencion para las interrupciones de baja prioridad
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:
 ******************************************************************************/
void __interrupt(low_priority) INTERRUPT_LowManager(void)
{
    
}

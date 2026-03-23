/* 
 * File:   INTERRUPTManager.h
 * Author: MrChunckuee
 *
 * Created on 23 de febrero de 2026, 11:37 PM
 */

#ifndef INTERRUPTMANAGER_H
#define	INTERRUPTMANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will enable global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptEnable();
     */
    #define INTERRUPT_GlobalInterruptEnable() (INTCONbits.GIE = 1)

    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will disable global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptHighDisable();
     */
    #define INTERRUPT_GlobalInterruptHighDisable() (INTCONbits.GIE = 0)
    
    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will enable high priority global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptHighEnable();
     */
    #define INTERRUPT_GlobalInterruptHighEnable() (INTCONbits.GIEH = 1)

    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will disable high priority global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptHighDisable();
     */
    #define INTERRUPT_GlobalInterruptHighDisable() (INTCONbits.GIEH = 0)

    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will enable low priority global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptLowEnable();
     */
    #define INTERRUPT_GlobalInterruptLowEnable() (INTCONbits.GIEL = 1)

    /**
     * @Param
        none
     * @Returns
        none
     * @Description
        This macro will disable low priority global interrupts.
     * @Example
        INTERRUPT_GlobalInterruptLowDisable();
     */
    #define INTERRUPT_GlobalInterruptLowDisable() (INTCONbits.GIEL = 0)

    
    void INTERRUPT_Initialize(void);
    void INTERRUPT_InterruptEnable(void);
    void INTERRUPT_InterruptDisable(void);
    


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTMANAGER_H */


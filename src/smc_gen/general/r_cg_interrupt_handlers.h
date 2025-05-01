/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_cg_interrupt_handlers.h
* Version          : 1.0.50
* Device(s)        : R5F5671EHxLE
* Description      : This file declares interrupt handlers.
***********************************************************************************************************************/

#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* CMT0 CMI0 */
void r_Config_CMT0_cmi0_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(CMT0,CMI0))));

/* SCI0 RXI0 */
void r_Config_SCI0_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI0,RXI0))));

/* SCI0 TXI0 */
void r_Config_SCI0_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI0,TXI0))));

/* SCI1 RXI1 */
void r_Config_SCI1_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,RXI1))));

/* SCI1 TXI1 */
void r_Config_SCI1_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,TXI1))));

/* SCI2 RXI2 */
void r_Config_SCI2_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI2,RXI2))));

/* SCI2 TXI2 */
void r_Config_SCI2_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI2,TXI2))));

/* ICU IRQ1 */
void r_Config_ICU_irq1_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(ICU,IRQ1))));

/* ICU IRQ10 */
void r_Config_ICU_irq10_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(ICU,IRQ10))));

/* ICU IRQ14 */
void r_Config_ICU_irq14_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(ICU,IRQ14))));

/* SCI10 RXI10 */
void r_Config_SCI10_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI10,RXI10))));

/* SCI10 TXI10 */
void r_Config_SCI10_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI10,TXI10))));

/* S12AD S12ADI */
void r_Config_S12AD0_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(PERIB,INTB183))));

/* S12AD1 S12ADI1 */
void r_Config_S12AD1_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(PERIB,INTB186))));

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

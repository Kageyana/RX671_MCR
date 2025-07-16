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
/* SCI1 RXI1 */
void r_Config_SCI1_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,RXI1))));

/* SCI1 TXI1 */
void r_Config_SCI1_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI1,TXI1))));

/* ICU IRQ3 */
void r_Config_ICU_irq3_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(ICU,IRQ3))));

/* ICU IRQ9 */
void r_Config_ICU_irq9_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(ICU,IRQ9))));

/* SCI4 RXI4 */
void r_Config_SCI4_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI4,RXI4))));

/* SCI4 TXI4 */
void r_Config_SCI4_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI4,TXI4))));

/* SCI10 RXI10 */
void r_Config_SCI10_receive_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI10,RXI10))));

/* SCI10 TXI10 */
void r_Config_SCI10_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(SCI10,TXI10))));

/* RSPI2 SPTI2 */
void r_Config_RSPI2_transmit_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(RSPI2,SPTI2))));

/* TPU5 TGI5A */
void r_Config_TPU5_tgi5a_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(PERIB,INTB164))));

/* S12AD1 S12ADI1 */
void r_Config_S12AD1_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(PERIB,INTB186))));

/* RSPI2 SPCI2 */
void r_Config_RSPI2_communication_end_interrupt(void) __attribute__ ((interrupt(".rvectors",VECT(PERIA,INTA254))));

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : sbrk.c
* Description  : Configures the MCU heap memory.  The size of the heap is defined by the macro HEAPSIZE below.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 3.00     Merged processing of all devices.
*                               Added support for GNUC and ICCRX.
*                               Fixed coding style.
*         : 26.07.2019 3.01     Fixed coding style.
*         : 26.02.2025 3.02     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "sbrk.h"

/* Only use this file if heap is enabled in r_bsp_config. */
#if BSP_CFG_HEAP_BYTES > 0

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

#if defined(__CCRX__) || defined(__GNUC__)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Declare memory heap area */
static u_heap_type_t s_heap_area;

/* End address allocated by sbrk (CC-RX and GNURX+NEWLIB) */
static int8_t *sp_brk=(int8_t *)&s_heap_area;

#if defined(__GNUC__)
/* Start address of allocated heap area (GNURX+OPTLIB only) */
int8_t *_heap_of_memory=(int8_t *)&s_heap_area;
/* End address of allocated heap area (GNURX+OPTLIB only) */
int8_t *_last_heap_object=(int8_t *)&s_heap_area;
#endif /* defined(__GNUC__) */

/***********************************************************************************************************************
* Function name: sbrk
* Description  : This function configures MCU memory area allocation. (CC-RX and GNURX+NEWLIB)
* Arguments    : size - 
*                    assigned area size
* Return value : Start address of allocated area (pass)
*                -1 (failure)
***********************************************************************************************************************/
int8_t  *sbrk(size_t size)
{
    int8_t  *p_area;

    if ((sp_brk + size) > (s_heap_area.heap + BSP_CFG_HEAP_BYTES))
    {
        /* Empty area size  */
        p_area = (int8_t *)-1;
    }
    else
    {
        /* Area assignment */
        p_area = sp_brk;

        /* End address update */
        sp_brk += size;
    }

    /* Return result */
    return p_area;
} /* End of function sbrk() */

#if defined(__GNUC__)
/***********************************************************************************************************************
* Function name: _top_of_heap
* Description  : This function returns end address of reserved heap area. (GNURX+OPTLIB only)
* Arguments    : none
* Return value : End address of reserved heap area
***********************************************************************************************************************/
int8_t *_top_of_heap(void)
{
    return (int8_t *)(s_heap_area.heap + BSP_CFG_HEAP_BYTES);
} /* End of function End of function sbrk()() */
#endif /* defined(__GNUC__) */

#endif /* defined(__CCRX__), defined(__GNUC__) */

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

#endif /* BSP_CFG_HEAP_BYTES */


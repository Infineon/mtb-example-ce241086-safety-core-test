/*******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for core peripheral self tests 
 *              for PSOC Control C3 MCU.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *******************************************************************************/


/*******************************************************************************
 * Header Files
 *******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "mtb_hal.h"
#include <stdio.h>
#include "self_test.h"

/*******************************************************************************
 * Macros
 *******************************************************************************/
uint8_t *res_cause  = (uint8_t*)0x40260800;

/*******************************************************************************
 * Global Variables
 *******************************************************************************/

/* For the Retarget -IO (Debug UART) usage */
static cy_stc_scb_uart_context_t    DEBUG_UART_context;           /** UART context */
static mtb_hal_uart_t               DEBUG_UART_hal_obj;           /** Debug UART HAL object  */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/*******************************************************************************
 * Function Definitions
 *******************************************************************************/

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 * Summary:
 * This is the main function. It initialize the device and board peripherals
 * and retarget-io for prints and calls the test APIs for testing the core
 * peripherlas
 * 
 * Parameters:
 *  void
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Debug UART init */
    result = (cy_rslt_t)Cy_SCB_UART_Init(DEBUG_UART_HW, &DEBUG_UART_config, &DEBUG_UART_context);

    /* UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SCB_UART_Enable(DEBUG_UART_HW);

    /* Setup the HAL UART */
    result = mtb_hal_uart_setup(&DEBUG_UART_hal_obj, &DEBUG_UART_hal_config, &DEBUG_UART_context, NULL);

    /* HAL UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    result = cy_retarget_io_init(&DEBUG_UART_hal_obj);

    /* HAL retarget_io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    printf("\x1b[2J\x1b[;H");

    printf("****************** "
            "Class-B Safety Test for PSoC Control C3: Core Peripheral Resources "
            "****************** \r\n\n");

    printf("------------------------------------------------------- \r\n");
    printf("| #   | IP under test                   | Test Status | \r\n");
    printf("------------------------------------------------------- \r\n");

    /* Start Up Test */
    Start_Up_Test();
    /* Program counter Test */
    ret = SelfTest_PC();
    PRINT_TEST_RESULT(ip_index++,"Program Counter Test",ret);

    /* CPU Registers Test*/
    ret = SelfTest_CPU_Registers();
    PRINT_TEST_RESULT(ip_index++,"CPU Register Test", ret);

    /* Program Flow Test*/
    ret = SelfTest_PROGRAM_FLOW();
    PRINT_TEST_RESULT(ip_index++,"Program Flow Test", ret);

    /* Watch Dog Timer Test */
    Wdt_Test();

    /* GPIO Test */
    IO_Test();

    /* FPU Test */
    FPU_Test();

    /* DMA DW Test */
    DMA_DW_Test();

    /* IPC Test */
    ret = SelfTest_IPC();
    PRINT_TEST_RESULT(ip_index++,"IPC Test", ret);

    /* Clock Test */
    Clock_Test();

    /* Interrupt Test */
    Interrupt_Test();

    /* SRAM Memory Test */
    SRAM_March_Test();

    /* Stack Memory Test */
    Stack_March_Test();

    /* Stack Overflow and Underflow Test */
    Stack_Memory_Test();

    /* Flash Test */
    Flash_Test();

    printf("------------------------------------------------------- \r\n\n");
    printf("END of the Core CPU Test.\r\n\n");
    printf("Total number of IPs covered in the Test      %d\r\n",--ip_index);

    for (;;)
    {

    }
}

/* [] END OF FILE */

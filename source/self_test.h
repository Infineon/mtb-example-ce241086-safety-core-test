/******************************************************************************
 * File Name:   self_test.h
 *
 * Description: This file is the public interface of self_test.c source file.
 *
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
 * Include guard
 *******************************************************************************/
#ifndef SELF_TEST_H_
#define SELF_TEST_H_

#include "SelfTest.h"

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
extern uint16_t test_counter;
/*Index for IPs*/
extern uint8_t ip_index;
/* SelfTest API return status */
extern uint8_t ret;

/*******************************************************************************
 * Macros
 *******************************************************************************/
#define MAX_INDEX_VAL (0xFFF0u)

#define CUSTOM_DELAY_VAL (500u)

#define MARCH         SRAM_MARCH_TEST_MODE
#define GALPAT        SRAM_GALPAT_TEST_MODE
#define TEST_MODE     MARCH

#define DEVICE_SRAM_BASE     (0x34000000)
#define DEVICE_STACK_SIZE    (0x1000)
#define DEVICE_SRAM_SIZE     (0x00010000)
#define DEVICE_STACK_BASE    (DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)
#define BLOCK_SIZE                1024
#define BUFFER_SIZE               4096

#define ALT_STACK_BASE    (0x3400F7FC)  /* Alt stack Base is within stack limit */
#define TEST_STACK_SIZE    (0x800) /* Half of the Total stack size */


#define PATTERN_BLOCK_SIZE (8u)

/* Print Test Result*/
#define PRINT_TEST_RESULT(index, test_name, status) \
        do { \
            if (OK_STATUS == ret) { \
                /* Process success */ \
                printf("| %-4d| %-32s| %-12s|\r\n",index,test_name,"SUCCESS"); \
            } \
else if (PASS_COMPLETE_STATUS == ret) { \
    /* Process status */ \
    printf("| %-4d| %-32s| %-12s|\r\n",index,test_name,"SUCCESS"); \
    break; \
} \
else if (PASS_STILL_TESTING_STATUS == ret) { \
    /* Print test counter */ \
    printf("| %-4d| %-32s| %-12s|count=%d\r",index,test_name,"IN PROGRESS", test_counter); \
} \
else { \
    /* Process error */ \
    printf("| %-4d| %-32s| %-12s|\t\t",index,test_name,"ERROR"); \
} \
} while (0)

#if COMPONENT_CAT1C
    /* Enable WWDT self test for XMC devices*/
#define WWDT_SELF_TEST_ENABLE (0u)
#endif

    /* Waiting time, in milliseconds, for proper start-up of ILO */
#define ILO_START_UP_TIME              (2U)

#define PATTERN_BLOCK_SIZE (8u)


/***************************************
* Initial Parameter Constants
***************************************/

/** Stack test pattern */
#define STACK_TEST_PATTERN        0x55AAu

/*The size of RAM/ STACK block to be tested. */
#define BLOCK_SIZE                1024

/*The size of buffer which is used to store/restore. */
#define BUFFER_SIZE               4096

#define DEVICE_SRAM_BASE     (0x34000000)
#define DEVICE_STACK_SIZE    (0x1000)
#define DEVICE_SRAM_SIZE     (0x00010000)
#define DEVICE_STACK_BASE    (DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)
#define BLOCK_SIZE                1024

#define PATTERN_BLOCK_SIZE (8u)

    /* Start of Stack address excluding the block size to store pattern */
#define DEVICE_STACK_BASE           (DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)

    /* End of Stack address excluding the block size to store pattern */
#define DEVICE_STACK_END            (uint32_t)(DEVICE_STACK_BASE - DEVICE_STACK_SIZE + STACK_TEST_BLOCK_SIZE)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void IO_Test(void);
void Clock_Test(void);
void Clock_Test_Init(void);
void Interrupt_Test(void);
void Interrupt_Test_Init(void);
void Flash_Test(void);
void Wdt_Test(void);
void FPU_Test(void);
void DMA_DW_Test(void);
void Start_Up_Test(void);
void Stack_March_Test(void);
void SRAM_March_Test(void);
void Stack_Memory_Test(void);

#if defined (__ICCARM__)
void IAR_Flash_Init();
#endif

#endif /* SELF_TEST_H_ */


/* [] END OF FILE */

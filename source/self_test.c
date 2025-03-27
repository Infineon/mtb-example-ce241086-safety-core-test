/******************************************************************************
 * File Name:   self_test.h
 *
 * Description: This file is the self_test.c source file.
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
#include <stdlib.h>
#include <stdio.h>

#include "self_test.h"

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
/* SelfTest API return status */
uint8_t ret = 0u;
/*Index for IPs*/
uint8_t ip_index = 1u;

uint16_t test_counter = 0u;

/* Array to set shifts for March RAM test. */
uint8_t sram_restore_buff[BUFFER_SIZE] = {0u};

#if defined (__ICCARM__)
#if (FLASH_TEST_MODE == FLASH_TEST_FLETCHER64)
CY_SECTION(".flash_checksum") const uint64_t flash_StoredCheckSum = 0x75207b14f910cb27;
#endif
#if (FLASH_TEST_MODE == FLASH_TEST_CRC32)
CY_SECTION(".flash_checksum") const uint64_t flash_StoredCheckSum = 0x75207b14f910cb27;
#endif
#else
#if (FLASH_TEST_MODE == FLASH_TEST_FLETCHER64)
CY_SECTION(".flash_checksum")  uint64_t flash_StoredCheckSum = 0x75207b14f910cb27;
#endif
#if (FLASH_TEST_MODE == FLASH_TEST_CRC32)
CY_SECTION(".flash_checksum") uint64_t flash_StoredCheckSum = 0x75207b14f910cb27;
#endif
#endif

/*******************************************************************************
 * Macros
 *******************************************************************************/

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

 /*****************************************************************************
* Function Name: IAR_Flash_Init
******************************************************************************
* Summary:
* The function ensures that compiler optimizations are not done for IAR
* compiler in the release mode.
*
* Parameters:
*  void
*
* Return:
*  void
*****************************************************************************/

#if defined (__ICCARM__)
#pragma optimize=none
void IAR_Flash_Init()
{
    SelfTest_Flash_init(CY_FLASH_BASE,FLASH_END_ADDR,flash_StoredCheckSum);
}
#endif
/*****************************************************************************
 * Function Name: Stack_March_Test
 ******************************************************************************
 * Summary:
 * Stack March Test: Testing Stack using March Self Tests.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void Stack_March_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

    __disable_irq();

    ret = SelfTest_SRAM_Stack((uint8_t *)DEVICE_STACK_BASE,(uint32_t)TEST_STACK_SIZE,(uint8_t *)ALT_STACK_BASE);
    __enable_irq();


    /*Process error*/
    PRINT_TEST_RESULT(ip_index,"Stack March Test", ret);
    ip_index++;

}
/*****************************************************************************
* Function Name: SRAM_March_Test
******************************************************************************
* Summary:
* SRAM March Test: Testing SRAM using March Self Tests...
*
* Parameters:
*  void
*
* Return:
*  void
*****************************************************************************/
void SRAM_March_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
    __disable_irq();

    ret = SelfTest_SRAM(TEST_MODE,(uint8_t *)DEVICE_SRAM_BASE,BLOCK_SIZE,(uint8_t *)sram_restore_buff,BUFFER_SIZE);

    __enable_irq();

    /*Process error*/
    char * test_name = (TEST_MODE)? "SRAM GALPAT Test" : "SRAM March Test";
    PRINT_TEST_RESULT(ip_index,test_name, ret);
    ip_index++;

}

/*****************************************************************************
 * Function Name: Memory_Test
 ******************************************************************************
 * Summary:
 * Memory Test: Testing memory using Self Tests...
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void Stack_Memory_Test(void)
{
    /* Init Stack SelfTest */
    SelfTests_Init_Stack_Range((uint16_t*)DEVICE_STACK_BASE, DEVICE_STACK_SIZE, PATTERN_BLOCK_SIZE);

    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
    /*******************************/
    /* Run Stack Self Test...      */
    /*******************************/
    uint8_t ret = SelfTests_Stack_Check_Range((uint16_t*)DEVICE_STACK_BASE, DEVICE_STACK_SIZE);
    if ((ERROR_STACK_OVERFLOW & ret))
    {
        /* Process error */
        PRINT_TEST_RESULT(ip_index,"Stack Overflow Test", ret);
    }
    else if ((ERROR_STACK_UNDERFLOW & ret))
    {
        ret = ERROR_STATUS;
        /* Process error */
        PRINT_TEST_RESULT(ip_index,"Stack Underflow Test", ret);
    }

    else
    {
        PRINT_TEST_RESULT(ip_index,"Stack Memory Test", ret);
    }
    Cy_SysLib_Delay(CUSTOM_DELAY_VAL);
    ip_index++;
}

/*****************************************************************************
 * Function Name: Start_Up_Test
 ******************************************************************************
 * Summary:
 * Start Up Test : This function checks the startup configuration registers.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void Start_Up_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

#if (STARTUP_CFG_REGS_MODE == CFG_REGS_TO_FLASH_MODE)

    /*******************************/
    /* Save Start-Up registers...  */
    /*******************************/
    if (CY_FLASH_DRV_SUCCESS  != SelfTests_Save_StartUp_ConfigReg())
    {
        /* Process error */
        printf("Error: Can't save Start-Up Config Registers\r\n");
    }

#endif /* End (STARTUP_CFG_REGS_MODE == CFG_REGS_TO_FLASH_MODE) */
    /**********************************/
    /* Run Start-Up regs Self Test... */
    /**********************************/
    ret = SelfTests_StartUp_ConfigReg();

    /* Process error */
    PRINT_TEST_RESULT(ip_index++,"Start-Up Register Test",ret);
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

}

/*****************************************************************************
 * Function Name: Wdt_Test
 ******************************************************************************
 * Summary:
 * Wdt Test : This function implements the watchdog and windowed watchdog
 * functional test.
 * Note: Only the XMC device supports the windowed watchdog feature.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void Wdt_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

    ret = SelfTest_WDT();
    PRINT_TEST_RESULT(ip_index++,"Watchdog Test", ret);


}

/*****************************************************************************
 * Function Name: FPU_Test
 ******************************************************************************
 * Summary:
 * FPU Test : The FPU registers test detects stuck-at faults in the FPU by
 * using the checkerboard test.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void FPU_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

    ret = SCB_GetFPUType();

    /**********************************/
    /* Run FPU Registers Self Test... */
    /**********************************/
    ret = SelfTest_FPU_Registers();
    PRINT_TEST_RESULT(ip_index++,"FPU Register Test", ret);

}

/*****************************************************************************
 * Function Name: IO_Test
 ******************************************************************************
 * Summary:
 * IO Test : Testing IO functionality by writing 1/0 to each pin and
 * then reading it back.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void IO_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
    ret = SelfTest_IO();

    if (OK_STATUS != ret)
    {
        ret = ERROR_STATUS;
    }
    PRINT_TEST_RESULT(ip_index++,"GPIO Test",ret);
}

/*****************************************************************************
 * Function Name: DMA_DW_Test
 ******************************************************************************
 * Summary:
 * DMA DW Test :
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void DMA_DW_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
    /**********************************/
    /* Run DMA DW Self Test... */
    /**********************************/
    ret = SelfTest_DMA_DW(DMA_DW_HW, DMA_DW_CHANNEL, &DMA_DW_Descriptor_0, &DMA_DW_Descriptor_1,&DMA_DW_Descriptor_0_config,  &DMA_DW_Descriptor_1_config,&DMA_DW_channelConfig, TRIG_OUT_MUX_0_PDMA0_TR_IN0);





    PRINT_TEST_RESULT(ip_index++,"DMA DW Test", ret);

}
/*****************************************************************************
 * Function Name: Clock_Test_Init
 ******************************************************************************
 * Summary:
 * This function initializes the WDT block and initialize the timer interrupt
 * for the Self test.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 *****************************************************************************/
void Clock_Test_Init(void)
{
    cy_en_tcpwm_status_t tcpwm_res;
    cy_en_sysint_status_t sysint_res;

    /* Unlock WDT */
    Cy_WDT_Unlock();

    /* Step 2- Write the ignore bits - operate with only 14 bits */
    Cy_WDT_SetIgnoreBits(IGNORE_BITS_CLK_TEST);
    if (Cy_WDT_GetIgnoreBits() != IGNORE_BITS_CLK_TEST)
    {
        CY_ASSERT(0);
    }

    /* Clear match event interrupt, if any */
    Cy_WDT_ClearInterrupt();


    /* Enable WDT */
    Cy_WDT_Enable();

    if(Cy_WDT_IsEnabled() == false)
    {
        CY_ASSERT(0);
    }

    /* Lock WDT configuration */
    Cy_WDT_Lock();

    tcpwm_res = Cy_TCPWM_Counter_Init(CYBSP_CLOCK_TEST_TIMER_HW, CYBSP_CLOCK_TEST_TIMER_NUM, &CYBSP_CLOCK_TEST_TIMER_config);
    if(CY_TCPWM_SUCCESS != tcpwm_res)
    {
        CY_ASSERT(0);
    }


    cy_stc_sysint_t intrCfg =
    {
            /*.intrSrc =*/ CYBSP_CLOCK_TEST_TIMER_IRQ, /* Interrupt source is Timer interrupt */
            /*.intrPriority =*/ 3UL   /* Interrupt priority is 3 */
    };

    sysint_res = Cy_SysInt_Init(&intrCfg, SelfTest_Clock_ISR_TIMER);


    if(CY_SYSINT_SUCCESS != sysint_res)
    {
        CY_ASSERT(0);
    }

    /* Enable Interrupt */
    NVIC_EnableIRQ(intrCfg.intrSrc);

    /* Enable timer */
    Cy_TCPWM_Counter_Enable(CYBSP_CLOCK_TEST_TIMER_HW, CYBSP_CLOCK_TEST_TIMER_NUM);

    Cy_TCPWM_SetInterruptMask(CYBSP_CLOCK_TEST_TIMER_HW, CYBSP_CLOCK_TEST_TIMER_NUM, CY_TCPWM_INT_ON_TC);

}
/*****************************************************************************
* Function Name: Clock_Test
******************************************************************************
* Summary:
* Clock Test : Testing clock frequency using Independent Time slot
* monitoring technique
*
* Parameters:
*  void
*
* Return:
*  void
*****************************************************************************/
void Clock_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }

    Clock_Test_Init();

    for (;;)
    {
        ret = SelfTest_Clock(CYBSP_CLOCK_TEST_TIMER_HW, CYBSP_CLOCK_TEST_TIMER_NUM);
        PRINT_TEST_RESULT(ip_index,"Clock Test", ret);
        if (PASS_STILL_TESTING_STATUS != ret) {
            break;
        }

        test_counter++;
        if (test_counter > MAX_INDEX_VAL){
            test_counter = 0u;
        }
    }
    Cy_SysLib_ClearResetReason();
    /* Either you need to clear WDT interrupt periodically or
     * disable it to ensure no WDT reset */
    Cy_WDT_ClearInterrupt();
    Cy_WDT_Unlock();
    Cy_WDT_Disable();
    ip_index++;
}

/*****************************************************************************
 * Function Name: Interrupt_Test
 ******************************************************************************
 * Summary:
 * Interrupt Test : Testing Interrupt controller using independent time
 * slot monitoring technique
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *****************************************************************************/
void Interrupt_Test(void)
{
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
    Interrupt_Test_Init();

    ret = SelfTest_Interrupt(CYBSP_TIMER_HW, CYBSP_TIMER_NUM);

    PRINT_TEST_RESULT(ip_index++,"Interrupt Test", ret);
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
}

/******************************************************************************
 * Function Name: Interrupt_Test_Init
 *******************************************************************************
 *
 * Summary: Initialize the Timer interrupt for Interrupt Self test.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 ******************************************************************************/
void Interrupt_Test_Init(void)
{
    cy_rslt_t result;
    cy_stc_sysint_t intrCfg =
    {
            /*.intrSrc =*/ CYBSP_TIMER_IRQ, /* Interrupt source is Timer interrupt */
            /*.intrPriority =*/ 3UL   /* Interrupt priority is 3 */
    };

    result = Cy_SysInt_Init(&intrCfg, SelfTest_Interrupt_ISR_TIMER);

    if(result != CY_SYSINT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable Interrupt */
    NVIC_EnableIRQ(intrCfg.intrSrc);

    /* Initialize TCPWM counter*/
    result = Cy_TCPWM_Counter_Init(CYBSP_TIMER_HW, CYBSP_TIMER_NUM, &CYBSP_TIMER_config);
    if(result != CY_TCPWM_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable timer */
    Cy_TCPWM_Counter_Enable(CYBSP_TIMER_HW, CYBSP_TIMER_NUM);

    Cy_TCPWM_SetInterruptMask(CYBSP_TIMER_HW, CYBSP_TIMER_NUM, CY_TCPWM_INT_ON_TC);

}

/*****************************************************************************
* Function Name: Flash_Test
******************************************************************************
* Summary:
* Flash Test : Testing the flash by comparing the stored checksum in
* flash with calculated checksum of the data stored in flash.
*
* Parameters:
*  void
*
* Return:
*  void
*****************************************************************************/
void Flash_Test(void)
{
    /* Variable for output calculated Flash Checksum */
    uint8_t flash_CheckSum_temp;
    if(ERROR_STATUS == ret)
    {
        printf("\r\n");
    }
#if defined (__ICCARM__)
    IAR_Flash_Init();
#else
    SelfTest_Flash_init(CY_FLASH_BASE,FLASH_END_ADDR,flash_StoredCheckSum);
#endif
    for(;;)
    {
        ret =  SelfTest_FlashCheckSum(FLASH_DOUBLE_WORDS_TO_TEST);
        PRINT_TEST_RESULT(ip_index,"Flash Test", ret);

        if (ERROR_STATUS == ret)
        {
#if(FLASH_TEST_MODE == FLASH_TEST_CRC32)
            printf("\r\nFLASH CRC: 0x");
#elif (FLASH_TEST_MODE == FLASH_TEST_FLETCHER64)
            printf("\tFLASH CHECKSUM: 0x");
#endif

            /* Output calculated Flash Checksum */
            for(int16_t i = sizeof(flash_CheckSum) - 1; i >= 0; i--)
            {
                flash_CheckSum_temp = (uint8_t) (flash_CheckSum >> (i*8u));
                printf("%02X", flash_CheckSum_temp);
            }
            printf("\r\n");
            break;
        }
        else if (PASS_COMPLETE_STATUS == ret) {
            break;
        }
        else
        {
            /* Do Nothing */
        }
        test_counter++;
        if (test_counter > MAX_INDEX_VAL) {
            test_counter = 0u;
        }
    }
    ip_index++;
}



/* [] END OF FILE */

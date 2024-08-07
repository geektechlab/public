/******************************************************************************
* File Name: main.c
*
* Description: This code example demonstrates displaying graphics on an E-Ink
* display using EmWin graphics library and the AppWizard GUI design tool.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
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

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ          (100000000u)

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static volatile uint8_t state_counter=0;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void oneShotCallBack( TimerHandle_t xTimer )
{
	/* Turn LED on */
	Cy_GPIO_Write(LED_RED_PORT,LED_RED_PIN, 0U);
}

void task1(void *argument)
{
	while(1)
	{
		/* Turn LED on */
		Cy_GPIO_Write(LED_RED_PORT,LED_RED_PIN, 1U);

		vTaskDelay(2000);
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		printf("idle task\r\n");
	}
}

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: int main(void)
********************************************************************************
*
* Summary: This is the main for this code example.  This function initializes
*          the BSP, creates the eInk_task and starts the scheduler.
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result ;
#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    CY_ASSERT(result == CY_RSLT_SUCCESS);


    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void)result;

    /* Enable global interrupts. */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("**********************************************************\r\n");
    printf("https://github.com/PacktPublishing/Hands-On-RTOS-with-Microcontrollers/blob/master/Chapter_8/Src/mainSemExample.c\r\n");
    printf("Give and take semaphore\r\n");
    printf("**********************************************************\r\n");

    TimerHandle_t oneShotHandle = xTimerCreate( "myOneShotTimer", /* name for timer */
    											1000/portTICK_PERIOD_MS, /* period of timer in ticks */
												pdFALSE, /* auto-reload flag */
												NULL, /* unique ID for timer */
												oneShotCallBack); /* callback function */

    /* Ensure creation of timer */
    CY_ASSERT(oneShotHandle != NULL);

    xTimerStart(oneShotHandle, 0); /* start with scheduler */

    /* Create the task */
	if (xTaskCreate(task1, "Task 1", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		/* Start the FreeRTOS scheduler. */
		vTaskStartScheduler();
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */

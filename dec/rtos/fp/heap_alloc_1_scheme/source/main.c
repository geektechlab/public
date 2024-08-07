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
/*******************************************************************************
* Macros
*******************************************************************************/
#define TASK_STACK_SIZE        (1024*10)
#define TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)
/*******************************************************************************
* Function Prototypes
********************************************************************************/

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void Task1(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("task-1\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void Task2(void *argument)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("task-2\r\n");
		//Cy_SysLib_Delay(1000U);
		//taskYIELD();
	}
}

void vApplicationIdleHook(void)
{
	while(1)
	{
		Cy_GPIO_Inv(ioss_0_port_1_pin_5_PORT,ioss_0_port_1_pin_5_PIN);
		printf("idle task\r\n");
		Cy_SysLib_Delay(1000U);
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
    printf("Heap 1 mem allocation example, page 55, 161204.pdf \r\n");
    printf("**********************************************************\r\n");

    /* Create the task */
	if (xTaskCreate(Task1, "task1", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
	{
		if (xTaskCreate(Task2, "task2", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL) == pdPASS)
		{
			/* In heap scheme 1, memory gets allocated by array ucHeap in heap_1.c and size is
			 * defined by configTOTAL_HEAP_SIZE in FreeRTOSConfig.h. This array gets allocated in .bss at compile
			 * time and then heap is emulated on that and using that heap, task stacks and TCB etc. are created
			 */

			/* This scheme is simplest amongst all except heap_3, in which it allocates memory using
			 * normal malloc. In this scheme, focus on heap and block size alignment. Also, we can
			 * check how it tracks allocated memory block sizes using stack variables. */

			/* memory only gets dynamically allocated by the kernel before the application starts
			 * to perform any real-time functionality, and the memory remains allocated for the
			 * lifetime of the application. This means the chosen allocation scheme does not have
			 * to consider any of the more complex memory allocation issues, such as determinism
			 * and fragmentation, and can instead just consider attributes such as code size and
			 * simplicity. */

			/* Some commercially critical and safety critical systems that would otherwise prohibit
			 * the use of dynamic memory allocation also have the potential to use heap_1. Critical
			 * systems often prohibit dynamic memory allocation because of the uncertainties
			 * associated with nondeterminism, memory fragmentation, and failed allocations — but
			 * heap_1 is always deterministic, and cannot fragment memory. */

			/* Fragmentation: It happens when memory has space available of demanded size, but is
			 * available in chunks of that, not contiguously. So, though space is available, it can't
			 * be allocated. It generally happens when some process/allocated block is deallocated
			 * but not merged back/reside separately from existing block
			 */

			/* Determinism: It comes into play when memory allocator has to decide which block to
			 * allocate. First fit is fast but can lead to fragmentation. To mitigate it, we can use
			 * best fit where we find smallest matching size of block to demanded memory size. Finding
			 * it is non-dterministic in time. Also, when memory block is deallocated, if nearby blocks
			 * are also available, we should merge those to make them single block. This process is also
			 * non-dterministic in time.
			 */
			void* mem = pvPortMalloc((size_t)100U);

			/* In heap scheme 1, memory free is not allowed */
			vPortFree(mem);

			/* should not use freed pointer, this just not to optimize mem variable */
			printf("%p\r\n",mem);

			/* Start the FreeRTOS scheduler. */
			vTaskStartScheduler();
		}
	}

	/* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */

/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* Configurator Backend 3.10.0
* device-db 4.100.0.4486
* mtb-pdl-cat1 3.9.0.0
*
********************************************************************************
* Copyright 2023 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_scb_uart.h"
#include "cy_sysclk.h"
#if defined (CY_USING_HAL)
    #include "cyhal_hwmgr.h"
    #include "cyhal.h"
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL_LITE)
    #include "cyhal_hw_types.h"
#endif //defined (CY_USING_HAL_LITE)
#include "cy_usbfs_dev_drv.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define UART_ENABLED 1U
#define UART_HW SCB5
#define UART_IRQ scb_5_interrupt_IRQn
#define USBD_ENABLED 1U
#define USBD_ACTIVE_ENDPOINTS_MASK 0x01U
#define USBD_ENDPOINTS_BUFFER_SIZE 512U
#define USBD_ENDPOINTS_ACCESS_TYPE 0U
#define USBD_HW USBFS0
#define USBD_HI_IRQ usb_interrupt_hi_IRQn
#define USBD_MED_IRQ usb_interrupt_med_IRQn
#define USBD_LO_IRQ usb_interrupt_lo_IRQn

extern const cy_stc_scb_uart_config_t UART_config;
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
    extern const cyhal_resource_inst_t UART_obj;
#endif //defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
#if defined(CY_USING_HAL_LITE)
    extern const cyhal_clock_t UART_clock;
#endif //defined(CY_USING_HAL_LITE)
#if defined (CY_USING_HAL)
    extern cyhal_clock_t UART_clock;
    extern const cyhal_resource_inst_t UART_clk_rsrc;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
    extern const cyhal_uart_configurator_t UART_hal_config;
#endif //defined (CY_USING_HAL) || defined(CY_USING_HAL_LITE)
extern const cy_stc_usbfs_dev_drv_config_t USBD_config;
#if defined (CY_USING_HAL)
    extern const cyhal_resource_inst_t USBD_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_peripherals(void);
void reserve_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */

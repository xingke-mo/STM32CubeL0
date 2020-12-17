/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone_LPM/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l073z_eval.h"
#include "stm32l073z_eval_io.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Uncomment the line below to select your USB clock source */
/* #define USE_USB_CLKSOURCE_CRSHSI48   1 */
#define USE_USB_CLKSOURCE_PLL        1

#if !defined (USE_USB_CLKSOURCE_PLL) && !defined (USE_USB_CLKSOURCE_CRSHSI48)
    #error "Missing USB clock definition"
#endif
/* Exported functions ------------------------------------------------------- */
void Error_Handler( void );
void usbd_hid_Process( void );
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

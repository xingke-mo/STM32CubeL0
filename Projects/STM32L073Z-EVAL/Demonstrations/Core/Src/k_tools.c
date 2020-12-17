/**
  ******************************************************************************
  * @file    k_tools.c
  * @author  MCD Application Team
  * @brief   This file contains the Hex dumps of the images available
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
#define  _K_TOOLS_C
/* Includes ------------------------------------------------------------------*/
#include <k_config.h>
#include <k_tools.h>

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_RES
  * @brief Kernel resources
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
uint16_t kTools_Buffercmp( uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength )
{
    while( BufferLength-- )
    {
        if( ( *pBuffer1 ) != *pBuffer2 )
        {
            return BufferLength;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return 0;
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

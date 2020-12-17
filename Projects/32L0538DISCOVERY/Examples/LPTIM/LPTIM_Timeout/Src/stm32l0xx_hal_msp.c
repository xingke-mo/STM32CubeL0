/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_Timeout/Src/stm32l0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *
  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]


  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit( LPTIM_HandleTypeDef *hlptim )
{
    GPIO_InitTypeDef     GPIO_InitStruct;

    /* ## - 1 - Enable LPTIM clock ############################################ */
    __HAL_RCC_LPTIM1_CLK_ENABLE();

    /* ## - 2 - Force & Release the LPTIM Periheral Clock Reset ############### */
    /* Force the LPTIM Periheral Clock Reset */
    __HAL_RCC_LPTIM1_FORCE_RESET();
    /* Release the LPTIM Periheral Clock Reset */
    __HAL_RCC_LPTIM1_RELEASE_RESET();

    /* ## - 3 - Enable & Configure LPTIM Output ############################### */
    /* Configure PB6 (LPTIM1_ETR) in alternate function (AF2), Low speed push-pull
    mode and pull-up enabled.
    Note: In order to reduce power consumption: GPIO Speed is configured in
    LowSpeed */

    /* Enable GPIO PORT B */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure PC3 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF2_LPTIM1;
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

    /* Enable and set LPTIM Interrupt to the highest priority */
    HAL_NVIC_SetPriority( LPTIM1_IRQn, 1, 0 );
    HAL_NVIC_EnableIRQ( LPTIM1_IRQn );
}

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit( void )
{
    /* NOTE : This function is eventually modified by the user */

}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit( void )
{
    /* NOTE : This function is eventually modified by the user */

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

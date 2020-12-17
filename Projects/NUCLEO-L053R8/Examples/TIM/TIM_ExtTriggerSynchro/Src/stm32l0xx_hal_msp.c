/**
  ******************************************************************************
  * @file    TIM/TIM_ExtTriggerSynchro/Src/stm32l0xx_hal_msp.c
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
#include "main.h"

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
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_OC_MspInit( TIM_HandleTypeDef *htim )
{
    GPIO_InitTypeDef   GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIMs Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM21_CLK_ENABLE();
    __HAL_RCC_TIM22_CLK_ENABLE();

    /* Enable GPIO Channels Clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*##-2- Configure I/Os #####################################################*/
    /* Configure PA.0 (TIM2_Channel1), PA.1 (TIM2_Channel2), PB.13 (TIM21_Channel1),
       PB.4 (TIM22_Channel1) in output, push-pull, alternate function mode
    */
    /* Common configuration for all channels */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    GPIO_InitStruct.Alternate = GPIO_AF6_TIM21;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

    GPIO_InitStruct.Alternate = GPIO_AF4_TIM22;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
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

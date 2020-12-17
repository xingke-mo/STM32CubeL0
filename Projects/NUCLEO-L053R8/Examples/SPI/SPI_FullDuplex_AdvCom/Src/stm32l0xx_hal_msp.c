/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_AdvCom/Src/stm32l0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @defgroup SPI_FullDuplex_AdvCom
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
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for SPI interrupt request enable if SPI_MODE_IT
  *             is enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit( SPI_HandleTypeDef *hspi )
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    SPIx_NSS_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init( SPIx_SCK_GPIO_PORT, &GPIO_InitStruct );

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Pin       = SPIx_MISO_PIN;
    HAL_GPIO_Init( SPIx_MISO_GPIO_PORT, &GPIO_InitStruct );

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_MOSI_PIN;
    HAL_GPIO_Init( SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct );

    /* SPI NSS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_NSS_PIN;
#ifdef MASTER_BOARD
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
#else /* SLAVE_BOARD */
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority( EXTI4_15_IRQn, 0x0F, 0x0F );
    HAL_NVIC_EnableIRQ( EXTI4_15_IRQn );
#endif /* MASTER_BOARD */
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init( SPIx_NSS_GPIO_PORT, &GPIO_InitStruct );

#if defined(SPI_MODE_IT)
    /*##-3- Configure the NVIC for SPI #########################################*/
    /* NVIC for SPI */
    HAL_NVIC_SetPriority( SPIx_IRQn, 0, 1 );
    HAL_NVIC_EnableIRQ( SPIx_IRQn );
#endif
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  *          - Revert NVIC configuration to its default state if SPI_MODE_IT
  *            is enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit( SPI_HandleTypeDef *hspi )
{
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit( SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN );
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit( SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN );
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit( SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN );

#if defined(SPI_MODE_IT)
    /*##-3- Disable the NVIC for SPI ###########################################*/
    HAL_NVIC_DisableIRQ( SPIx_IRQn );
#endif
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

/**
  ******************************************************************************
  * @file    COMP/COMP_PulseWidthMeasurement/Src/stm32l0xx_hal_msp.c
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

/** @defgroup COMP_PulseWidthMeasurement
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
  * @brief COMP MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit( COMP_HandleTypeDef *hcomp )
{
    GPIO_InitTypeDef      GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO clock ***************************************************/
    COMPx_GPIO_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* COMP GPIO pin configuration */
    GPIO_InitStruct.Pin = COMPx_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( COMPx_GPIO_PORT, &GPIO_InitStruct );

    /*##-3- Configure the NVIC for COMPx #######################################*/
    /* Enable the COMPx IRQ Channel */

    HAL_NVIC_SetPriority( COMPx_IRQn, 3, 0 );
    HAL_NVIC_EnableIRQ( COMPx_IRQn );

}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp: pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.
  * @retval None
  */
void HAL_COMP_MspDeInit( COMP_HandleTypeDef *hcomp )
{
    /*##-1- De-initialize peripheral GPIO ######################################*/
    /* De-initialize the COMPx GPIO pin */
    HAL_GPIO_DeInit( COMPx_GPIO_PORT, COMPx_PIN );

    /*##-2- Disable peripherals and GPIO clocks ################################*/
    /* Disable COMP peripheral clock ***************************************/
    __HAL_RCC_SYSCFG_CLK_DISABLE();

    /* Disable GPIO clock **************************************************/
    /* Note: In a real application, by disabling GPIO port, be cautious to not  */
    /* interfere with potential other peripherals using other GPIOs on the same */
    /* port.                                                                    */
    COMPx_GPIO_CLK_DISABLE();

    /*##-3- Disable the NVIC for COMP ##########################################*/
    HAL_NVIC_DisableIRQ( COMPx_IRQn );
}

/**
  * @brief TIM MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit( TIM_HandleTypeDef *htim )
{
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIM peripheral clock enable */
    TIMx_CLK_ENABLE();

    /*##-2- Configure the NVIC #################################################*/
    /* NVIC configuration for TIM interrupt */
    /* Priority: high-priority */
    HAL_NVIC_SetPriority( TIMx_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( TIMx_IRQn );

}

/**
  * @brief TIM MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspDeInit( TIM_HandleTypeDef *htim )
{
    /*##-1- Reset peripherals ##################################################*/
    TIMx_FORCE_RESET();
    TIMx_RELEASE_RESET();

    /*##-2- Disable the NVIC ###################################################*/
    HAL_NVIC_DisableIRQ( TIMx_IRQn );

}

/**
  * @brief DAC MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of peripheral
  *          - Configure the GPIO associated to the peripheral channels
  *          - Configure the DMA associated to the peripheral
  *          - Configure the NVIC associated to the peripheral interruptions
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit( DAC_HandleTypeDef *hdac )
{
    GPIO_InitTypeDef          GPIO_InitStruct;
#if defined DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
    static DMA_HandleTypeDef  DmaHandle;
#endif //DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
    /*##-1- Enable and configure GPIO for DAC_CHANNEL_1 ######################*/

    DACx_CHANNEL_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = DACx_CHANNEL_SIGNAL_FOR_TEST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( DACx_CHANNEL_SIGNAL_FOR_TEST_GPIO_PORT, &GPIO_InitStruct );


}

/**
  * @brief DAC MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of peripheral
  *          - Revert GPIO associated to the peripheral channels to their default state
  *          - Revert DMA associated to the peripheral to its default state
  *          - Revert NVIC associated to the peripheral interruptions to its default state
  * @param hadc: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspDeInit( DAC_HandleTypeDef *hdac )
{
    /*##-1- Reset peripherals ##################################################*/
    DACx_FORCE_RESET();
    DACx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* De-initialize the ADC Channel GPIO pin */
    HAL_GPIO_DeInit( DACx_CHANNEL_SIGNAL_FOR_TEST_GPIO_PORT, DACx_CHANNEL_SIGNAL_FOR_TEST_PIN );
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

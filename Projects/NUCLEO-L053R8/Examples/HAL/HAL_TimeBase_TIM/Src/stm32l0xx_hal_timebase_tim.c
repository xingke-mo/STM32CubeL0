/**
  ******************************************************************************
  * @file    stm32l0xx_hal_timebase_tim_template.c
  * @author  MCD Application Team
  * @brief   HAL time base based on the hardware TIM Template.
  *
  *          This file override the native HAL time base functions (defined as weak)
  *          the TIM time base:
  *           + Intializes the TIM peripheral to generate a Period elapsed Event each 1ms
  *           + HAL_IncTick is called inside HAL_TIM_PeriodElapsedCallback ie each 1ms
  *
 @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    This file must be copied to the application folder and modified as follows:
    (#) Rename it to 'stm32l0xx_hal_timebase_tim.c'
    (#) Add this file and the TIM HAL driver files to your project and make sure
       HAL_TIM_MODULE_ENABLED is defined in stm32l0xx_hal_conf.h

    [..]
    (@) The application needs to ensure that the time base is always set to 1 millisecond
       to have correct HAL operation.

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

/** @addtogroup HAL_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef        htim6;
/* Private function prototypes -----------------------------------------------*/
void TIM6_IRQHandler( void );
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the TIM6 as a time base source.
  *         The time source is configured  to have 1ms time base with a dedicated
  *         Tick interrupt priority.
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick( uint32_t TickPriority )
{
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock;
    uint32_t              uwAPB2Prescaler;
    uint32_t              uwPrescalerValue;
    uint32_t              pFLatency;
    HAL_StatusTypeDef     status;

    /* Configure the TIM6 IRQ priority */
    HAL_NVIC_SetPriority( TIM6_IRQn, TickPriority, 0U );

    /* Enable the TIM6 global Interrupt */
    HAL_NVIC_EnableIRQ( TIM6_IRQn );

    /* Enable TIM6 clock */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig( &clkconfig, &pFLatency );

    /* Get APB2 prescaler */
    uwAPB2Prescaler = clkconfig.APB2CLKDivider;

    /* Compute TIM6 clock */
    if( uwAPB2Prescaler == RCC_HCLK_DIV1 )
    {
        uwTimclock = HAL_RCC_GetPCLK2Freq();
    }
    else
    {
        uwTimclock = 2U * HAL_RCC_GetPCLK2Freq();
    }

    /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
    uwPrescalerValue = ( uint32_t )( ( uwTimclock / 1000000U ) - 1U );

    /* Initialize TIM6 */
    htim6.Instance = TIM6;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim6.Init.Period = ( 1000000U / 1000U ) - 1U;
    htim6.Init.Prescaler = uwPrescalerValue;
    htim6.Init.ClockDivision = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    status = HAL_TIM_Base_Init( &htim6 );

    if( status == HAL_OK )
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT( &htim6 );

        if( status == HAL_OK )
        {
            /* Configure the SysTick IRQ priority */
            if( TickPriority < ( 1UL << __NVIC_PRIO_BITS ) )
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority( TIM6_IRQn, TickPriority, 0U );
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick( void )
{
    /* Disable TIM6 update interrupt */
    __HAL_TIM_DISABLE_IT( &htim6, TIM_IT_UPDATE );
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by enabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick( void )
{
    /* Enable TIM6 update interrupt */
    __HAL_TIM_ENABLE_IT( &htim6, TIM_IT_UPDATE );
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

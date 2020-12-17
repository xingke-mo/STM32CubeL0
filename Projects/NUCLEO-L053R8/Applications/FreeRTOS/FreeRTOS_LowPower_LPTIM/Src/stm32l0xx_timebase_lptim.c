/**
  ******************************************************************************
  * @file    stm32l07xx_timebase_lptim.c
  * @author  MCD Application Team
  * @brief   RTOS and HAL time base based on LPTIM.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "cmsis_os.h"
#include "stm32l0xx_timebase_lptim.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Set the Maximum value of the counter (Auto-Reload) that defines the Period */
#define PeriodValue             (uint32_t) (32 - 1)

/* Set the Compare value that defines the duty cycle */
#define PulseValue              (uint32_t) (15 - 1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* LPTIM handle declaration */
LPTIM_HandleTypeDef             LptimHandle;

__IO uint32_t ulTickInit = 0;

/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

#if( configUSE_TICKLESS_IDLE == 2 )
    extern uint32_t ulTickFlag;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  This function configures the LPTIM to generate an interrupt each 1ms.
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
void InitTick( uint32_t TickPriority )
{
    RCC_OscInitTypeDef RCC_OscInitStruct;

    if( ulTickInit == 0 )
    {
        /* Enable LSE clock */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.LSEState = RCC_LSE_ON;
        HAL_RCC_OscConfig( &RCC_OscInitStruct );

        /* ### - 1 - Re-target the LSE to Clock the LPTIM Counter ################# */
        /* Select the LSE clock as LPTIM peripheral clock */
        RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
        RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
        HAL_RCCEx_PeriphCLKConfig( &RCC_PeriphCLKInitStruct );

        /* ### - 2 - Initialize LPTIM peripheral ################################## */
        /*
         *  Instance        = LPTIM1
         *  Clock Source    = APB or LowPowerOSCillator (in this example LSE is
         *                    already selected from the RCC stage)
         *  Clock prescaler = 1 (No division)
         *  Counter source  = Internal event.
         *  Counter Trigger = Software trigger
         *  Output Polarity = High
         *  Update mode     = Immediate (Registers are immediately updated after any
         *                    write access)
         */

        LptimHandle.Instance = LPTIM1;

        LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
        LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
        LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
        LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
        LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
        LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;

        /* Initialize LPTIM peripheral according to the passed parameters */
        if( HAL_LPTIM_Init( &LptimHandle ) != HAL_OK )
        {
            while( 1 );
        }

        HAL_NVIC_SetPriority( LPTIM1_IRQn, TickPriority, 0U );
        HAL_NVIC_EnableIRQ( LPTIM1_IRQn );

        /* ### - 3 - Start generating the PWM signal ############################## */
        /*
         *  Period = 99
         *  Pulse  = 49
         *  According to this configuration, the duty cycle will be equal to 50%
         */
        if( HAL_LPTIM_PWM_Start_IT( &LptimHandle, PeriodValue, PulseValue ) != HAL_OK )
        {
            while( 1 );
        }

        ulTickInit = 1;
    }
}

/**
  * @brief This function configures the source of the time base.
  *        The time source is configured  to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick( uint32_t TickPriority )
{
    InitTick( TickPriority );

    return HAL_OK;
}


void HAL_LPTIM_CompareMatchCallback( LPTIM_HandleTypeDef *hlptim )
{
    if( HAL_LPTIM_ReadAutoReload( hlptim ) != PeriodValue )
    {
        HAL_LPTIM_PWM_Start_IT( &LptimHandle, PeriodValue, PulseValue );
    }

    HAL_IncTick();
    osSystickHandler();

#if( configUSE_TICKLESS_IDLE == 2 )
    ulTickFlag = pdTRUE;
#endif
}

/**
  * @brief  This function handles  WAKE UP TIMER  interrupt request.
  * @param  None
  * @retval None
  */
void LPTIM1_IRQHandler( void )
{
    /* Compare match interrupt */
    if( __HAL_LPTIM_GET_FLAG( &LptimHandle, LPTIM_FLAG_CMPM ) != RESET )
    {
        /* Clear Compare match flag */
        __HAL_LPTIM_CLEAR_FLAG( &LptimHandle, LPTIM_FLAG_CMPM );

        /* Compare match Callback */
        HAL_LPTIM_CompareMatchCallback( &LptimHandle );
    }

    __HAL_LPTIM_CLEAR_FLAG( &LptimHandle, 0x1F );
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

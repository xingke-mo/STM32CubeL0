/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_PWMExternalClock/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use LPTIM to generate a
  *          PWM at the lowest power consumption, using an external counter
  *          clock, through the STM32L0xx HAL API.
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

/** @addtogroup LPTIM_PWMExternalClock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Set the Maximum value of the counter (Auto-Reload) that defines the Period */
#define PeriodValue             (uint32_t) (100 -1)

/* Set the Compare value that defines the duty cycle */
#define PulseValue              (uint32_t) (50 -1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* LPTIM handle declaration */
LPTIM_HandleTypeDef             LptimHandle;

/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config( void );
static void Error_Handler( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
    /* This sample code shows how to use STM32L0xx LPTIM HAL API to generate a
       PWM at the lowest power consumption, using an external counter clock, in
       Low Power mode (STOP mode).
     To proceed, 3 steps are required: */

    /* STM32L0xx HAL library initialization:
         - Configure the Flash prefetch, Flash preread and Buffer caches
         - Systick timer is configured by default as source of time base, but user
               can eventually implement his proper time base source (a general purpose
               timer for example or other time source), keeping in mind that Time base
               duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
               handled in milliseconds basis.
         - Low Level Initialization
       */
    HAL_Init();

    /* Configure the System clock to have a frequency of 2 MHz (Up to 32MHZ possible) */
    SystemClock_Config();


    /* ### - 1 - Initialize LPTIM peripheral ################################## */
    /*
     *  Instance        = LPTIM1
     *  Clock Source    = Ultra Low Power source (external input 1)
     *  Clock prescaler = 1 (No division)
     *  Counter Trigger = Software trigger
     *  Output Polarity = High
     *  Update mode     = Immediate (Registers are immediately updated after any
     *                    write access)
     */

    LptimHandle.Instance = LPTIM1;

    LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_ULPTIM;
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_EXTERNAL;
    LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
    LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
    LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;

    /* Initialize LPTIM peripheral according to the passed parameters */
    if( HAL_LPTIM_Init( &LptimHandle ) != HAL_OK )
    {
        Error_Handler();
    }

    /* ### - 2 - Start generating the PWM signal ############################## */
    /*
     *  Period = 99
     *  Pulse  = 49
     *  According to this configuration, the duty cycle will be equal to 50% and
     *  the output frequency will be equal to the input frequency divided by 100
     *  since the LPTIM have to count 100 external clock edges each period.
     */
    if( HAL_LPTIM_PWM_Start( &LptimHandle, PeriodValue, PulseValue ) != HAL_OK )
    {
        Error_Handler();
    }

    /* ### - 3 - Enter in Stop mode ########################################### */
    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );

    /* Infinite Loop */
    while( 1 )
    {
    }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config( void )
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable MSI Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.MSICalibrationValue = 0x00;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE ;

    if( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_0 ) != HAL_OK )
    {
        /* Initialization Error */
        Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler( void )
{
    /* Infinite loop */
    while( 1 )
    {
    }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed( uint8_t *file, uint32_t line )
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while( 1 )
    {
    }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

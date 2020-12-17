/**
  ******************************************************************************
  * @file    IWDG/IWDG_WindowMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the IWDG HAL API
  *          to update at regular period the IWDG counter and how to simulate a
  *          software fault generating an MCU IWDG reset on expiry of a
  *          programmed time period.
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

/** @addtogroup IWDG_WindowMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* IWDG and TIM handlers declaration */
IWDG_HandleTypeDef IwdgHandle;
RCC_ClkInitTypeDef RCC_ClockFreq;

/* In while loop, time to wait before refresh */
__IO uint32_t WaitingDelay = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config( void );
static void Error_Handler( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main( void )
{

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

    /* Configure the system clock to 2 MHz */
    SystemClock_Config();

    /* Configure LED3 and LED4 */
    BSP_LED_Init( LED3 );
    BSP_LED_Init( LED4 );

    /* Configure User push-button */
    BSP_PB_Init( BUTTON_KEY, BUTTON_MODE_EXTI );

    /*##-1- Check if the system has resumed from IWDG reset ####################*/
    if( __HAL_RCC_GET_FLAG( RCC_FLAG_IWDGRST ) != RESET )
    {
        /* IWDGRST flag set: Turn LED3 on */
        BSP_LED_On( LED3 );

        /* Insert 4s delay */
        HAL_Delay( 4000 );

        /* Prior to clear IWDGRST flag: Turn LED3 off */
        BSP_LED_Off( LED3 );
    }

    /* Clear reset flags anyway */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /*##-2- Configure & Start the IWDG peripheral ##############################*/
    /* Set counter reload value to obtain 762ms IWDG TimeOut.
       Counter Reload Value = (LsiFreq(Hz) * Timeout(ms)) / (prescaler * 1000)
    */
    IwdgHandle.Instance = IWDG;
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_16;
    IwdgHandle.Init.Reload = ( 37000 * 762 ) / ( 16 * 1000 ); /* 762 ms */
    IwdgHandle.Init.Window = ( 37000 * 400 ) / ( 16 * 1000 ); /* 400 ms */

    if( HAL_IWDG_Init( &IwdgHandle ) != HAL_OK )
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Initial delay will be 450 ms in order to be inside the window */
    WaitingDelay = 450;

    /* Infinite loop */
    while( 1 )
    {
        /* Toggle LED3 */
        BSP_LED_Toggle( LED3 );

        HAL_Delay( WaitingDelay );

        /* Refresh IWDG: reload counter */
        if( HAL_IWDG_Refresh( &IwdgHandle ) != HAL_OK )
        {
            /* Refresh Error */
            Error_Handler();
        }
    }
}


/**
  * @brief  EXTI line detection callback to decreaase waiting delay. That will make
            refresh being outside window value.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
    if( GPIO_Pin == KEY_BUTTON_PIN )
    {
        /* waiting 200 ms to be above window value on next refresh */
        WaitingDelay = 200;
    }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler( void )
{
    /* Turn LED4 on */
    BSP_LED_On( LED4 );

    /* Infinite loop */
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
  * @retval None
  */
void SystemClock_Config( void )
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable MSI Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.MSICalibrationValue = 0x00;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
    {
        /* Initialization Error */
        while( 1 );
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
        while( 1 );
    }

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE3 );

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

/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx PWR HAL API to enter
  *          and exit the standby mode with a wakeup pin or external reset.
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

/** @addtogroup PWR_STANDBY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t TimingDelay = LED_TOGGLE_DELAY;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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

    /* Configure LED2 */
    BSP_LED_Init( LED2 );

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Check if the system was resumed from Standby mode */
    if( __HAL_PWR_GET_FLAG( PWR_FLAG_SB ) != RESET )
    {
        /* Clear Standby flag */
        __HAL_PWR_CLEAR_FLAG( PWR_FLAG_SB );


        /* Wait that user release the User push-button */
        BSP_PB_Init( BUTTON_KEY, BUTTON_MODE_GPIO );

        while( BSP_PB_GetState( BUTTON_KEY ) == GPIO_PIN_RESET ) {}
    }

    /* Insert 5 seconds delay */
    HAL_Delay( 5000 );

    /* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
       mainly when using more than one wakeup source this is to not miss any wakeup event.
       - Disable all used wakeup sources,
       - Clear all related wakeup flags,
       - Re-enable all used wakeup sources,
       - Enter the Standby mode.
    */

    /* Disable all used wakeup sources: PWR_WAKEUP_PIN1 */
    HAL_PWR_DisableWakeUpPin( PWR_WAKEUP_PIN1 );

    /* Clear all related wakeup flags*/
    __HAL_PWR_CLEAR_FLAG( PWR_FLAG_WU );

    /* Enable WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00 */
    HAL_PWR_EnableWakeUpPin( PWR_WAKEUP_PIN1 );

    /* Enter the Standby mode */
    HAL_PWR_EnterSTANDBYMode();

    /* This code will never be reached! */
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

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback( void )
{
    HAL_IncTick();

    if( TimingDelay != 0 )
    {
        TimingDelay--;
    }
    else
    {
        /* Toggle LED2 */
        BSP_LED_Toggle( LED2 );
        TimingDelay = LED_TOGGLE_DELAY;
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

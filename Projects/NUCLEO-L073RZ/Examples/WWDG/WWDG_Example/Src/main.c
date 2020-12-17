/**
  ******************************************************************************
  * @file    WWDG/WWDG_Example/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the WWDG HAL API
  *          to update at regular period the WWDG counter and how to generate
  *          a software fault generating an MCU WWDG reset on expiry of a
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

/** @addtogroup WWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* WWDG handler declaration */
static WWDG_HandleTypeDef   WwdgHandle;

/* Private function prototypes -----------------------------------------------*/
static uint32_t TimeoutCalculation( uint32_t timevalue );
void            SystemClock_Config( void );
static void     Error_Handler( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
    uint32_t delay;

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

    /*##-1- Check if the system has resumed from WWDG reset ####################*/
    if( __HAL_RCC_GET_FLAG( RCC_FLAG_WWDGRST ) != RESET )
    {
        /* WWDGRST flag set: Turn LED2 on */
        BSP_LED_On( LED2 );

        /* Insert 4s delay */
        HAL_Delay( 4000 );

        /* Prior to clear WWDGRST flag: Turn LED2 off */
        BSP_LED_Off( LED2 );
    }

    /* Clear reset flags in any case */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /*##-2- Init & Start WWDG peripheral ######################################*/
    /* WWDG clock counter = (PCLK1 (2.1MHz)/4096)/8) = 64.08 Hz (~15.6 ms)
       WWDG Window value = 80 means that the WWDG counter should be refreshed only
       when the counter is below 80 (and greater than 64) otherwise a reset will
       be generated.
       WWDG Counter value = 127, WWDG timeout = 15.6 ms * 64 = 998.4 ms
       In this case the refresh window is comprised between : ~15.6 * (127-80) = 733.2ms and ~15.6 * 64 = 998.4ms
     */
    WwdgHandle.Instance = WWDG;
    WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
    WwdgHandle.Init.Window    = 0x50;
    WwdgHandle.Init.Counter   = 0x7F;
    WwdgHandle.Init.EWIMode   = WWDG_EWI_DISABLE;

    if( HAL_WWDG_Init( &WwdgHandle ) != HAL_OK )
    {
        /* Initialization Error */
        Error_Handler();
    }

    /* Configure User push-button */
    BSP_PB_Init( BUTTON_KEY, BUTTON_MODE_EXTI );

    /* calculate delay to enter window. Add 1ms to secure round number to upper number  */
    delay = TimeoutCalculation( ( WwdgHandle.Init.Counter - WwdgHandle.Init.Window ) + 1 ) + 1;

    /* Infinite loop */
    while( 1 )
    {
        /* Toggle LED2 */
        BSP_LED_Toggle( LED2 );

        /* Insert calculated delay */
        HAL_Delay( delay );

        if( HAL_WWDG_Refresh( &WwdgHandle ) != HAL_OK )
        {
            Error_Handler();
        }
    }
}


/**
  * @brief  Timeout calculation function.
  *         This function calculates any timeout related to
  *         WWDG with given prescaler and system clock.
  * @param  timevalue: period in term of WWDG counter cycle.
  * @retval None
  */
static uint32_t TimeoutCalculation( uint32_t timevalue )
{
    uint32_t timeoutvalue = 0;
    uint32_t pclk1 = 0;
    uint32_t wdgtb = 0;

    /* considering APB divider is still 1, use HCLK value */
    pclk1 = HAL_RCC_GetPCLK1Freq();

    /* get prescaler */
    wdgtb = ( 1 << ( ( WwdgHandle.Init.Prescaler ) >> WWDG_CFR_WDGTB_Pos ) ); /* 2^WDGTB[1:0] */

    /* calculate timeout */
    timeoutvalue = ( ( 4096 * wdgtb * timevalue ) / ( pclk1 / 1000 ) );

    return timeoutvalue;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler( void )
{
    /* Turn LED2 off */
    BSP_LED_Off( LED2 );

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

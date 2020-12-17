/**
  ******************************************************************************
  * @file    Examples_LL/UTILS/UTILS_ConfigureSystemClock/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure system clock using PLL with
  *          HSI as source clock through the STM32L0xx UTILS LL API.
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

/** @addtogroup STM32L0xx_LL_Examples
  * @{
  */

/** @addtogroup UTILS_ConfigureSystemClock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable to store PLL parameters */
/* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
   Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
               ((16MHz * 6)/ 4)                  = 24MHz             */
LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_4}; ;

/* Variable to store AHB and APB buses clock configuration */
/* Settings to have HCLK set to 12MHz, APB1 to 6 MHz and APB2 to 12 MHz */
LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_2, LL_RCC_APB1_DIV_2, LL_RCC_APB2_DIV_1};

/* Private function prototypes -----------------------------------------------*/
void     LED_Init( void );
void     MCO_ConfigGPIO( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
    /* System started with default clock used after reset */

    /* Switch to PLL with HSI as clock source             */
    LL_PLL_ConfigSystemClock_HSI( &sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct );

    /*
       CMSIS variable automatically updated according to new configuration.
       SystemCoreClock should be equal to calculated HCLK frequency.
       FLASH latency is also tuned according to system constraints described
       in the reference manual.
    */

    /* Set Systick to 1ms in using frequency set to SystemCoreClock */
    LL_Init1msTick( SystemCoreClock );

    /* Initialize LED2 */
    LED_Init();

    /* Configure SYSCLK for MCO */
    MCO_ConfigGPIO();

    /* Toggle LED2 in an infinite loop with a period of 1Hz */
    while( 1 )
    {
        LL_GPIO_TogglePin( LED2_GPIO_PORT, LED2_PIN );
        LL_mDelay( 1000 );
    }
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init( void )
{
    /* Enable the LED2 Clock */
    LED2_GPIO_CLK_ENABLE();

    /* Configure IO in output push-pull mode to drive external LED2 */
    LL_GPIO_SetPinMode( LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT );
    /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
    //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
    //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
    /* Reset value is LL_GPIO_PULL_NO */
    //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);

    /* Select MCO clock source and prescaler */
    LL_RCC_ConfigMCO( LL_RCC_MCO1SOURCE_SYSCLK, LL_RCC_MCO1_DIV_1 );
}

/**
  * @brief  Configure MCO pin (PA8).
  * @param  None
  * @retval None
  */
void MCO_ConfigGPIO( void )
{
    /* MCO Clock Enable */
    LL_IOP_GRP1_EnableClock( LL_IOP_GRP1_PERIPH_GPIOA );

    /* Configure the MCO pin in alternate function mode */
    LL_GPIO_SetPinMode( GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE );
    LL_GPIO_SetPinOutputType( GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL );
    LL_GPIO_SetPinSpeed( GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH );
    LL_GPIO_SetPinPull( GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO );
    LL_GPIO_SetAFPin_8_15( GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_0 );
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
       ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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

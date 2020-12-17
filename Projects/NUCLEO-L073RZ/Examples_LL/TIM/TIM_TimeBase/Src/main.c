/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_TimeBase/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance to generate a
  *          time base using the STM32L0xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* Number of time base frequencies */
#define TIM_BASE_FREQ_NB 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Initial autoreload value */
static uint32_t InitialAutoreload = 0;

/* Actual autoreload value multiplication factor */
static uint8_t AutoreloadMult = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config( void );
__STATIC_INLINE void     Configure_TIMTimeBase( void );
__STATIC_INLINE void     LED_Init( void );
__STATIC_INLINE void     UserButton_Init( void );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
    /* Configure the system clock to 2.097 MHz */
    SystemClock_Config();

    /* Initialize LED2 */
    LED_Init();

    /* Initialize button in EXTI mode */
    UserButton_Init();

    /* Configure the timer time base */
    Configure_TIMTimeBase();

    /* Infinite loop */
    while( 1 )
    {
    }
}

/**
  * @brief  Configures the timer as a time base.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  Configure_TIMTimeBase( void )
{
    /* Enable the timer peripheral clock */
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM2 );

    /* Set counter mode */
    /* Reset value is LL_TIM_COUNTERMODE_UP */
    //LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);

    /* Set the pre-scaler value to have TIM2 counter clock equal to 10 kHz      */
    /*
    In this example TIM2 input clock TIM2CLK is set to APB1 clock (PCLK1),
     since APB1 pre-scaler is equal to 1.
        TIM2CLK = PCLK1
        PCLK1 = HCLK
        => TIM2CLK = SystemCoreClock (2 MHz)
    */
    LL_TIM_SetPrescaler( TIM2, __LL_TIM_CALC_PSC( SystemCoreClock, 10000 ) );

    /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    InitialAutoreload = __LL_TIM_CALC_ARR( SystemCoreClock, LL_TIM_GetPrescaler( TIM2 ), 10 );
    LL_TIM_SetAutoReload( TIM2, InitialAutoreload );

    /* Enable the update interrupt */
    LL_TIM_EnableIT_UPDATE( TIM2 );

    /* Configure the NVIC to handle TIM2 update interrupt */
    NVIC_SetPriority( TIM2_IRQn, 0 );
    NVIC_EnableIRQ( TIM2_IRQn );

    /* Enable counter */
    LL_TIM_EnableCounter( TIM2 );

    /* Force update generation */
    LL_TIM_GenerateEvent_UPDATE( TIM2 );
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LED_Init( void )
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
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UserButton_Init( void )
{
    /* Enable the BUTTON Clock */
    USER_BUTTON_GPIO_CLK_ENABLE();

    /* Configure GPIO for BUTTON */
    LL_GPIO_SetPinMode( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT );
    LL_GPIO_SetPinPull( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO );

    /* Connect External Line to the GPIO*/
    USER_BUTTON_SYSCFG_SET_EXTI();

    /* Enable a rising trigger EXTI line 13 Interrupt */
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

    /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
    NVIC_EnableIRQ( USER_BUTTON_EXTI_IRQn );
    NVIC_SetPriority( USER_BUTTON_EXTI_IRQn, 0x03 );
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2097000
  *            HCLK(Hz)                       = 2097000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config( void )
{
    /* MSI configuration and activation */
    LL_RCC_PLL_Disable();
    /* Set new latency */
    LL_FLASH_SetLatency( LL_FLASH_LATENCY_1 );

    LL_RCC_MSI_Enable();

    while( LL_RCC_MSI_IsReady() != 1 )
    {
    };

    LL_RCC_MSI_SetRange( LL_RCC_MSIRANGE_5 );

    LL_RCC_MSI_SetCalibTrimming( 0x0 );

    /* Sysclk activation  */
    LL_RCC_SetAHBPrescaler( LL_RCC_SYSCLK_DIV_1 );

    LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_MSI );

    while( LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI )
    {
    };

    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler( LL_RCC_APB1_DIV_1 );

    LL_RCC_SetAPB2Prescaler( LL_RCC_APB2_DIV_1 );

    /* Set systick to 1ms in using frequency set to 2MHz */
    LL_Init1msTick( 2097000 );

    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock( 2097000 );

    /* Enable Power Control clock */
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    LL_PWR_SetRegulVoltageScaling( LL_PWR_REGU_VOLTAGE_SCALE3 );
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Update the timer update event period
  * @param  None
  * @retval None
  */
void UserButton_Callback( void )
{
    /* Change the update event period by modifying the autoreload value.        */
    /* In up-counting update event is generated at each counter overflow (when  */
    /* the counter reaches the auto-reload value).                              */
    /* Update event period is calculated as follows:                            */
    /*   Update_event = TIM2CLK /((PSC + 1)*(ARR + 1))                          */
    /*   where TIM2CLK is 2.097 MHz                                                */
    AutoreloadMult = AutoreloadMult % TIM_BASE_FREQ_NB;
    LL_TIM_SetAutoReload( TIM2, InitialAutoreload * ( AutoreloadMult + 1 ) );

    /* Force update generation */
    LL_TIM_GenerateEvent_UPDATE( TIM2 );

    AutoreloadMult++;
}

/**
  * @brief  Timer update interrupt processing
  * @param  None
  * @retval None
  */
void TimerUpdate_Callback( void )
{
    LL_GPIO_TogglePin( LED2_GPIO_PORT, LED2_PIN );
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

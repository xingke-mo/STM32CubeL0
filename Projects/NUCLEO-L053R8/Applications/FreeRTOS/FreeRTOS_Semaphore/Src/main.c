/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define semtstSTACK_SIZE      configMINIMAL_STACK_SIZE

/* Private variables ---------------------------------------------------------*/
osThreadId SemThread1Handle, SemThread2Handle;
osSemaphoreId osSemaphore;

/* Private function prototypes -----------------------------------------------*/
static void SemaphoreThread1( void const *argument );
static void SemaphoreThread2( void const *argument );
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

    /* Configure the System clock to 2 MHz */
    SystemClock_Config();

    /* Initialize LED */
    BSP_LED_Init( LED2 );

    /* Define used semaphore */
    osSemaphoreDef( SEM );

    /* Create the semaphore used by the two threads */
    osSemaphore = osSemaphoreCreate( osSemaphore( SEM ), 1 );

    /* Create the first Thread */
    osThreadDef( SEM_Thread1, SemaphoreThread1, osPriorityLow, 0, semtstSTACK_SIZE );
    SemThread1Handle = osThreadCreate( osThread( SEM_Thread1 ), ( void * ) osSemaphore );

    /* Create the second Thread */
    osThreadDef( SEM_Thread2, SemaphoreThread2, osPriorityIdle, 0, semtstSTACK_SIZE );
    SemThread2Handle = osThreadCreate( osThread( SEM_Thread2 ), ( void * ) osSemaphore );

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for( ;; );

}

/**
  * @brief  Semaphore Thread 1 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread1( void const *argument )
{
    uint32_t count = 0;
    osSemaphoreId semaphore = ( osSemaphoreId ) argument;

    for( ;; )
    {

        if( semaphore != NULL )
        {
            /* Try to obtain the semaphore */
            if( osSemaphoreWait( semaphore, 100 ) == osOK )
            {
                count = osKernelSysTick() + 5000;

                /* Toggle LED2 every 200 ms for 5 seconds */
                while( count >= osKernelSysTick() )
                {
                    /* Toggle LED2 */
                    BSP_LED_Toggle( LED2 );

                    /* Delay 200 ms */
                    osDelay( 200 );
                }

                /* Release the semaphore */
                osSemaphoreRelease( semaphore );

                /* Suspend ourseleves to execute thread 2 (lower priority)  */
                osThreadSuspend( NULL );
            }
        }
    }
}

/**
  * @brief  Semaphore Thread 2 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread2( void const *argument )
{
    uint32_t count = 0;
    osSemaphoreId semaphore = ( osSemaphoreId ) argument;

    for( ;; )
    {
        if( semaphore != NULL )
        {
            /* Try to obtain the semaphore */
            if( osSemaphoreWait( semaphore, 0 ) == osOK )
            {
                /* Resume Thread 1 (higher priority)*/
                osThreadResume( SemThread1Handle );

                count = osKernelSysTick() + 5000;

                /* Toggle LED2 every 400 ms for 5 seconds*/
                while( count >= osKernelSysTick() )
                {
                    BSP_LED_Toggle( LED2 );

                    osDelay( 400 );
                }

                /* Release the semaphore to unblock Thread 1 (higher priority)  */
                osSemaphoreRelease( semaphore );
            }
        }
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
  *   where the assert_param error has occurred.
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
    {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

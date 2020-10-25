/** 
  ******************************************************************************
  * @file    main.c
  * @brief   pwm generation for RGB(W) LEDs
  * @author  simon.burkhardt@eta-systems.ch
  * @version 1.0
  * @date    2020-10-21
  * 
  * @note    stm8l050j3
  * @note    COSMIC STM8 C Compiler Special Edition
  *          Version: 4.4.12 Date: 02 Jul 2020
  * @see     http://embedded-lab.com/blog/starting-stm8-microcontrollers/6
  * @see     https://github.com/sihyungwoo/STM8-Development/blob/master/example/PWM/main.c
  * 
  ******************************************************************************
  * Copyright (c) 2020 eta Systems GmbH
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stm8s_conf.h"
#include "ws2812B_fx.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIM4_PERIOD       124

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void CLK_Config(void);
void TIM4_Config(void);
void GPIO_Config(void);
void _delay_ms(u16);
void uptime_routine(void);

/* Private user code ---------------------------------------------------------*/
void main( void )
{  
  CLK_Config();
  TIM4_Config();
  GPIO_Config();

  enableInterrupts();

  /* MCU Configuration -------------------------------------------------------*/

  /* Infinite Loop -----------------------------------------------------------*/
  while(1)
  { 
    rainbowCycle(25); 
  }
}

/**
  * @brief  Configure system clock to run at Maximum clock speed and output the 
  *         system clock on CCO pin
  * @param  None
  * @retval None
  */
void CLK_Config(void)
{
  ErrorStatus status = FALSE;
  CLK_DeInit();
  /* Configure the Fcpu to DIV1*/
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  /* Configure the HSI prescaler to the optimal value */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  /* Output Fcpu on CLK_CCO pin */
  CLK_CCOConfig(CLK_OUTPUT_CPU);
  /* Configure the system clock to use HSI clock source and to run at 16Mhz */
  status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

/* GPIO Init -----------------------------------------------------------------*/
void GPIO_Config(void)
{
  GPIO_Init (GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init (GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // LED Strip
  GPIO_Init (GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST); 
  GPIO_Init (GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST); // LD1 on board
}

/* Timer4 Init ---------------------------------------------------------------*/
void TIM4_Config(void)
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  TIM4_Cmd(ENABLE);
}


static u32 uptime = 0;
static u32 delay_time = 0;
u8 z = 0;

void uptime_routine(void)
{
  uptime++;
  if(uptime == 0xFFFFFFFF){
    delay_time = 0;
    uptime  = 0;
  }
  z++;
  if(z == 250){
    z = 0;
    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
  }
}

void _delay_ms(u16 wait)
{ 
  delay_time = uptime + wait;
  while(delay_time > uptime){}
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************************* (c) 2020 eta Systems GmbH *****END OF FILE****/


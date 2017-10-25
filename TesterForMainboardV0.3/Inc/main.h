/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define S_5V_Pin GPIO_PIN_13
#define S_5V_GPIO_Port GPIOC
#define S_3V3_Pin GPIO_PIN_14
#define S_3V3_GPIO_Port GPIOC
#define S_LAMP_Pin GPIO_PIN_15
#define S_LAMP_GPIO_Port GPIOC
#define S_220V_Pin GPIO_PIN_0
#define S_220V_GPIO_Port GPIOD
#define S_3V4_Pin GPIO_PIN_1
#define S_3V4_GPIO_Port GPIOD
#define AD_VOL_Pin GPIO_PIN_0
#define AD_VOL_GPIO_Port GPIOA
#define AD_CUR_Pin GPIO_PIN_1
#define AD_CUR_GPIO_Port GPIOA
#define AD_18V_Pin GPIO_PIN_4
#define AD_18V_GPIO_Port GPIOA
#define AD_3V3_Pin GPIO_PIN_5
#define AD_3V3_GPIO_Port GPIOA
#define AD_I3V3_Pin GPIO_PIN_6
#define AD_I3V3_GPIO_Port GPIOA
#define AD_ENV_Pin GPIO_PIN_7
#define AD_ENV_GPIO_Port GPIOA
#define AD_SUR_Pin GPIO_PIN_0
#define AD_SUR_GPIO_Port GPIOB
#define AD_TRA_Pin GPIO_PIN_1
#define AD_TRA_GPIO_Port GPIOB
#define PROG_START_Pin GPIO_PIN_12
#define PROG_START_GPIO_Port GPIOB
#define PROG_OK_Pin GPIO_PIN_13
#define PROG_OK_GPIO_Port GPIOB
#define PROG_BUSY_Pin GPIO_PIN_14
#define PROG_BUSY_GPIO_Port GPIOB
#define S_PROG_Pin GPIO_PIN_15
#define S_PROG_GPIO_Port GPIOB
#define LED_PASS_Pin GPIO_PIN_8
#define LED_PASS_GPIO_Port GPIOA
#define LED_FAIL_Pin GPIO_PIN_11
#define LED_FAIL_GPIO_Port GPIOA
#define BEEPER_Pin GPIO_PIN_12
#define BEEPER_GPIO_Port GPIOA
#define IN_FAN_Pin GPIO_PIN_3
#define IN_FAN_GPIO_Port GPIOB
#define INT_IGBTL_Pin GPIO_PIN_4
#define INT_IGBTL_GPIO_Port GPIOB
#define INT_IGBTL_EXTI_IRQn EXTI4_IRQn
#define INT_IGBTH_Pin GPIO_PIN_5
#define INT_IGBTH_GPIO_Port GPIOB
#define INT_IGBTH_EXTI_IRQn EXTI9_5_IRQn
#define IN_H18VL_Pin GPIO_PIN_6
#define IN_H18VL_GPIO_Port GPIOB
#define IN_H18VH_Pin GPIO_PIN_7
#define IN_H18VH_GPIO_Port GPIOB
#define KEY_END_Pin GPIO_PIN_8
#define KEY_END_GPIO_Port GPIOB
#define KEY_START_Pin GPIO_PIN_9
#define KEY_START_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

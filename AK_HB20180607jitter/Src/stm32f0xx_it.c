/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "adc.h"
#include "heater.h"
#include "tim.h"
#include "usart.h"
#define JETTER_RANGE	30
extern uint8_t flag_jitter;
extern uint8_t rx_len;
extern uint8_t txdata[50];
extern uint8_t rxdata[50];
extern uint8_t flag_rx;
extern uint8_t heatstep;
extern uint8_t counter_throttle;
extern uint8_t counter_syntony;
extern uint16_t pwm_pulse;
extern uint16_t pulse_jitter;
extern uint16_t pulse_temp;
extern uint16_t target_power;
/* USER CODE BEGIN 0 */
extern void task_countdown(void);
extern void SetPowerBus(uint8_t status,uint16_t delay);
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;


extern PARAMETER para; 

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  static uint16_t timer;
	if (timer<1000)
	    timer++;
	else
	{
		timer=0;
		para.rClock++;
		if (para.rClock == 0xffff)
			  para.rClock=0;
	}
	
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  task_countdown();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/


/**
* @brief This function handles EXTI line 4 to 15 interrupts.
*/
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint8_t jitter_order=0;
	
	if (GPIO_Pin==Surge_Pin)
	{
    heat_stop_with_on_delay();
		ErrorReport(ERROR_L_SURGE);
	}


	
	if (GPIO_Pin==Throttle_Pin)
	{
		if (flag_jitter==0)
		{
			  pwm_pulse=pwm_pulse-5;//-10, *5 stable paramters		  
				if (pwm_pulse<MiniHeatPwm)
						pwm_pulse=MiniHeatPwm;						
				htim1.Instance->ARR = pwm_pulse*2;//period	
				htim1.Instance->CCR3 = pwm_pulse;//pulse
		}
		else
		{
				pulse_jitter=pulse_jitter-5;
				if (pulse_jitter<MiniHeatPwm)
						pulse_jitter=MiniHeatPwm;						
				htim1.Instance->ARR = pulse_jitter*2;//period	
				htim1.Instance->CCR3 = pulse_jitter;//pulse
		}
	  counter_throttle=10;
		ErrorReport(ERROR_L_CURRENT);
	}	
	

  if (GPIO_Pin==Counter_Pin)
	{
		if (flag_jitter==1)
				{
					if ((pulse_jitter>(pulse_temp-JETTER_RANGE))&&(jitter_order==0))
					{
						  pulse_jitter--;
							if (pulse_jitter<MiniHeatPwm)
									pulse_jitter=MiniHeatPwm;						
							htim1.Instance->ARR = pulse_jitter*2;//period	
	            htim1.Instance->CCR3 = pulse_jitter;//pulse
					}
					else
						jitter_order=1;


					if ((pulse_jitter<(pulse_temp+JETTER_RANGE))&&(jitter_order==1))
					{
						  pulse_jitter++;
						  if (pulse_jitter>(para.mPulse+JETTER_RANGE))
									pulse_jitter=para.mPulse+JETTER_RANGE;				
							htim1.Instance->ARR = pulse_jitter*2;//period	
	            htim1.Instance->CCR3 = pulse_jitter;//pulse				
					}
					else
						jitter_order=0;
				}
    else
				{
					counter_syntony++;
					if (counter_syntony>200)
							counter_syntony=200;
				}
  }

}
/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint8_t jitter_order=0;
	
	if (GPIO_Pin==Surge_Pin)
	{
    heat_stop_with_on_delay();
		ErrorReport(ERROR_L_SURGE);
	}


	
	if (GPIO_Pin==Throttle_Pin)
	{
		ErrorReport(ERROR_L_CURRENT);
		if (flag_jitter==0)
		{
			 pwm_pulse=pwm_pulse-5;//-10, *5 stable paramters		  
		   pwm_pulse_set(pwm_pulse);
		}
		else
		{
				pulse_jitter=pulse_jitter-5;
				pwm_pulse_set(pulse_jitter);
		}
	  counter_throttle=10;
	}	
	

	
  if (GPIO_Pin==Counter_Pin)
	{
		if (flag_jitter==1)
				{
					if ((pulse_jitter>(pulse_temp-20))&&(jitter_order==0))
					{//pwm_pulse_set(pulse_jitter--);
						  pulse_jitter--;
							htim1.Instance->ARR = pulse_jitter*2;//period	
	            htim1.Instance->CCR3 = pulse_jitter;//pulse
					}
					else
						jitter_order=1;
					
					if ((pulse_jitter<(pulse_temp+20))&&(jitter_order==1))
					{//pwm_pulse_set(pulse_jitter++);
						  pulse_jitter++;
							htim1.Instance->ARR = pulse_jitter*2;//period	
	            htim1.Instance->CCR3 = pulse_jitter;//pulse						
					}
					else
						jitter_order=0;
				}
    else
				{
					counter_syntony++;
					if (counter_syntony>200)
							counter_syntony=200;
				}
  }
}*/
/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel 2 and 3 interrupts.
*/
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}



/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)==1)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
  rx_len=50-hdma_usart1_rx.Instance->CNDTR; //get received data size 
	flag_rx=1;		
}
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

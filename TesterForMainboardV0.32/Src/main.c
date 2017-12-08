/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "..\MyCode\global_variable.h"
/* USER CODE BEGIN Includes */
extern uint8_t LoadTestReference;
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
extern void task_scheduler(void) ;
/* USER CODE END PFP */
extern PeripheralState LedBeeper;		
extern TestState TestStatus;
extern uint32_t AdcTank[32];
extern uint8_t  calibrate_table[28];

/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
      HAL_UART_Transmit(&huart3, (uint8_t *)&ch,1, 0xFFFF); 
      return ch;
}

int fgetc(FILE *f)
{
      uint8_t  ch;
      HAL_UART_Receive(&huart3,(uint8_t *)&ch, 1, 0xFFFF);
      return  ch;
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1,AdcTank,32);
  //MX_IWDG_Init();
  MX_USART1_UART_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  MX_USART2_UART_Init();
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  MX_USART3_UART_Init();


  /* USER CODE BEGIN 2 */
	LedBeeper.beeper=0x13;
	LoadTestReference=1;
	TestStatus.test_step=0;
	TestStatus.error_code =0;
	TestStatus.test_instruction =0;
	TestStatus.test_percent =0;
	
	//HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_SET);
	 uint16_t i,j;
	 for (i=0;i<2000;i++)
	 {
		 for (j=0;j<255;j++)
		 {}
	 }
  /* USER CODE END 2 */
	printf ("\n\r Mainboard diagnose system started! V0.31 20171206");
	/*how to use printf
	1/print string, printf("abcdefg")
	2/print variable ,printf("%d",test) or ("%x",test); //%d output 10formart,%x output 16formart
	3/print string amd variable, printf ("ad_voltage=%d",test);	
	*/
//HAL_GPIO_WritePin(GPIOC, S_5V_Pin|S_3V3_Pin|S_LAMP_Pin, GPIO_PIN_SET);
   //HAL_GPIO_WritePin(GPIOD, S_220V_Pin|S_3V4_Pin, GPIO_PIN_SET);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	calibrate_table[0]=0x41;
	calibrate_table[1]=0x4b;
	calibrate_table[2]=28;
	calibrate_table[3]=0;
	calibrate_table[4]=CLB_VOLTAGE_LO;
	calibrate_table[5]=0;
	calibrate_table[6]=CLB_VOLTAGE_HI;
	calibrate_table[7]=0;	
	calibrate_table[8]=CLB_CURRENT1_LO;
	calibrate_table[9]=0;	
	calibrate_table[10]=CLB_CURRENT1_HI;
	calibrate_table[11]=0;	
	calibrate_table[12]=CLB_CURRENT2_LO;
	calibrate_table[13]=0;
	calibrate_table[14]=CLB_CURRENT2_HI;
	calibrate_table[15]=0;
	calibrate_table[16]=CLB_CURRENT3_LO;
	calibrate_table[17]=0;
	calibrate_table[18]=CLB_CURRENT3_HI;
	calibrate_table[19]=0;	
	calibrate_table[20]=CLB_CURRENT4_LO;
	calibrate_table[21]=0;
	calibrate_table[22]=CLB_CURRENT4_HI;
	calibrate_table[23]=0;	
	calibrate_table[24]=CLB_CURRENT5_LO;
	calibrate_table[25]=0;
	calibrate_table[26]=CLB_CURRENT5_HI;
	calibrate_table[27]=0;

  while (1)
  {
  /* USER CODE END WHILE */
    task_scheduler();
  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

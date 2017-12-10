/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"
#include "flash.h"
#include "gpio.h"
#include "dma.h"

#include "heater.h"

void task2_UartSendReceive(void);
void task4_ErrorHandle(void);
void UartSend(void);
void UartReceive(void);

extern void set_fan_speed(uint8_t percent);
extern void SetPowerBus(uint8_t status);

extern uint8_t rx_len;
extern uint8_t txdata[50];
extern uint8_t rxdata[50];
extern uint8_t flag_rx;
extern uint16_t task_delay[];

uint8_t flag_write_current_fact_calibration=0;
uint8_t flag_write_current_base_calibration=0;


/*test content*/
extern uint16_t frequency;
extern uint16_t ad_voltage;
extern uint16_t ad_current;
extern uint16_t ad_sink;
extern uint16_t ad_pan;
extern uint16_t ad_coil;
extern uint16_t ad_pan3;
extern uint16_t pwm_pulse;
/*test content*/

PARAMETER para; 

static uint8_t com_buffer[MAX_CommBuffer];


/* USER CODE BEGIN 0 */
void task2_UartSendReceive(void)
{			
	task_delay[2]=40;
	static uint8_t SendReceiveChance=0;
	if (SendReceiveChance==0)
		SendReceiveChance=1;
	else
		SendReceiveChance=0;
	if (SendReceiveChance==0)
		  UartSend();
	else
		  UartReceive();

}

void UartReceive(void)
{
		
	uint8_t i,checksum;
	static uint8_t counter_com=100;
	if (counter_com>0)
	    counter_com--;
	if(flag_rx==1)
			{			
				flag_rx=0;							
				checksum=0;
				uint8_t flag_key_right=1;
				  for (i=4;i<rxdata[2];i=i+2)
				        {
				          if (rxdata[i]>MAX_RxKey)
				              flag_key_right=0;
				        }
				
				for (i=0;i<rxdata[2];i++)
				{
					checksum=checksum+rxdata[i];					
				}

				if ((rxdata[0]=='A')&&(rxdata[1]=='K')&&(checksum==0)&&(flag_key_right==1))
						{						
			        InstructionDecode();
							InstructionExecute();
							counter_com=50;
						}	
			  HAL_UART_DMA_RX_Stop(&huart1);	  				
				HAL_UART_Receive_DMA(&huart1, rxdata, 50);	
									
			}
	if (counter_com<25)
	{
		HAL_UART_Init(&huart1);
	}
	if (counter_com==0)
	{
		heat_stop_with_on_delay();
		ErrorReport(ERROR_L_LINK);
	}	
	
}

void UartSend(void)
{	
	PostToTxdata();
  txdata[0]='A';
	txdata[1]='K';
	txdata[2]=para.mRsplength*2+4;
	txdata[3]=0;
	uint8_t i,j;
	j='A'+'K'+txdata[2];

	for (i=0;i<para.mRsplength;i++)
	    {
				txdata[i*2+4]=i+0x62;
				j=j+txdata[i*2+4];
				txdata[i*2+5]=com_buffer[i+0x62];
				j=j+txdata[i*2+5];
			}
			
	txdata[3]=0x100-j;			
  			
	HAL_UART_Transmit_DMA(&huart1, txdata,txdata[2]);			
}

void task4_ErrorHandle(void)
{
	task_delay[4]=3000;
	ErrorReset();
}

void Parameters_Init(void)
{
	
	para.mCurrent=1000;//max current                 10.00A
	para.mPower=3000;//min continuously heat power  300.0W
	para.mPulse=1160;//1265=19k,1230=19.5k,1200=20k,max pwm pulse
	para.mRsplength=16;//max returning data length    2
	para.mTemperature=1300;//max heatsink temperatue  1100=92,1250=87
	para.mVoltage=2600;//max voltage                260.00V
	pwm_pulse=0;//
	para.bVolfact=1250;
	para.bVolbase=10000;	
	uint8_t i;
	for (i=0;i<MAX_CommBuffer;i++)
	{com_buffer[i]=0;}
}	


void InstructionDecode(void)
{
	uint8_t i;
	uint16_t j;
  
	for(i=4;i<rxdata[2];i=i+2)
       com_buffer[rxdata[i]]=rxdata[i+1];
	
	for(i=0;i<50;i++)
	{rxdata[i]=0;}
  
	/*decode calibration instruction start*/
	static uint16_t old_current_fact=0;
	static uint16_t new_current_fact=0;
	static uint16_t old_current_base=0;
	static uint16_t new_current_base=0;

	new_current_fact=com_buffer[CLB_CURFACT_HI];
	new_current_fact=(new_current_fact<<8)+com_buffer[CLB_CURFACT_LO];
	new_current_base=com_buffer[CLB_CURBASE_HI];
	new_current_base=(new_current_base<<8)+com_buffer[CLB_CURBASE_LO];

	if ((new_current_fact!=old_current_fact)&&(new_current_fact>0))
	{
    old_current_fact=new_current_fact;
		para.bCurfact=old_current_fact;
		flag_write_current_fact_calibration=1;
	}
	else
		flag_write_current_fact_calibration=0;

	if ((new_current_base!=old_current_base)&&(new_current_base>0))
	{
    old_current_base=new_current_base;
		para.bCurbase=old_current_base;
		flag_write_current_base_calibration=1;
	}
	else
		flag_write_current_base_calibration=0;
	/*decode calibration instruction end*/
	
	
	j=com_buffer[CMD_POWER_HI];
	j=j<<8;
	para.cPower=j+com_buffer[CMD_POWER_LO];
	
	j=com_buffer[CMD_PERIPHERAL_HI];
	j=j<<8;
	para.cPeripheral=j+com_buffer[CMD_PERIPHERAL_LO];
	
	if(com_buffer[CMD_LOADTEST]>0)
	   para.cLoadtest=com_buffer[CMD_LOADTEST];

	
	j=com_buffer[LMT_RSPLENGTH];
	if (j>0)
	  para.mRsplength=j;
	else
		ErrorReport(ERROR_H_INIT);
	
	j=com_buffer[LMT_CURRENT_HI];
	j=j<<8;
	j=j+com_buffer[LMT_CURRENT_LO];
	if (j>0)
	   para.mCurrent=j;
	else
		 ErrorReport(ERROR_H_INIT);

	j=com_buffer[LMT_POWER_HI];
	j=j<<8;
	j=j+com_buffer[LMT_POWER_LO];
	if (j>0)
    	para.mPower=j;
	else
	    ErrorReport(ERROR_H_INIT);

	j=com_buffer[LMT_PULSE_HI];
	j=j<<8;
	j=j+com_buffer[LMT_PULSE_LO];
	if (j>0)
	    para.mPulse=j;
	else
		  ErrorReport(ERROR_H_INIT);
	
	j=com_buffer[LMT_TEMPERATURE_HI];
	j=j<<8;
	j=j+com_buffer[LMT_TEMPERATURE_LO];
	if (j>0)
	   para.mTemperature=j;
	else
		  ErrorReport(ERROR_H_INIT);
	
	j=com_buffer[LMT_VOLTAGE_HI];
	j=j<<8;
	j=j+com_buffer[LMT_VOLTAGE_LO];	
	if (j>0)
	   para.mVoltage=j;
	else
		  ErrorReport(ERROR_H_INIT);	
		
	if (para.mRsplength<2)
		  para.mRsplength=2;
}


void InstructionExecute(void)
{
	/*write calibration data start*/
	if (flag_write_current_fact_calibration)
	{
		flag_write_current_fact_calibration=0;
		WriteFlashCurrentFactCalibration();
	}
	if (flag_write_current_base_calibration)
	{
		flag_write_current_base_calibration=0;
		WriteFlashCurrentBaseCalibration();
	}		
	/*write calibration data end*/
	
	uint8_t temp_fan_speed;
	temp_fan_speed=para.cPeripheral&0x0003;
	if (temp_fan_speed)
	    {
				if (temp_fan_speed==1)
					  set_fan_speed(45);
				if (temp_fan_speed==2)
					  set_fan_speed(67);
				if (temp_fan_speed==3)
					  set_fan_speed(100);				
			}
  else
      set_fan_speed(0);
	
	
	if (para.cPeripheral&0x0040)
		  SetPowerBus(1);
  else
      SetPowerBus(0);
}

void PostToTxdata(void)
{
	com_buffer[RSP_ERROR_HI]=para.rError>>8;
	com_buffer[RSP_ERROR_LO]=para.rError;
	
	com_buffer[RSP_VOLTAGE_HI]=para.rVoltage>>8;
	com_buffer[RSP_VOLTAGE_LO]=para.rVoltage;
	
	com_buffer[RSP_CURRENT_HI]=para.rCurrent>>8;//ad_current>>8;//
	com_buffer[RSP_CURRENT_LO]=para.rCurrent;//ad_current;//
	
	com_buffer[RSP_PULSE_HI]=para.rPulse>>8;
	com_buffer[RSP_PULSE_LO]=para.rPulse;	
	

	com_buffer[RSP_TEMPSINK_HI]=para.rTempsink>>8;
	com_buffer[RSP_TEMPSINK_LO]=para.rTempsink;	
	
	
	com_buffer[RSP_TEMPERATURE_HI]=para.rTemperature>>8;
	com_buffer[RSP_TEMPERATURE_LO]=para.rTemperature;
	


	com_buffer[RSP_LOADTEST]=para.rLoadtest;
	
	com_buffer[RSP_TOPOLOGY]=para.rTopology;

	com_buffer[RSP_VERSION_HI]=para.rVersion>>8;
	com_buffer[RSP_VERSION_LO]=para.rVersion;
	
	com_buffer[RSP_CLOCK_HI]=para.rClock>>8;
	com_buffer[RSP_CLOCK_LO]=para.rClock;	
	
}

void ErrorReport(uint16_t errorcode)
{
	para.rError = para.rError|errorcode;
}

void ErrorReset(void)
{
	para.rError = 0;
}
	



HAL_StatusTypeDef HAL_UART_DMA_RX_Stop(UART_HandleTypeDef *huart)
{
  /* The Lock is not implemented on this API to allow the user application
     to call the HAL UART API under callbacks HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback() /
     HAL_UART_TxHalfCpltCallback() / HAL_UART_RxHalfCpltCallback (): 
     indeed, when HAL_DMA_Abort() API is called, the DMA TX/RX Transfer or Half Transfer complete interrupt is 
     generated if the DMA transfer interruption occurs at the middle or at the end of the stream
     and the corresponding call back is executed. 
     */
  
  /* Disable the UART Rx DMA requests */
  huart->Instance->CR3 &= ~USART_CR3_DMAR;

  /* Abort the UART DMA rx channel */
  if(huart->hdmarx != NULL)
  {
    HAL_DMA_Abort(huart->hdmarx);
  }

  huart->State = HAL_UART_STATE_READY;

  return HAL_OK;
}
	


/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT
                              |UART_ADVFEATURE_AUTOBAUDRATE_INIT;
  huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_ENABLE;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  huart1.AdvancedInit.AutoBaudRateEnable = UART_ADVFEATURE_AUTOBAUDRATE_ENABLE;
  huart1.AdvancedInit.AutoBaudRateMode = UART_ADVFEATURE_AUTOBAUDRATE_ONSTARTBIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA2     ------> USART1_TX
    PA3     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral DMA init*/
  
    hdma_usart1_rx.Instance = DMA1_Channel3;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    hdma_usart1_tx.Instance = DMA1_Channel2;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA2     ------> USART1_TX
    PA3     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

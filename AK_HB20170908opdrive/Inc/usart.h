/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
HAL_StatusTypeDef HAL_UART_DMA_RX_Stop(UART_HandleTypeDef *huart);
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
void InstructionDecode(void);
void InstructionExecute(void);
void Parameters_Init(void);
void ErrorReport(uint16_t errorcode);
void ErrorReset(void);
void PostToTxdata(void);
/* USER CODE END Private defines */

extern void Error_Handler(void);

void MX_USART1_UART_Init(void);

#define __HAL_UART_RX_ENABLE(__HANDLE__)               ((__HANDLE__)->Instance->CR1 |=  USART_CR1_RE)
#define __HAL_UART_RX_DISABLE(__HANDLE__)              ((__HANDLE__)->Instance->CR1 &=  ~USART_CR1_RE)
#define __HAL_UART_TX_ENABLE(__HANDLE__)               ((__HANDLE__)->Instance->CR1 |=  USART_CR1_TE)
#define __HAL_UART_TX_DISABLE(__HANDLE__)              ((__HANDLE__)->Instance->CR1 &=  ~USART_CR1_TE)

#define     MAX_CommBuffer  0x7f
#define     MAX_RxKey       0x4d
/* USER CODE BEGIN Prototypes */
#define CMD_POWER_LO		    0x22
#define CMD_POWER_HI		    0x23
#define CMD_PERIPHERAL_LO	  0x24
#define CMD_PERIPHERAL_HI	  0x25
#define CMD_LOADTEST		    0x26
#define CMD_ID				      0x27//
#define CMD_TIMER_LO		    0x28//
#define CMD_TIMER_HI		    0x29//
#define CMD_TEMPERATURE_LO	0x2a//
#define CMD_TEMPERATURE_HI	0x2b//
#define CMD_INDICATOR		    0x2c//
#define CMD_MODE			      0x2d//
	
#define CLB_CURBASE_LO		  0x32
#define CLB_CURBASE_HI		  0x33
#define CLB_CURFACT_LO		  0x34
#define CLB_CURFACT_HI		  0x35
#define CLB_VOLBASE_LO		  0x36
#define CLB_VOLBASE_HI		  0x37
#define CLB_VOLFACT_LO		  0x38
#define CLB_VOLFACT_HI	   	0x39
#define RESERVED_A			    0x41
#define LMT_RSPLENGTH		    0x42
#define LMT_CURRENT_LO		  0x44
#define LMT_CURRENT_HI	    0x45
#define LMT_POWER_LO		    0x46
#define LMT_POWER_HI		    0x47
#define LMT_PULSE_LO		    0x48
#define LMT_PULSE_HI		    0x49
#define LMT_TEMPERATURE_LO	0x4a
#define LMT_TEMPERATURE_HI	0x4b
#define LMT_VOLTAGE_LO		  0x4c
#define LMT_VOLTAGE_HI		  0x4d

#define LOB_DEFAULTS 		    0x52//
#define LOB_LOG				      0x53//
#define LOB_FIRMWARE		    0x54//
#define LOB_GEARP			      0x55//
#define LOB_GEART			      0x56//
	
#define RSP_ERROR_LO		    0x62
#define RSP_ERROR_HI		    0x63
#define RSP_VOLTAGE_LO		  0x64
#define RSP_VOLTAGE_HI		  0x65
#define RSP_CURRENT_LO		  0x66
#define RSP_CURRENT_HI		  0x67
#define RSP_TEMPERATURE_LO	0x68
#define RSP_TEMPERATURE_HI	0x69
#define RSP_LOADTEST	     	0x6a

#define RSP_TEMPSINK_LO		  0x6c
#define RSP_TEMPSINK_HI		  0x6d
#define RSP_CLOCK_LO		    0x6e
#define RSP_CLOCK_HI		    0x6f
#define RSP_PULSE_LO		    0x70
#define RSP_PULSE_HI		    0x71
#define RSP_TEMPCOIL_LO		  0x72//
#define RSP_TEMPCOIL_HI		  0x73//
#define RSP_TOPOLOGY		    0x74//
#define RSP_VERSION_LO		  0x76
#define RSP_VERSION_HI		  0x77
#define RSP_TEMPENV_LO		  0x78//
#define RSP_TEMPENV_HI		  0x79//
#define RSP_BUTTON			    0x7e//
#define RSP_KNOB			      0x7f//


#define ERROR_L_CURRENT		  0x0001  //ERROR_SYNTONYCURRENT
#define ERROR_L_VOLTAGE		  0x0002  //ERROR_SYNTONYVOLTAGE
#define ERROR_L_SURGE		    0x0004	//ERROR_SURGE
#define ERROR_L_LINK		    0x0008	//ERROR_COMFAILURE
#define ERROR_L_TEMPERATURE	0x0010	//ERROR_SINKOVERHOT
#define ERROR_L_LOAD		    0x0020	//ERROR_NOPAN
#define ERROR_L_POWER		    0x0040  //ERROR_LOWPOWER
#define ERROR_L_HALT		    0x0080  //ERROR_STOPRUNNING

#define ERROR_H_CURRENT		  0x0100	//ERROR_CURRENT
#define ERROR_H_VOLTAGE		  0x0200	//ERROR_VOLTAGE

#define ERROR_H_RADIO		    0x0800

#define ERROR_H_INIT		    0x1000	//ERROR_INITFAILURE

#define ERROR_H_REBOOT		  0x4000
	
#define MODE_POWER			  0//
#define MODE_TEMPERATURE	1//
#define MODE_SLAVE			  2//
#define MODE_HEATER			  3//

#define PERIPHERAL_L_FAN0	  0x00
#define PERIPHERAL_L_FAN1 	0x01
#define PERIPHERAL_L_FAN2 	0x02
#define PERIPHERAL_L_FAN3 	0x03
#define PERIPHERAL_L_BEEP  	0x04
#define PERIPHERAL_L_LIGHT	0x08
#define PERIPHERAL_L_RELAY	0x40
typedef struct{
	uint16_t cPower;		// 0x22
	uint16_t cPeripheral;	// 0x24
	uint8_t  cLoadtest;	// 0x26

	uint16_t bCurbase;		// 0x32
	uint16_t bCurfact;		// 0x34
	uint16_t bVolbase;		// 0x36
	uint16_t bVolfact;		// 0x38

	uint16_t mRsplength;	// 0x42
	uint16_t mCurrent;		// 0x44
	uint16_t mPower;		// 0x46
	uint16_t mPulse;		// 0x48
	uint16_t mTemperature;// 0x4a
	uint16_t mVoltage;		// 0x4c

	uint16_t rPower;    //0x60
	uint16_t rError;		// 0x62
	uint16_t rVoltage;		// 0x64
	uint16_t rCurrent;		// 0x66
	uint16_t rTemperature;	// 0x68
	uint8_t  rLoadtest;	// 0x6a
	uint16_t rTempsink;	// 0x6c
	uint16_t rClock;		// 0x6e
	uint16_t rPulse;		// 0x70
	uint16_t rTempcoil;	// 0x72
	uint16_t rTopology;	// 0x74
	
	uint16_t rVersion;		// 0x76
}PARAMETER;

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
#define     MAX_CommuBuffer  0x7f
#define     MAX_RxKey       0x4d

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

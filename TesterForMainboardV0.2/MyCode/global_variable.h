

#ifndef __global_variable_H
#define __global_variable_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
typedef struct{
								uint16_t  ad_voltage;			  // 0x22//target power
								uint16_t  ad_current;		// 0x24//fan and led control
								uint16_t  ad_18v;		// 0x2a//
								uint16_t  ad_3v3;		// 0x2c//
								uint16_t  ad_i3v3;		// 0x2e//
								uint16_t  ad_envronment;		// 0x30//
								uint16_t  ad_surge;	// 0x32//
								uint16_t  ad_transformer;	// 0x34//
								uint16_t  ad_realtime_voltage;			  // 0x22//target power
								uint16_t  ad_realtime_current;		// 0x24//fan and led control	
								uint16_t  voltage;	// 0x36//
								uint16_t  current;	// 0x38//
								uint16_t  power;		// 0x3a//
              }AD_parameter;

typedef struct{
								uint8_t  led_pass;			  // 0x22//target power
								uint8_t  led_fail;		// 0x24//fan and led control
								uint8_t  beeper;		// 0x2a//
              }PeripheralState;

typedef struct{
								uint8_t  igbtl;			  // 0x22//target power
								uint8_t  igbth;		// 0x24//fan and led control
	              uint8_t  uart1_rx;
	              uint8_t  uart2_rx;
              }flag;	

typedef struct{
								uint8_t  error_code;			  // 0x22//target power
								uint8_t  test_step;		// 0x24//fan and led control
	              uint8_t  test_instruction;
	              uint8_t  test_percent;
              }TestState;		

typedef struct{
								uint16_t  mask;			  // 0x22//target power
								uint16_t  value;		// 0x24//fan and led control
              }BoardContent;							
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */




#ifdef __cplusplus
}
#endif
#endif


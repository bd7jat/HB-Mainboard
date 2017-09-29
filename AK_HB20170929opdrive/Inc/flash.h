#ifndef __flash_H
#define __flash_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void WriteFlashCurrentFactCalibration (void);
void WriteFlashCurrentBaseCalibration (void);
uint16_t ReadFlashCurrentFactCalibration (void);
uint16_t ReadFlashCurrentBaseCalibration (void);


/* USER CODE END Prototypes */
#define PageCalibration             0x08003c00
#define AddressCurrentFactCalibration   0x08003c00
#define AddressCurrentBaseCalibration   0x08003c02
#define AddressVoltageFactCalibration   0x08003c04
#define AddressVoltageBaseCalibration   0x08003c06
#ifdef __cplusplus
}
#endif
#endif



#ifndef __heater_H
#define __heater_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void power_handle(void);
void heat_stop_with_on_delay(void);
void heat_protect(void);
void heat_process(void);
void heat_stop_with_off_delay(void);
/* USER CODE END Prototypes */

#define MiniHeatPwm   450//400=60k,480=50K
#define StartHeatPwm  490//400=60k,480=50K,600=40k
#define nopan_delay  2000
#define PowerDistributePeriod 2000
#ifdef __cplusplus
}
#endif
#endif


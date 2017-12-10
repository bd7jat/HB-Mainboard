#include "stdint.h"
#include "flash.h"
#include "usart.h"
#include "stm32f0xx_hal.h"

extern PARAMETER para; 
extern uint16_t calibration_table[6];
FLASH_EraseInitTypeDef Calibration ;

extern void heat_stop_with_no_delay(void);

void WriteFlashCalibration (void)
{   
	  heat_stop_with_no_delay();
		HAL_FLASH_Unlock();
		Calibration.TypeErase = FLASH_TYPEERASE_PAGES;//page earse only
		Calibration.PageAddress = PageCalibration;//the address of page is about to be erased
		Calibration.NbPages = 1;
		uint32_t PageEraseError = 0;
		HAL_FLASHEx_Erase(&Calibration,&PageEraseError);
	  uint8_t i;
	  uint16_t j;
	  for (i=0;i<6;i++)
	  {
			j=calibration_table[i]<<4;
			j=j+i;
			HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressVoltage+i*2,j);
		}	
		HAL_FLASH_Lock();
		
}


uint16_t ReadFlashCalibration (uint32_t address)
{
  uint16_t temp = *(uint16_t*)(address);
	return(temp);
}




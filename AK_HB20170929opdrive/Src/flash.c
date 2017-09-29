#include "stdint.h"
#include "flash.h"
#include "usart.h"
#include "stm32f0xx_hal.h"

extern PARAMETER para; 

FLASH_EraseInitTypeDef Calibration ;

extern void heat_stop_with_no_delay(void);


void WriteFlashCurrentFactCalibration (void)
{   
	  heat_stop_with_no_delay();
	  uint16_t TempCurrentCalibrationFactData=para.bCurfact;//2.55A
		uint16_t TempCurrentCalibrationBaseData=ReadFlashCurrentBaseCalibration();
		HAL_FLASH_Unlock();
		Calibration.TypeErase = FLASH_TYPEERASE_PAGES;//page earse only
		Calibration.PageAddress = PageCalibration;//the address of page is about to be erased
		Calibration.NbPages = 1;
		uint32_t PageEraseError = 0;
		HAL_FLASHEx_Erase(&Calibration,&PageEraseError);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentFactCalibration, TempCurrentCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentBaseCalibration, TempCurrentCalibrationBaseData);		
		HAL_FLASH_Lock();
}

void WriteFlashCurrentBaseCalibration (void)
{
	  heat_stop_with_no_delay();	
		uint16_t TempCurrentCalibrationFactData=ReadFlashCurrentFactCalibration();
	  uint16_t TempCurrentCalibrationBaseData=para.bCurbase;//2.55A
		HAL_FLASH_Unlock();
		Calibration.TypeErase = FLASH_TYPEERASE_PAGES;//page earse only
		Calibration.PageAddress = PageCalibration;//the address of page is about to be erased
		Calibration.NbPages = 1;
		uint32_t PageEraseError = 0;
		HAL_FLASHEx_Erase(&Calibration,&PageEraseError);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentFactCalibration, TempCurrentCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentBaseCalibration, TempCurrentCalibrationBaseData);		
		HAL_FLASH_Lock();
}

uint16_t ReadFlashCurrentFactCalibration (void)
{
  uint16_t temp = *(uint16_t*)(AddressCurrentFactCalibration);
	return(temp);
}

uint16_t ReadFlashCurrentBaseCalibration (void)
{
  uint16_t temp = *(uint16_t*)(AddressCurrentBaseCalibration);
	return(temp);
}

/*void WriteFlashVoltageFactCalibration (void)
{
	  heat_stop_with_no_delay();	
	  uint16_t TempCurrentCalibrationBaseData=ReadFlashCurrentBaseCalibration();
	  uint16_t TempCurrentCalibrationFactData=ReadFlashCurrentFactCalibration();
		uint16_t TempVoltageCalibrationFactData=para.bVolfact;
		uint16_t TempVoltageCalibrationBaseData=ReadFlashVoltageBaseCalibration();
		HAL_FLASH_Unlock();
		Calibration.TypeErase = FLASH_TYPEERASE_PAGES;//page earse only
		Calibration.PageAddress = PageCalibration;//the address of page is about to be erased
		Calibration.NbPages = 1;
		uint32_t PageEraseError = 0;
		HAL_FLASHEx_Erase(&Calibration,&PageEraseError);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentFactCalibration, TempCurrentCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentBaseCalibration, TempCurrentCalibrationBaseData);		
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressVoltageFactCalibration, TempVoltageCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressVoltageBaseCalibration, TempVoltageCalibrationBaseData);
		HAL_FLASH_Lock();
}

void WriteFlashVoltageBaseCalibration (void)
{
	  heat_stop_with_no_delay();	
	  uint16_t TempCurrentCalibrationBaseData=ReadFlashCurrentBaseCalibration();
	  uint16_t TempCurrentCalibrationFactData=ReadFlashCurrentFactCalibration();
		uint16_t TempVoltageCalibrationFactData=ReadFlashVoltageFactCalibration();
		uint16_t TempVoltageCalibrationBaseData=para.bVolbase;
		HAL_FLASH_Unlock();
		Calibration.TypeErase = FLASH_TYPEERASE_PAGES;//page earse only
		Calibration.PageAddress = PageCalibration;//the address of page is about to be erased
		Calibration.NbPages = 1;
		uint32_t PageEraseError = 0;
		HAL_FLASHEx_Erase(&Calibration,&PageEraseError);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentFactCalibration, TempCurrentCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressCurrentBaseCalibration, TempCurrentCalibrationBaseData);		
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressVoltageFactCalibration, TempVoltageCalibrationFactData);
		HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, AddressVoltageBaseCalibration, TempVoltageCalibrationBaseData);
		HAL_FLASH_Lock();
}

uint16_t ReadFlashVoltageFactCalibration (void)
{
	uint16_t temp = (*(uint16_t*)(AddressVoltageFactCalibration));
	return(temp);
}

uint16_t ReadFlashVoltageBaseCalibration (void)
{
	uint16_t temp = (*(uint16_t*)(AddressVoltageBaseCalibration));
	return(temp);
}*/

#include "stdint.h"
#include "gpio.h"
#include "..\MyCode\global_variable.h"

extern uint16_t task_delay[];
extern PeripheralState LedBeeper;		

void task5_PeripheralControl(void);


void beeper_control(void);
void led_pass_control(void);
void led_fail_control(void);


void task5_PeripheralControl(void)
{
	task_delay[5]=97;
  beeper_control();
  led_pass_control();
  led_fail_control();
	
}
void beeper_control(void)
{
	static uint8_t beeper_data,beeper_counter,beeper_delay,beeper_state=0;
	if (LedBeeper.beeper)
			{
				beeper_data=LedBeeper.beeper;
				LedBeeper.beeper=0;
				beeper_counter=beeper_data>>4;
				beeper_delay=beeper_data&0x0f;
				beeper_state=0;
			}
	if (beeper_counter>0)
			{
				if (beeper_delay>0)
				{
					beeper_delay--;
					if (beeper_state==0)
					   HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_RESET);
					else
						 HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_SET);
				}
				else
				{
					beeper_counter--;
					if (beeper_state==0)
						  beeper_state=1;
					else
						  beeper_state=0;
					beeper_delay=beeper_data&0x0f;
				}
			}
	else
	   HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_SET);
	
}

void led_pass_control(void)
{
	  static uint8_t led_pass_state,led_pass_temp;
		if (LedBeeper.led_pass)
		{
			 led_pass_state=LedBeeper.led_pass;
			 LedBeeper.led_pass=0;
			 led_pass_state=led_pass_state&0x0f;
			 led_pass_temp=led_pass_state;
		}
			
		if (led_pass_state==0x0f)
			HAL_GPIO_WritePin(LED_PASS_GPIO_Port,LED_PASS_Pin,GPIO_PIN_RESET);
		else if (led_pass_state==0)
			HAL_GPIO_WritePin(LED_PASS_GPIO_Port,LED_PASS_Pin,GPIO_PIN_SET);
		else 
		{
			if (led_pass_temp>0)
				  led_pass_temp--;
			if (led_pass_temp==0)
					{
						led_pass_temp=led_pass_state;
						HAL_GPIO_TogglePin(LED_PASS_GPIO_Port,LED_PASS_Pin);
					}
		}
		
}

void led_fail_control(void)
{
		static uint8_t led_fail_state,led_fail_temp;
		if (LedBeeper.led_fail)
		{
			 led_fail_state=LedBeeper.led_fail;
			 LedBeeper.led_fail=0;
			 led_fail_state=led_fail_state&0x0f;
			 led_fail_temp=led_fail_state;
		}
			
		if (led_fail_state==0x0f)
			HAL_GPIO_WritePin(LED_FAIL_GPIO_Port,LED_FAIL_Pin,GPIO_PIN_RESET);
		else if (led_fail_state==0)
			HAL_GPIO_WritePin(LED_FAIL_GPIO_Port,LED_FAIL_Pin,GPIO_PIN_SET);
		else 
		{
			if (led_fail_temp>0)
				  led_fail_temp--;
			if (led_fail_temp==0)
					{
						led_fail_temp=led_fail_state;
						HAL_GPIO_TogglePin(LED_FAIL_GPIO_Port,LED_FAIL_Pin);
					}
		}
}


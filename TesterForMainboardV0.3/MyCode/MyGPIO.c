#include "stdint.h"
#include "gpio.h"
#include "..\MyCode\global_variable.h"

extern uint16_t task_delay[];
extern PeripheralState LedBeeper;		

void task5_PeripheralControl(void);


void beeper_control(void);
void led_pass_control(void);
void led_fail_control(void);
void key_scan(void);

void task5_PeripheralControl(void)
{
	task_delay[5]=47;
  beeper_control();
  led_pass_control();
  led_fail_control();
	key_scan();
	
}
void key_scan(void)
{
	static uint8_t counter_start=0;
	static uint8_t counter_end=0;
	static uint8_t step_start=0;
	static uint8_t step_end=0;
	switch(step_start)
	{
		case 0:
			if (HAL_GPIO_ReadPin(KEY_START_GPIO_Port,KEY_START_Pin)==0)
				 step_start=1;
		break;
		case 1:
			if (HAL_GPIO_ReadPin(KEY_START_GPIO_Port,KEY_START_Pin)==0)
          counter_start++;
			else
			{
				counter_start=0;
				step_start=0;
			}
			if (counter_start>2)
			{
				counter_start=0;
				step_start=2;
			}		
		break;
		case 2:
			if (HAL_GPIO_ReadPin(KEY_START_GPIO_Port,KEY_START_Pin))
			{
				LedBeeper.key_start=1;
				step_start=0;
			}
		break;		
		default:
		break;
	}
	
	switch(step_end)
	{
		case 0:
			if (HAL_GPIO_ReadPin(KEY_END_GPIO_Port,KEY_END_Pin)==0)
				 step_end=1;
		break;
		case 1:
			if (HAL_GPIO_ReadPin(KEY_END_GPIO_Port,KEY_END_Pin)==0)
          counter_end++;
			else
			{
				counter_end=0;
				step_end=0;
			}
			if (counter_end>2)
			{
				counter_end=0;
				step_end=2;
			}		
		break;
		case 2:
			if (HAL_GPIO_ReadPin(KEY_END_GPIO_Port,KEY_END_Pin))
			{
				LedBeeper.key_end=1;
				step_end=0;
			}
		break;		
		default:
		break;
	}
	

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
					   HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_SET);
					else
						 HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_RESET);
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
	   HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_RESET);
	
}

void led_pass_control(void)
{
	  static uint8_t led_pass_state=0;
	  static uint8_t led_pass_temp=0;
		if ((LedBeeper.led_pass)&&(LedBeeper.led_pass!=led_pass_state))
		{
			 led_pass_state=LedBeeper.led_pass;
			 led_pass_state=led_pass_state&0x0f;
			 led_pass_temp=led_pass_state;
		}
		if (LedBeeper.led_pass==0)
			led_pass_state=0;
			
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
		static uint8_t led_fail_state=0;
	  static uint8_t led_fail_temp=0;
		if ((LedBeeper.led_fail)&&(LedBeeper.led_fail!=led_fail_state))
		{
			 led_fail_state=LedBeeper.led_fail;
			 led_fail_state=led_fail_state&0x0f;
			 led_fail_temp=led_fail_state;
		}
		if (LedBeeper.led_fail==0)
			led_fail_state=0;
			
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


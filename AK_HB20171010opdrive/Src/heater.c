#include "heater.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

extern uint16_t task_delay[];
extern uint16_t pwm_pulse;
extern uint16_t target_power;

extern uint8_t counter_throttle;
extern uint8_t heatstep;
extern uint8_t flag_closepower;
extern uint8_t counter_syntony;

extern PARAMETER para; 

extern uint16_t poweroff_delay;

static uint8_t counter_addpower_delay=0;

void heat_stop_with_no_delay(void);

void task1_PowerControl(void)
{

	task_delay[1] = 1;
	heat_protect();	
	power_handle();
  heat_process();
	if (poweroff_delay>0)
		  poweroff_delay--;
}

void power_handle(void)
{
  static uint16_t  new_target_power,old_target_power,poweron_time,poweroff_time,counter_poweron,counter_poweroff;

		target_power = para.cPower;
		new_target_power = para.cPower;							
		if ((new_target_power > 0)&&(new_target_power < para.mPower))
			{
				if (old_target_power != new_target_power)
				{
					old_target_power = new_target_power;
					poweron_time=old_target_power*PowerDistributePeriod/para.mPower;
					if (poweron_time<200)
							poweron_time=200;
					if (poweron_time>(PowerDistributePeriod-200))
							poweron_time=PowerDistributePeriod-200;					
					poweroff_time=PowerDistributePeriod-poweron_time;
					counter_poweron=poweron_time;
					counter_poweroff=poweroff_time;
				}

				if (counter_poweron>0)//timer for targer power lower than 1000w
						counter_poweron--;	
				if ((counter_poweroff>0)&&(counter_poweron==0))
						counter_poweroff--;	
				if (counter_poweroff==0)
						{  
							counter_poweroff=poweroff_time;
							counter_poweron=poweron_time;
						}
				
				if (counter_poweron>0)
					target_power=para.mPower;
				else
					target_power=0;
				
			}		
			
    static uint8_t flag_poweroffcommand=0;		
		if ((target_power==0)&&(flag_poweroffcommand==0))
		{
			heatstep = 5;
			flag_poweroffcommand=1;
		}
			
		if((heatstep==0)&&(target_power>(para.mPower-1)))
		{
			heatstep = 1;	
			flag_poweroffcommand=0;
		}
}


void load_test(void)
{
	  if ((poweroff_delay==0)&&((para.rError&0x023d)==0))//(poweroff_delay==0)//
				{
					set_counter_pin_interrupt(1);		
					pwm_start();					
					uint8_t i;
					for (i=0;i<255;i++)
					{}					
					pwm_stop();					
          heatstep++;		
          counter_syntony=0;				
				}
}



void load_check(void)
{
	  set_counter_pin_interrupt(0);
	  para.rLoadtest=counter_syntony;	
	   if ((para.rLoadtest<para.cLoadtest)&&(para.rLoadtest>1)) 
		   {
				  heatstep++;
	        pwm_start();
				  counter_addpower_delay=60;
			 }
		else
		{
      heat_stop_with_on_delay();
			ErrorReport(ERROR_L_LOAD);
		}
}

void heat_start(void)
{	
	if (counter_addpower_delay>0)//waitting for voltage drop 
       counter_addpower_delay--;
	if (para.rPower<para.mPower)
			    {
						pwm_pulse_set(pwm_pulse++);
						if (pwm_pulse>950)
								{
									heat_stop_with_on_delay();
									ErrorReport(ERROR_L_LOAD);
								}
						
					}
	else
	{
		if (counter_addpower_delay==0)
		      heatstep++;
	}
	
}

void heat_adjust(void)
{
	static uint8_t counter_add=0;
	
	if (counter_add>3)
		 counter_add=0;
	else
		 counter_add++;
	if ((para.rPower<target_power)&&(counter_add==0)&&(counter_throttle==0)&&(para.rCurrent<(para.mCurrent-40)))
			    {
						if ((target_power-para.rPower)>300)
							pwm_pulse_set(pwm_pulse++);
								
			    }
	if	(para.rPower>target_power)	 
			{ 	
				 if ((para.rPower-target_power>300))
             {
							 pwm_pulse=pwm_pulse-5;
							 pwm_pulse_set(pwm_pulse);
						 }				 
					
			}

	if (counter_throttle>0)
		 counter_throttle--;

	
		para.rPulse=pwm_pulse;			
}

void heat_stop_with_off_delay(void)//for case of normally power off such as power distribution and power off command
{
	if (flag_closepower)
	{
			pwm_stop();
			heatstep=0;	
			pwm_pulse=0;
		  ErrorReport(ERROR_L_HALT);
	}
}

void heat_stop_with_on_delay(void)//for case of protection
{
	pwm_stop();
	heatstep=0;	
	pwm_pulse=0;
  poweroff_delay=nopan_delay;
	ErrorReport(ERROR_L_HALT);
	
}

void heat_stop_with_no_delay(void)
{
	pwm_stop();
	heatstep=0;	
	pwm_pulse=0;
  poweroff_delay=0;
	ErrorReport(ERROR_L_HALT);
}

void heat_protect(void)
{
	if ((para.rVoltage<(70*para.mVoltage/100))||(para.rVoltage>para.mVoltage))	
	{
			heat_stop_with_on_delay();
			ErrorReport(ERROR_H_VOLTAGE);
	}
	
	if (para.rCurrent>(para.mCurrent-20))//max current limitation
	{ 
		pwm_pulse_set(pwm_pulse--);
		ErrorReport(ERROR_H_CURRENT);
	}
	
	if (para.rCurrent>(para.mCurrent-40))//max current limitation
		ErrorReport(ERROR_H_CURRENT);

				
	/*if (heatstep==4)//protect of bad pan , decrease target power
	{
		static int16_t counter_bad_pan=0;
	  if (para.rPower<(para.cPower-1000))//report low power
		{ 
			counter_bad_pan++;
			if (counter_bad_pan>30000)
				  counter_bad_pan=30000;
   }
	 else
		  counter_bad_pan=0;
	 if (counter_bad_pan==30000)
	 {
		 ErrorReport(ERROR_L_POWER);
		 para.cPower=para.cPower-2000;
		 
	 }
	 
  }*/
	if (heatstep==4)
	 {
	  if (para.rPower<(para.cPower-1000))
        ErrorReport(ERROR_L_POWER);//report low power
   }
	

	/*static uint8_t counter_movepandelay=0;
	if (heatstep==4)//check pan status
	 {
			if (para.rPower<(para.mPower-200))
					{
						counter_movepandelay++;
						if (counter_movepandelay>100)
						{
							counter_movepandelay=0;
						  heat_stop_with_on_delay();
						  ErrorReport(ERROR_L_LOAD);
						}
					}
      else
         	counter_movepandelay=0;			
			if (para.rPower==0)
			{
							counter_movepandelay=0;
						  heat_stop_with_on_delay();
						  ErrorReport(ERROR_L_LOAD);
				
			}
		}*/

	static uint16_t counter_movepandelay=0;
	static uint16_t counter_overcurrent=0;
	static uint16_t counter_zeropower=0;
	if (heatstep==4)//check pan status
	 { 
			if (para.rPower<(para.mPower+4000))
					 {
								counter_movepandelay++;
						    if (para.rError&0x0001)
									  counter_overcurrent++;
								if ((counter_movepandelay>200)&&(counter_overcurrent>100))
									{
										counter_zeropower=0;
										counter_overcurrent=0;
										counter_movepandelay=0;
										heat_stop_with_on_delay();
										ErrorReport(ERROR_L_LOAD);
									}
						}
      else
			{
								counter_zeropower=0;
								counter_overcurrent=0;
								counter_movepandelay=0;
			}			
			if (para.rPower<(para.mPower-800))
			{
				      counter_zeropower++;
				      if (counter_zeropower>200)
							{
								counter_zeropower=0;
								counter_overcurrent=0;
								counter_movepandelay=0;
								heat_stop_with_on_delay();
								ErrorReport(ERROR_L_LOAD);
							}
				
			}
			
		}

		
    static uint8_t flag_sink_overhot=0;	//heatsink overhot protect
		if (para.rTempsink<para.mTemperature)
			{
					flag_sink_overhot=1;		
					heat_stop_with_on_delay();
					ErrorReport(ERROR_L_TEMPERATURE);
			}			
		if (flag_sink_overhot==1)
			{
				if (para.rTempsink>(para.mTemperature+200))
						flag_sink_overhot=0;
				else
				{
					ErrorReport(ERROR_L_TEMPERATURE); 
					heat_stop_with_on_delay();
				}
			}		
		
}




void heat_process(void)
{	
				 switch (heatstep)
							{
									case 1://
									{
										load_test();
										break;
									}
									case 2://
									{
										load_check();
										break;
									}
									case 3://
									{  
									 heat_start();				
										break;
									}
									case 4://
									{
										heat_adjust();
										break;
									}
									case 5://
									{ 
										heat_stop_with_off_delay();
										break;
									}		
									default://
									{
										heatstep = 0;	
										break;
									}
					}
}


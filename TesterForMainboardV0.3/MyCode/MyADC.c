#include "stdint.h"
#include "usart.h"
#include "..\MyCode\global_variable.h"


extern uint16_t task_delay[];
extern uint32_t AdcTank[32];


extern AD_parameter  AdcQueue;

void task0_AdSample(void);



void task0_AdSample(void)
{
	task_delay[0]=1;
	/*start ad filter*/
	uint8_t i;
	static uint8_t pointer_adc=0;
	static uint16_t filter_current[50],filter_voltage[50],filter_surge[50];
	static uint32_t ADCfilter[8];	
	uint16_t realtime_surge;
	/*for(i=0;i<32;i++)
	{AdcTank[i]=1;}*/	
  for(i=2;i<8;i++)
	     {ADCfilter[i]=ADCfilter[i]+AdcTank[i]+AdcTank[i+8]+AdcTank[i+16]+AdcTank[i+24];}
	AdcQueue.ad_realtime_voltage=(AdcTank[0]+AdcTank[8]+AdcTank[16]+AdcTank[24])>>2;			 
	AdcQueue.ad_realtime_current=(AdcTank[1]+AdcTank[9]+AdcTank[17]+AdcTank[25])>>2;
	realtime_surge=(AdcTank[6]+AdcTank[14]+AdcTank[22]+AdcTank[30])>>2;
	
  if (pointer_adc<49)
		pointer_adc++;
	else
	  pointer_adc=0;
	
	filter_voltage[pointer_adc]=AdcQueue.ad_realtime_voltage;
	filter_current[pointer_adc]=AdcQueue.ad_realtime_current;
	filter_surge[pointer_adc]=realtime_surge;
	uint32_t temp_current=0,temp_voltage=0,temp_surge;
	for(i=0;i<50;i++)
		 {
			 temp_current=temp_current+filter_current[i];
			 temp_voltage=temp_voltage+filter_voltage[i];
			 temp_surge=temp_surge+filter_surge[i];
		 }		
 	AdcQueue.ad_current=temp_current/50;
	AdcQueue.ad_voltage=temp_voltage/50;	
	AdcQueue.ad_surge=temp_surge/50;
		 
	static uint8_t counter_adc;
	if (counter_adc==50)
	{
		counter_adc=0;
		AdcQueue.ad_18v=ADCfilter[2]/200;
		AdcQueue.ad_3v3=ADCfilter[3]/200;
		AdcQueue.ad_i3v3=ADCfilter[4]/200;
		AdcQueue.ad_envronment=ADCfilter[5]/200;
		AdcQueue.ad_transformer=ADCfilter[7]/200;
		for(i=0;i<8;i++)
		{ADCfilter[i]=0;}
	}
	counter_adc++;	
  /*end ad filter and save the result to AdcQueue*/
	AdcQueue.voltage=AdcQueue.ad_voltage*1537/1000+5;//unit is 0.1V
	AdcQueue.current = (AdcQueue.ad_current*37+3923)/100;//uint is 0.01A
	AdcQueue.power = AdcQueue.voltage*AdcQueue.current/100;//unit is 0.1W	
}


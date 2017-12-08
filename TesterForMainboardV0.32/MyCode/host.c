#include "stdint.h"
#include "gpio.h"
#include "usart.h"
#include "..\MyCode\global_variable.h"

extern uint8_t LoadTestReference;
extern uint8_t FlagSendMainBoard;
extern uint16_t task_delay[];
static  uint8_t test_order=0;
extern uint8_t BoardReveivedData[50];
extern uint8_t  CommuData[MAX_CommuBuffer];
extern AD_parameter  AdcQueue;
extern flag Signal;
extern PeripheralState LedBeeper;	
extern TestState TestStatus;
extern BoardContent BoardControl;
extern uint8_t  calibrate_table[28];

void task1_TestHandle(void);

void ResetTest (void);
void TestShortCurrent(void);
void TestProgram(void);
void TestH18V(void);
void Test18V(void);
void Test3V3(void);
void TestI3V3(void);
void TestSurge(void);
void TestTransformer(void);
void TestCommunation(void);
void TsetFan(void);
void TestIgbtTemperature(void);
void TestOverCurrentProtect(void);
void TestStaticVlotageAD(void);
void TestStaticCurrentAD(void);
void TestPanChecker(void);
void TestIgbthDriver(void);
void TestIgbtlDriver(void);	
void TestCalibrate(void);
void TestPowerCheck(void);
void TestComplete(void);

const uint8_t PageProcess[]="page process";
const uint8_t PageMain[]="page main";


void task1_TestHandle(void)
{
	task_delay[1]=5;
  static uint8_t changescreen[20];
	uint8_t k;
	if ((TestStatus.test_step==0)&&(LedBeeper.key_start==1))
	{
		LedBeeper.beeper=0x11;
		for (k=0;k<12;k++)
		    {
					changescreen[k]=PageProcess[k];
				}
		changescreen[12]=0xff;	
    changescreen[13]=0xff;
    changescreen[14]=0xff;		
    HAL_UART_Transmit_DMA(&huart1,changescreen,15);					
		TestStatus.test_instruction=4;
	}
  LedBeeper.key_start=0;
		
	if (LedBeeper.key_end==1)
	{
		LedBeeper.beeper=0x11;
		for (k=0;k<9;k++)
		    {
					changescreen[k]=PageMain[k];
				}
		changescreen[9]=0xff;	
    changescreen[10]=0xff;
    changescreen[11]=0xff;		
    HAL_UART_Transmit_DMA(&huart1,changescreen,12);						
		TestStatus.test_instruction=5;
	}
	LedBeeper.key_end=0;
	
	if ((TestStatus.error_code>0)&&(TestStatus.error_code<20))
		LedBeeper.led_fail=0x0f;
	
	
	if (TestStatus.test_instruction)
	{
		TestStatus.error_code=0;
		TestStatus.test_percent =0;
		TestStatus.test_step=0;
		if(TestStatus.test_instruction==1)
		{
			TestStatus.test_step=2;
			test_order=1;
		}
		if(TestStatus.test_instruction==2)
		{
			TestStatus.test_step=1;
			test_order=2;
		}		
		if(TestStatus.test_instruction==3)
		{
			TestStatus.test_step=1;
			test_order=3;
		}
		if(TestStatus.test_instruction==4)
		{
			TestStatus.test_step=1;
			test_order=4;
		}		
		if(TestStatus.test_instruction==5)
		{
			TestStatus.test_step=0;
			test_order=5;
		}			
		TestStatus.test_instruction=0;		
		LedBeeper.led_fail=0;
		LedBeeper.led_pass=0;
	}
  switch (TestStatus.test_step )
				{
					case 0://
						ResetTest();
						break;
					case 1://
						TestShortCurrent();//jump options 2,3,18
						break;
					case 2://
						TestProgram();//jump options 3,20
						break;
					case 3://
						TestH18V();
						break;	
					case 4://
						Test18V();
						break;						
					case 5://
						Test3V3();
						break;
					case 6://
						TestI3V3();
						break;
					case 7://
						TestSurge();
						break;
					case 8://
						TestTransformer();
						break;
					case 9://
						TestCommunation();
						break;
					case 10://
						TsetFan();
						break;
					case 11://
						TestIgbtTemperature();
						break;
					case 12://
						TestOverCurrentProtect();
						break;
					case 13://
						TestStaticVlotageAD();
						break;
					case 14://
						TestStaticCurrentAD();
						break;
					case 15://
						TestPanChecker();
						break;
					case 16://
						TestIgbthDriver();
						break;
					case 17://
						TestIgbtlDriver();//jump options 18,20
						break;
					case 18://
						TestCalibrate();
						break;
					case 19://
						TestPowerCheck();
						break;
					case 20://
						TestComplete();
						break;						
					default://
						break;					
				}
	
}
void ResetTest (void)
{
	 HAL_GPIO_WritePin(GPIOC, S_5V_Pin|S_3V3_Pin|S_LAMP_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD, S_220V_Pin|S_3V4_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(PROG_START_GPIO_Port, PROG_START_Pin, GPIO_PIN_SET);
   HAL_GPIO_WritePin(S_PROG_GPIO_Port, S_PROG_Pin, GPIO_PIN_RESET);
	 TestStatus.test_step=0;
	 
//HAL_GPIO_WritePin(GPIOC, S_LAMP_Pin, GPIO_PIN_SET);
//HAL_GPIO_WritePin(GPIOD, S_220V_Pin, GPIO_PIN_SET);	
}

void TestShortCurrent(void)//TestStep1,jump options 2,3,18
{
	static uint8_t ShortCurrentCounter=0;
	HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);
	ShortCurrentCounter++;
	if (ShortCurrentCounter>30)
	{
		ShortCurrentCounter=0;
		uint8_t limit_current;
		limit_current=45*AdcQueue.voltage/2200;
		if (AdcQueue.current<limit_current)//detect overload of mainboard
		   {
					if (test_order==1)
						 TestStatus.test_step=2;
					if (test_order==2)
						 TestStatus.test_step=3; 
					if (test_order==3)
						 TestStatus.test_step=18; 	
					if (test_order==4)
						 TestStatus.test_step=2; 
					TestStatus.error_code=0;
					TestStatus.test_percent=5;	
					//HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);		
          //HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_RESET);					
			 }
	  else
		{
		     TestStatus.error_code=1;
			   LedBeeper.beeper=0x1f;
				 TestStatus.test_step=0;	
	       HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_RESET);
	       HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);			   
		}
	printf ("\n\r Current= %d",AdcQueue.current);
		
	}
	
	
	
	
}
	
void TestProgram(void)//TestStep2,jump options 3,20
{
 static uint16_t ProgramCounter=0;	
 static uint8_t ProgramStep=0;
   switch (ProgramStep)
				{
					case 0://
						HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_RESET);//power off 220V
					  HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(S_PROG_GPIO_Port, S_PROG_Pin, GPIO_PIN_SET);//enable analog switchers of programming
					  HAL_GPIO_WritePin(S_3V4_GPIO_Port, S_3V4_Pin, GPIO_PIN_SET);	
					  ProgramCounter++;
					  if (ProgramCounter>20)
						{
		          ProgramStep=1;
							ProgramCounter=0;
						}
						break;
					case 1://
						ProgramCounter++;
						HAL_GPIO_WritePin(PROG_START_GPIO_Port, PROG_START_Pin, GPIO_PIN_RESET);//produce start signal and last for at least 50ms
					   if (ProgramCounter>10)
							 {
								 HAL_GPIO_WritePin(PROG_START_GPIO_Port, PROG_START_Pin, GPIO_PIN_SET);//disable start signal
								 ProgramStep=2;
								 ProgramCounter=0;
							 }
						break;
					case 2://
						ProgramCounter++;
					  if(ProgramCounter>10)
							{								
								if (HAL_GPIO_ReadPin(PROG_BUSY_GPIO_Port,PROG_BUSY_Pin))//busy pin high level represents process finished
								{
									ProgramStep=3;
									ProgramCounter=0;
								}
							}
						if (ProgramCounter>800)//out of waitting time 4s
						{
							  ProgramCounter=0;
							  ProgramStep=0;
								TestStatus.test_step=0; 
							  TestStatus.error_code=2;
							  printf ("\n\r program fail!");	
							  LedBeeper.beeper=0x1f;
						}
						break;
					case 3://
					  if(HAL_GPIO_ReadPin(PROG_OK_GPIO_Port,PROG_OK_Pin)==0)//if get signal of sucess
							{
								if (test_order==1)		
									{
									 TestStatus.test_step=20; 
									 TestStatus.test_percent=100;							
					         printf ("\n\r program past!");										
									}	
               else 	
								 {								 
									TestStatus.test_step=3; 
									TestStatus.test_percent=10;
									printf ("\n\r program past!");			
								 }
							}
						else
						{
								TestStatus.test_step=0; 
							  TestStatus.error_code=2;
							  printf ("\n\r program fail!");	
                LedBeeper.beeper=0x1f;							
						}
						ProgramStep=0;
						HAL_GPIO_WritePin(S_PROG_GPIO_Port, S_PROG_Pin, GPIO_PIN_RESET);//disable analog switchers of programming	
						HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_RESET);	
            HAL_GPIO_WritePin(S_3V4_GPIO_Port, S_3V4_Pin, GPIO_PIN_RESET);						
						break;						
					default://
						break;					
				}	
				
 

}
	
void TestH18V(void)//TestStep3
{
	//IN_H18VL_Pin=1 stands for H18V is lower than 17V,IN_H18VH_Pin=1 stands for H18V is higher than 20V
 static uint8_t H18VCounter=0;	
 static uint8_t H18VStep=0;
   switch (H18VStep)
				{
					case 0://	
						    HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);	
						    HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_SET);	
                	
						    H18VCounter++;
					      if (H18VCounter>100)
								{
									HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_SET);
									H18VCounter=0;
									H18VStep=1;
								}
						break;
					case 1:
							if(1)//if ((HAL_GPIO_ReadPin(IN_H18VH_GPIO_Port,IN_H18VH_Pin)==1)&&(HAL_GPIO_ReadPin(IN_H18VL_GPIO_Port,IN_H18VL_Pin)==0))
							{
								  H18VStep=0;
									TestStatus.test_step=4; 
									TestStatus.test_percent=15;		
									printf ("\n\r H18V past!");
							}
							else
							{
								  H18VStep=0;
									TestStatus.test_step=0; 
									TestStatus.error_code=3;	
									printf ("\n\r H18V fail!");	
                  LedBeeper.beeper=0x1f;								
							}						
						break;
					default://
						break;							
				}


}
	
void Test18V(void)//TestStep4
{
	//limit ad= voltage*1.5/11.5*4096/3.3 can be simplified as ad=voltage*162
	if ((AdcQueue.ad_18v>2754)&&(AdcQueue.ad_18v<3074))
	{
			TestStatus.test_step=5; 
			TestStatus.test_percent=20;		
	}
	else
	{
		  TestStatus.test_step=0; 
			TestStatus.error_code=4;	
      LedBeeper.beeper=0x1f;			
	}
	printf ("\n\r 18v_ad= %d",AdcQueue.ad_18v);
}
	
void Test3V3(void)//TestStep5
{
	//limit ad= voltage*0.5*4096/3.3 can be simplified as ad=voltage*621
	if ((AdcQueue.ad_3v3>1950)&&(AdcQueue.ad_3v3<2111))
	{
			TestStatus.test_step=6; 
			TestStatus.test_percent=25;		
	}
	else
	{
		  TestStatus.test_step=0; 
			TestStatus.error_code=5;	
      LedBeeper.beeper=0x1f;			
	}	
		printf ("\n\r 3.3v_ad= %d",AdcQueue.ad_3v3);
}
	
void TestI3V3(void)//TestStep6
{
	//limit ad= voltage*0.5*4096/3.3 can be simplified as ad=voltage*621
	if ((AdcQueue.ad_i3v3>1950)&&(AdcQueue.ad_i3v3<2111))
	{
			TestStatus.test_step=7; 
			TestStatus.test_percent=30;		
	}
	else
	{
		  TestStatus.test_step=0; 
			TestStatus.error_code=6;	
      LedBeeper.beeper=0x1f;			
	}	
	printf ("\n\r I3.3v_ad= %d",AdcQueue.ad_i3v3);	
}
	
void TestSurge(void)//TestStep7
{
	if ((AdcQueue.ad_surge>1900)&&(AdcQueue.ad_surge<2200))//if ((AdcQueue.ad_surge>1862)&&(AdcQueue.ad_surge<2482))
	{
			TestStatus.test_step=8; 
			TestStatus.test_percent=35;		
	}
	else
	{
		  TestStatus.test_step=0; 
			TestStatus.error_code=7;	
      LedBeeper.beeper=0x1f;			
	}	
	printf ("\n\r Surge_ad= %d",AdcQueue.ad_surge);		
}
	
void TestTransformer(void)//TestStep8
{
	static uint8_t TransformerCounter=0;
	HAL_GPIO_WritePin(GPIOC, S_3V3_Pin, GPIO_PIN_SET);//add 3.3V to current transformer
	TransformerCounter++;
	if (TransformerCounter>30)
	{
		TransformerCounter=0;
		if ((AdcQueue.ad_transformer>650)&&(AdcQueue.ad_transformer<945))//resistance of transfoemer approximate between 200R and 300R
			{
					TestStatus.test_step=9; 
					TestStatus.test_percent=40;		
			}
			else
			{
					TestStatus.test_step=0; 
					TestStatus.error_code=8;	
          LedBeeper.beeper=0x1f;					
			}
	   printf ("\n\r Transformer_ad= %d",AdcQueue.ad_transformer);						
     HAL_GPIO_WritePin(GPIOC, S_3V3_Pin, GPIO_PIN_RESET);//release 3.3V from current transformer			
	}
	
}
	
void TestCommunation(void)//TestStep9
{
	static uint16_t CommunationCounter=700;
	static uint8_t SucessCounter=0;
	uint8_t i;
	BoardControl.mask=0x2425;
	BoardControl.value=0;

	if (CommunationCounter>0)
		CommunationCounter--;
	if ((BoardReveivedData[0]==0x41)&&(BoardReveivedData[1]==0x4b))
	{
	   if ((BoardReveivedData[5]&0x08)==0)//make sure there is not communation error report
         SucessCounter++;
		 for (i=0;i<50;i++)
		 {
			 BoardReveivedData[i]=0;
		 }
	}
	if (CommunationCounter==0)
	{
			if ((SucessCounter>10))
				 {
						TestStatus.test_step=10; 
						TestStatus.test_percent=45;		
					}
			else
					{
						TestStatus.test_step=0; 
						TestStatus.error_code=9;	
            LedBeeper.beeper=0x1f;							
					}
	printf ("\n\r Comm_SucessCounter= %d",SucessCounter);							
	CommunationCounter=700;
	SucessCounter=0;
					
	}
	
}
	
void TsetFan(void)//TestStep10
{
	static uint16_t FanCounter=400;
	BoardControl.mask=0x2425;
	BoardControl.value=0xffff;
	if (FanCounter>0)
	   FanCounter--;
	if (FanCounter==0)
	{
		if (HAL_GPIO_ReadPin(IN_FAN_GPIO_Port,IN_FAN_Pin)==0)
		  {
				TestStatus.test_step=11; 
				TestStatus.test_percent=50;		
				printf ("\n\r fan test past!");
			}
	  else
			{
				TestStatus.test_step=0; 
				TestStatus.error_code=10;	
        printf ("\n\r fan test fail!");	
        LedBeeper.beeper=0x1f;					
			}	
	  BoardControl.mask=0x2425;
	  BoardControl.value=0;
	  FanCounter=400;

	}	
}
	
void TestIgbtTemperature(void)//TestStep11
{
	uint16_t igbttemp=0;
	igbttemp=CommuData[RSP_TEMPSINK_HI];
	igbttemp=(igbttemp<<8)+CommuData[RSP_TEMPSINK_LO];
	
	if ((igbttemp<(AdcQueue.ad_envronment+450))&&(igbttemp>(AdcQueue.ad_envronment-450)))
		  {
				TestStatus.test_step=12; 
				TestStatus.test_percent=55;		
			}
	  else
			{
				TestStatus.test_step=0; 
				TestStatus.error_code=11;		
        LedBeeper.beeper=0x1f;					
			}
    printf ("\n\r ad_envronment= %d",AdcQueue.ad_envronment);			
		printf ("\n\r igbttemp= %d",igbttemp);
	
}
	
void TestOverCurrentProtect(void)//TestStep12
{
	static uint16_t CurrentProtectCounter=300;
	static uint8_t CurrentProtectStep=0;	
	switch (CurrentProtectStep)
				{
					case 0://						
							if ((CommuData[RSP_ERROR_LO]&0x05)==0)
									{
										HAL_GPIO_WritePin(S_5V_GPIO_Port, S_5V_Pin, GPIO_PIN_SET);
										CurrentProtectStep=1;
										CurrentProtectCounter=400;
									}
						  else
							{
									TestStatus.test_step=0; 
									TestStatus.error_code=12;	
								  CurrentProtectStep=0;
								  printf ("\n\r overcurrent signal triggered error!");	
								  LedBeeper.beeper=0x1f;									
							}
						break;
					case 1:
						  if (CurrentProtectCounter>0)
						      CurrentProtectCounter--;
							if (((CommuData[RSP_ERROR_LO]&0x05)==0x05)&&(CurrentProtectCounter>0))
									{
										HAL_GPIO_WritePin(S_5V_GPIO_Port, S_5V_Pin, GPIO_PIN_RESET);
										CurrentProtectStep=0;
										TestStatus.test_step=13; 
				            TestStatus.test_percent=60;		
										CurrentProtectCounter=300;
										printf ("\n\r overcurrent test past !");
									}	
							if (CurrentProtectCounter==0)
								{
									HAL_GPIO_WritePin(S_5V_GPIO_Port, S_5V_Pin, GPIO_PIN_RESET);
									TestStatus.test_step=0; 
									TestStatus.error_code=12;	
                  CurrentProtectStep=0;		
									CurrentProtectCounter=300;
                  printf ("\n\r overcurrent signal not exists!");	
                  LedBeeper.beeper=0x1f;										
								}										
					break;
					default:
						break;
				}
}
	
void TestStaticVlotageAD(void)//TestStep13
{
	uint16_t vlotage=0;
	vlotage=CommuData[RSP_VOLTAGE_HI];
	vlotage=(vlotage<<8)+CommuData[RSP_VOLTAGE_LO];
		if ((vlotage<(AdcQueue.voltage +100))&&(vlotage>(AdcQueue.voltage-100)))
		  {
				TestStatus.test_step=14; 
				TestStatus.test_percent=65;		
			}
	  else
			{
				TestStatus.test_step=0; 
				TestStatus.error_code=13;	
			  LedBeeper.beeper=0x1f;					
			}
  printf ("\n\r StaticVlotage= %d",vlotage);
  printf ("\n\r RealVoltage= %d",AdcQueue.voltage);			
}
	
void TestStaticCurrentAD(void)//TestStep14
{
	uint16_t current=0;
	current=CommuData[RSP_CURRENT_HI];
	current=(current<<8)+CommuData[RSP_CURRENT_LO];
	if (current==0)
		  {
				TestStatus.test_step=15; 
				TestStatus.test_percent=70;		
			}
	  else
			{
				TestStatus.test_step=0; 
				TestStatus.error_code=14;	
			  LedBeeper.beeper=0x1f;					
			}	
  printf ("\n\r static current= %d",current);		  			
}
	
void TestPanChecker(void)//TestStep15
{
	static uint16_t PanCheckerCounter=0;
	static uint8_t PanCheckerStep=0;	

	switch (PanCheckerStep)
				{
					case 0://
						    LoadTestReference=1;
                HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);
					      PanCheckerStep=1;
						break;
					case 1:
						 PanCheckerCounter++;
					   if (PanCheckerCounter>20)
						 {
                PanCheckerCounter=200;	
                PanCheckerStep=2;							 
						 }									
					break;
					case 2:
								BoardControl.mask=0x2223;
								BoardControl.value=3000;	
					      if (PanCheckerCounter>0)
									 PanCheckerCounter--;

							 if (PanCheckerCounter==0)
							 {
								 PanCheckerStep=3;		
								 Signal.igbth=0;
								 Signal.igbtl=0;	
                 PanCheckerCounter=500;								 
							 }
					break;						 
					case 3://
								BoardControl.mask=0x2223;
								BoardControl.value=3000;	
					      if (PanCheckerCounter>0)
									 PanCheckerCounter--;

							 if (PanCheckerCounter==0)
                  PanCheckerStep=4;																		
						break;	
					case 4:
								if ((CommuData[RSP_LOADTEST]>1)&&(CommuData[RSP_LOADTEST]<12))
										{
											PanCheckerCounter=0;
											PanCheckerStep=0;
											TestStatus.test_step=16; 
											TestStatus.test_percent=75;	
											BoardControl.mask=0x2223;
											BoardControl.value=0;		
											printf ("\n\r LoadTestValue= %d",CommuData[RSP_LOADTEST]);												
										}	
                else 
										{
											printf ("\n\r loadvalue= %d",CommuData[RSP_LOADTEST]);	
											TestStatus.test_step=0; 
											TestStatus.error_code=15;		
											BoardControl.mask=0x2223;
											BoardControl.value=0;		
											PanCheckerStep=0;		
								      LedBeeper.beeper=0x1f;												
										}									
					break;								
					default:
						break;
				}	
}
	
void TestIgbthDriver(void)//TestStep16
{
		BoardControl.mask=0x2223;
		BoardControl.value=0;		
		if (Signal.igbth)
			 {
					TestStatus.test_step=17; 
					TestStatus.test_percent=80;
					BoardControl.mask=0x2223;
					BoardControl.value=0;	
          printf ("\n\r IGBTH test past !");						 
				}
			else
				{
					TestStatus.test_step=0; 
					TestStatus.error_code=16;	
          printf ("\n\r IGBTH test fail !");	
				  LedBeeper.beeper=0x1f;						
				}		
	
}
	
void TestIgbtlDriver(void)//TestStep17,jump options 18,20
{
		BoardControl.mask=0x2223;
		BoardControl.value=0;	
		if (Signal.igbtl)
			 {

					TestStatus.test_percent=85;
					BoardControl.mask=0x2223;
					BoardControl.value=0;	
					if (test_order==4)
						 TestStatus.test_step=18; 
					else
						 TestStatus.test_step=20; 	
          printf ("\n\r IGBTL test past !");						
				}
			else
				{
					TestStatus.test_step=0; 
					TestStatus.error_code=17;		
          printf ("\n\r IGBTL test fail !");		
					LedBeeper.beeper=0x1f;						
				}

}
	
void TestCalibrate(void)//TestStep18
{
	static uint16_t CalibrateCounter=0;
	static uint8_t CalibrateStep=0;	
	static uint16_t current1=0;
	static uint16_t current2=0;
	static uint16_t current1_ad=0;
	static uint16_t current2_ad=0;	
	static uint16_t cali_vol=0;
	static uint16_t cali_cur1=0;
	static uint16_t cali_cur2=0;	
	static uint16_t cali_cur3=0;	
	static uint16_t cali_cur4=0;
	static uint16_t cali_cur5=0;	
	static	uint16_t middle_current_bais=0;
	static	uint16_t high_current_bais=0;	
	static uint16_t decode_current_ad=0;
	static uint16_t mainboard_current=0;
	switch (CalibrateStep)
				{
					case 0://
								  BoardControl.mask=0x2223;
		              BoardControl.value=0;	
					        LoadTestReference=10;
                  TestStatus.test_percent=86;		
                  HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);	
                  HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_SET);	
                  CalibrateCounter++;
                  if (CalibrateCounter>200)					
									{CalibrateStep=1;
									CalibrateCounter=0;}
						break;
					case 1:
						      if (FlagSendMainBoard==0)
									{
											FlagSendMainBoard=1;//restore default calibrate parameters	
										  cali_vol=1;
										  cali_cur1=278;
										  cali_cur2=556;
										  cali_cur3=926;
										  cali_cur4=1296;
										  cali_cur5=1667;
											calibrate_table[3]=0;					
											calibrate_table[5]=(cali_vol<<4)+0;//CLB_VOLTAGE_LO
											calibrate_table[7]=(cali_vol<<4)>>8;//CLB_VOLTAGE_HI	
											calibrate_table[9]=(cali_cur1<<4)+1;//CLB_CURRENT1_LO	
											calibrate_table[11]=(cali_cur1<<4)>>8;//CLB_CURRENT1_HI	
											calibrate_table[13]=(cali_cur2<<4)+2;//CLB_CURRENT2_LO
											calibrate_table[15]=(cali_cur2<<4)>>8;//CLB_CURRENT2_HI
											calibrate_table[17]=(cali_cur3<<4)+3;//CLB_CURRENT3_LO
											calibrate_table[19]=((cali_cur3)<<4)>>8;//CLB_CURRENT3_HI	
											calibrate_table[21]=(cali_cur4<<4)+4;//CLB_CURRENT4_LO
											calibrate_table[23]=(cali_cur4<<4)>>8;//CLB_CURRENT4_HI	
											calibrate_table[25]=(cali_cur5<<4)+5;//CLB_CURRENT5_LO
											calibrate_table[27]=(cali_cur5<<4)>>8;//CLB_CURRENT5_HI	
											uint8_t j,k;
											k=0;
											for (j=0;j<28;j++)	
													{k=k+calibrate_table[j];}	
											calibrate_table[3]=0x100-k;		
									}										
									CalibrateCounter++;
									if (CalibrateCounter>250)
										 HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_SET);
									if (CalibrateCounter>300)
										{
											CalibrateStep=2;
											CalibrateCounter=0;	
											BoardControl.mask=0x2223;//start to heat
											BoardControl.value=3000;
											cali_vol=1;											
										  cali_cur1=0;
										  cali_cur2=0;
										  cali_cur3=0;
										  cali_cur4=0;
										  cali_cur5=0;
										}	
						break;
					case 2:						      
                  BoardControl.mask=0x2223;//adjust current to 1.36A 
								  BoardControl.value=3000;					        
									CalibrateCounter++;
									if (CalibrateCounter>600)
										{	                     								
											if (((CommuData[RSP_ERROR_LO]&0xa0)==0)&&(AdcQueue.current>80))//make sure stove keep power outputting
													{
														mainboard_current=CommuData[RSP_CURRENT_HI];
														mainboard_current=(CommuData[RSP_CURRENT_HI]<<8)+CommuData[RSP_CURRENT_LO];
														current1_ad=100*mainboard_current/54;
														current1=AdcQueue.current;
														printf ("\n\r current1_ad= %d",current1_ad);  	
														printf ("\n\r real_current1= %d",current1);																
														CalibrateStep=3;
														CalibrateCounter=0;														    
														BoardControl.mask=0x2223;//adjust current to 3A 
														BoardControl.value=6600;	
													}
											else
											{
													printf ("\n\r Calibrate Start heatting fail !");
													TestStatus.test_step=0; 
													TestStatus.error_code=18;		
													CalibrateStep=0;	
													CalibrateCounter=0;		
													LedBeeper.beeper=0x1f;	
													LoadTestReference=1;														
											}											
										}	                     									
						break;									
					case 3:
                  BoardControl.mask=0x2223;//adjust current to 3A 
								  BoardControl.value=6600;					        
									CalibrateCounter++;
									if (CalibrateCounter>600)
										{
											if (((CommuData[RSP_ERROR_LO]&0xa0)==0)&&(AdcQueue.current>150))//make sure stove keep power outputting
													{
														mainboard_current=CommuData[RSP_CURRENT_HI];
														mainboard_current=(CommuData[RSP_CURRENT_HI]<<8)+CommuData[RSP_CURRENT_LO];
														current2_ad=100*mainboard_current/54;
														current2=AdcQueue.current;
														cali_cur1=current2_ad-(current2-150)*(current2_ad-current1_ad)/(current2-current1);
														cali_cur2=current2_ad*300/AdcQueue.current;			
														printf ("\n\r current2_ad= %d",current2_ad);  
														printf ("\n\r real_current2= %d",current2);																													
														CalibrateStep=4;
														CalibrateCounter=0;
														BoardControl.mask=0x2223;//adjust current to 5A 
														BoardControl.value=11000;	
													}
											else
											{
													printf ("\n\r Heatting stop accidentally !");
													TestStatus.test_step=0; 
													TestStatus.error_code=18;		
													CalibrateStep=0;	
													CalibrateCounter=0;		
													LedBeeper.beeper=0x1f;	
													LoadTestReference=1;														
											}											
										}
						break;										
					case 4:
                  BoardControl.mask=0x2223;//adjust current to 5A 
								  BoardControl.value=11000;					        
									CalibrateCounter++;
									if (CalibrateCounter>600)
										{
											if (((CommuData[RSP_ERROR_LO]&0xa0)==0)&&(AdcQueue.current>350))//make sure stove keep power outputting
													{
														mainboard_current=CommuData[RSP_CURRENT_HI];
														mainboard_current=(CommuData[RSP_CURRENT_HI]<<8)+CommuData[RSP_CURRENT_LO];
														decode_current_ad=100*mainboard_current/54;
														cali_cur3=decode_current_ad*500/AdcQueue.current;		
														printf ("\n\r current3_ad= %d",decode_current_ad);  
														printf ("\n\r real_current3= %d",AdcQueue.current);		
														uint16_t MainboardVoltage;
														MainboardVoltage=(CommuData[RSP_VOLTAGE_HI]<<8)+CommuData[RSP_VOLTAGE_LO];
														if (AdcQueue.voltage>MainboardVoltage)//obtain voltage calibration data
																cali_vol=AdcQueue.voltage-MainboardVoltage;
														else
																cali_vol=1;														
														CalibrateStep=5;
														CalibrateCounter=0;
														BoardControl.mask=0x2223;//adjust current to 7A 
														BoardControl.value=15400;	
													}
											else
											{
													printf ("\n\r Heatting stop accidentally !");
													TestStatus.test_step=0; 
													TestStatus.error_code=18;		
													CalibrateStep=0;	
													CalibrateCounter=0;		
													LedBeeper.beeper=0x1f;	
													LoadTestReference=1;														
											}											
										}						
	                break;
					case 5:
                  BoardControl.mask=0x2223;//adjust current to 7A 
								  BoardControl.value=15400;					        
									CalibrateCounter++;
									if (CalibrateCounter>600)
										{
											if (((CommuData[RSP_ERROR_LO]&0xa0)==0)&&(AdcQueue.current>550))//make sure stove keep power outputting
													{
														mainboard_current=CommuData[RSP_CURRENT_HI];
														mainboard_current=(CommuData[RSP_CURRENT_HI]<<8)+CommuData[RSP_CURRENT_LO];
														decode_current_ad=100*mainboard_current/54;
														cali_cur4=decode_current_ad*700/AdcQueue.current;																
														printf ("\n\r current4_ad= %d",decode_current_ad);  
														printf ("\n\r real_current4= %d",AdcQueue.current);																
														CalibrateStep=6;
														CalibrateCounter=0;
														BoardControl.mask=0x2223;//adjust current to 9A 
														BoardControl.value=19800;	
													}
											else
											{
													printf ("\n\r Heatting stop accidentally !");
													TestStatus.test_step=0; 
													TestStatus.error_code=18;		
													CalibrateStep=0;	
													CalibrateCounter=0;		
													LedBeeper.beeper=0x1f;	
													LoadTestReference=1;														
											}											
										}																				
						break;	
					case 6:
                  BoardControl.mask=0x2223;//adjust current to 9A 
								  BoardControl.value=19800;					        
									CalibrateCounter++;
									if (CalibrateCounter>600)
										{
											if (((CommuData[RSP_ERROR_LO]&0xa0)==0)&&(AdcQueue.current>700))//make sure stove keep power outputting
													{
														mainboard_current=CommuData[RSP_CURRENT_HI];
														mainboard_current=(CommuData[RSP_CURRENT_HI]<<8)+CommuData[RSP_CURRENT_LO];
														decode_current_ad=100*mainboard_current/54;															
														cali_cur5=decode_current_ad*900/AdcQueue.current;	
														printf ("\n\r current5_ad= %d",decode_current_ad);  
														printf ("\n\r real_current5= %d",AdcQueue.current);					
														CalibrateStep=7;
														CalibrateCounter=0;
														BoardControl.mask=0x2223;//close output power
														BoardControl.value=0;	
													}
											else
											{
													printf ("\n\r Heatting stop accidentally !");
													TestStatus.test_step=0; 
													TestStatus.error_code=18;		
													CalibrateStep=0;	
													CalibrateCounter=0;		
													LedBeeper.beeper=0x1f;	
													LoadTestReference=1;														
											}											
										}														
						break;	
					case 7:		
                  BoardControl.mask=0x2223;//stop output
								  BoardControl.value=0;		
                 	CalibrateCounter++;
					        if (CalibrateCounter>300)
									{
										 CalibrateCounter=0;
											if (cali_cur3>926)//calculate the bias of current in case of substandard mainboard
													middle_current_bais=100*(cali_cur3-926)/926;
											else
													middle_current_bais=100*(926-cali_cur3)/926;
											if (cali_cur5>1667)
													high_current_bais=100*(cali_cur5-1667)/1667;
											else
													high_current_bais=100*(1667-cali_cur5)/1667;		
											
											if ((middle_current_bais<20)&&(high_current_bais<20))
													{

														CalibrateStep=8;
													}
											else
													{
															printf ("\n\r Calibration data out of range !");
															TestStatus.test_step=0; 
															TestStatus.error_code=19;		
															CalibrateStep=0;	
															CalibrateCounter=0;		
															LedBeeper.beeper=0x1f;	
															LoadTestReference=1;														
													}											
											printf ("\n\r cali-vol= %d",cali_vol);
											printf ("\n\r cali-cur1= %d",cali_cur1);
											printf ("\n\r cali-cur2= %d",cali_cur2);  
											printf ("\n\r cali-cur3= %d",cali_cur3);
											printf ("\n\r cali-cur4= %d",cali_cur4);	
											printf ("\n\r cali-cur5= %d",cali_cur5);	
										}											
						break;	
					case 8:
						      if (FlagSendMainBoard==0)
									{
											FlagSendMainBoard=1;//Write flash instruction of calibrate parameters	
											calibrate_table[3]=0;					
											calibrate_table[5]=cali_vol<<4;//CLB_VOLTAGE_LO
											calibrate_table[7]=(cali_vol<<4)>>8;//CLB_VOLTAGE_HI	
											calibrate_table[9]=(cali_cur1<<4)+1;//CLB_CURRENT1_LO	
											calibrate_table[11]=(cali_cur1<<4)>>8;//CLB_CURRENT1_HI	
											calibrate_table[13]=(cali_cur2<<4)+2;//CLB_CURRENT2_LO
											calibrate_table[15]=(cali_cur2<<4)>>8;//CLB_CURRENT2_HI
											calibrate_table[17]=(cali_cur3<<4)+3;//CLB_CURRENT3_LO
											calibrate_table[19]=((cali_cur3)<<4)>>8;//CLB_CURRENT3_HI	
											calibrate_table[21]=(cali_cur4<<4)+4;//CLB_CURRENT4_LO
											calibrate_table[23]=(cali_cur4<<4)>>8;//CLB_CURRENT4_HI	
											calibrate_table[25]=(cali_cur5<<4)+5;//CLB_CURRENT5_LO
											calibrate_table[27]=(cali_cur5<<4)>>8;//CLB_CURRENT5_HI	
											uint8_t j,k;
											k=0;
											for (j=0;j<28;j++)	
													{k=k+calibrate_table[j];}	
											calibrate_table[3]=0x100-k;		
									}										
									CalibrateCounter++;
									if (CalibrateCounter>300)
										{
											CalibrateStep=9;
											CalibrateCounter=0;
											HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);	
											HAL_GPIO_WritePin(S_220V_GPIO_Port,S_220V_Pin,GPIO_PIN_RESET);	
											BoardControl.mask=0x2223;//stop heat
											BoardControl.value=0;	
										}							
						break;
					case 9://
						    TestStatus.test_percent=95;						
                TestStatus.test_step=19;
								CalibrateStep=0;
					      LoadTestReference=1;
								CalibrateCounter=200;	
						break;									

											
					default:
						break;
				}	
}
	
void TestPowerCheck(void)//TestStep19
{
	
	TestStatus.test_step=20; 
	
}
	
void TestComplete(void)//TestStep20
{
		TestStatus.error_code=20;
		TestStatus.test_step=0;
	  TestStatus.test_percent =100;
	  HAL_GPIO_WritePin(S_LAMP_GPIO_Port,S_LAMP_Pin,GPIO_PIN_RESET);
	  ResetTest();
	  LedBeeper.beeper=0x31;
	  LedBeeper.led_pass=0x0f;
}
	
	

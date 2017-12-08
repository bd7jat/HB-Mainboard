#include "stdint.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "..\MyCode\global_variable.h"


#define MASK_Power 0x2223
#define MASK_Peripheral 0x2425

extern uint8_t  calibrate_table[28];
extern uint16_t task_delay[];
extern uint8_t FlagSendMainBoard;
extern uint8_t LoadTestReference;
extern uint32_t AdcTank[32];
extern uint8_t BoardReveivedData[50];
extern AD_parameter  AdcQueue;
extern flag Signal;
extern PeripheralState LedBeeper;	
extern TestState TestStatus;
extern BoardContent BoardControl;

void task2_Uart3Debug(void);
void task3_Uart2Comm(void);
void task4_Uart1HMI(void);
void Debug_output_str(uint8_t string[]);
void Debug_output_num(uint16_t number);
void Debug_output_str_num(uint8_t string[],uint16_t number);
void Uart1HMIReceive(void);
void Uart1HMISend(void);
void Uart1SendError(uint8_t error);
void Uart1SendBar(uint8_t value);
void Uart2BoardSend(void);
void Uart2BoardReceive(void);	
void SendBoardCMD(uint16_t mask,uint16_t value);

uint8_t  CommuData[MAX_CommuBuffer];
static uint8_t	HMIRx[6];		
static uint8_t  BoardRx[50];
static uint8_t  BoardTx[10];
  const uint8_t E0[]="process.t1.txt=error.t0.txt";
	const uint8_t E1[]="process.t1.txt=error.t1.txt";
	const uint8_t E2[]="process.t1.txt=error.t2.txt";
	const uint8_t E3[]="process.t1.txt=error.t3.txt";
	const uint8_t E4[]="process.t1.txt=error.t4.txt";
	const uint8_t E5[]="process.t1.txt=error.t5.txt";
	const uint8_t E6[]="process.t1.txt=error.t6.txt";
	const uint8_t E7[]="process.t1.txt=error.t7.txt";
	const uint8_t E8[]="process.t1.txt=error.t8.txt";
	const uint8_t E9[]="process.t1.txt=error.t9.txt";
	const uint8_t E10[]="process.t1.txt=error.t10.txt";
	const uint8_t E11[]="process.t1.txt=error.t11.txt";
	const uint8_t E12[]="process.t1.txt=error.t12.txt";
	const uint8_t E13[]="process.t1.txt=error.t13.txt";
	const uint8_t E14[]="process.t1.txt=error.t14.txt";
	const uint8_t E15[]="process.t1.txt=error.t15.txt";
	const uint8_t E16[]="process.t1.txt=error.t16.txt";
	const uint8_t E17[]="process.t1.txt=error.t17.txt";
	const uint8_t E18[]="process.t1.txt=error.t18.txt";
	const uint8_t E19[]="process.t1.txt=error.t19.txt";
	const uint8_t E20[]="process.t1.txt=error.t20.txt";

void task2_Uart3Debug(void)
{
	task_delay[2]=1001;
	//printf ("\n\r voltage_ad= %d",AdcQueue.ad_voltage);
	//printf ("\n\r current_ad= %d",AdcQueue.ad_current);
	//printf ("\n\r voltage= %d",vol);
	//printf ("\n\r voltage= %d",AdcQueue.voltage);
	//printf ("\n\r current= %d",AdcQueue.current);
  //printf ("\n\r ad_18v= %d",AdcQueue.ad_18v);
	//printf ("\n\r ad_3v3= %d",AdcQueue.ad_3v3);
	//printf ("\n\r ad_i3v3= %d",AdcQueue.ad_i3v3);
	//printf ("\n\r ad_envronment= %d",AdcQueue.ad_envronment);
	//printf ("\n\r ad_surge= %d",AdcQueue.ad_surge);
	//printf ("\n\r ad_transformer= %d",AdcQueue.ad_transformer );
	//HAL_UART_Transmit_DMA(&huart3,CommuData,0x7f);
	//HAL_UART_Transmit_DMA(&huart3,BoardReveivedData,36);
}

void task3_Uart2Comm(void)
{
	task_delay[3]=50;
 	static uint8_t Board_send_receive_chance=0;
	if (Board_send_receive_chance==0)
	    Board_send_receive_chance=1;
  else
      Board_send_receive_chance=0;
	
	if (Board_send_receive_chance==0)
     Uart2BoardSend();
	if (Board_send_receive_chance>0) 
		 Uart2BoardReceive();		 

}

void task4_Uart1HMI(void)
{
	task_delay[4]=101;
	static uint8_t HMI_send_receive_chance=0;
	if (HMI_send_receive_chance==0)
	    HMI_send_receive_chance=1;
  else
      HMI_send_receive_chance=0;

	if (HMI_send_receive_chance==0)
     Uart1HMISend();
	if (HMI_send_receive_chance>0) 
		 Uart1HMIReceive();		
}

void Uart2BoardSend(void)
{
	if (FlagSendMainBoard)
	{
		FlagSendMainBoard=0;
		HAL_UART_Transmit_DMA(&huart2,calibrate_table,28);
	}
	else
	{
			if (BoardControl.mask)
			{
				SendBoardCMD(BoardControl.mask,BoardControl.value);
				BoardControl.mask=0;
				BoardControl.value=0;
			}
	}
}

void SendBoardCMD(uint16_t mask,uint16_t value)
{
	uint8_t i,j;
	BoardTx[0]=0x41;
	BoardTx[1]=0x4b;
	BoardTx[2]=0x0a;
	BoardTx[3]=0x00;
	BoardTx[4]=mask>>8;
	BoardTx[5]=value;
	BoardTx[6]=mask;
	BoardTx[7]=value>>8;
  BoardTx[8]=0x26;
	BoardTx[9]=LoadTestReference;	
	j=0;
	for (i=0;i<10;i++)
	    {
				j=j+BoardTx[i];
			}			
	BoardTx[3]=0x100-j;	
	HAL_UART_Transmit_DMA(&huart2,BoardTx,10);
}

void Uart2BoardReceive(void)
{
	static uint8_t counter_com=20;
 
	if (counter_com>0)
	    counter_com--;
	uint8_t i,j;
	if (Signal.uart2_rx)
	{
		HAL_UART_DMAStop(&huart2);
		Signal.uart2_rx=0;
		j=0;
		for(i=0;i<BoardRx[2];i++)
			{j=j+BoardRx[i];}
		if ((BoardRx[0]==0x41)&&(BoardRx[1]==0x4b)&&(j==0))
			{
				//HAL_GPIO_TogglePin(LED_FAIL_GPIO_Port,LED_FAIL_Pin);
				counter_com=20;
				for(i=4;i<BoardRx[2];i=i+2)
            {
							CommuData[BoardRx[i]]=BoardRx[i+1];
						}
				for (i=0;i<50;i++)
						{
							BoardReveivedData[i]=BoardRx[i];
						}
			}
	if (counter_com<10)
		 HAL_UART_Init(&huart2);

	HAL_UART_Receive_DMA(&huart2,BoardRx,50);

	//HAL_UART_Receive_IT(&huart2,BoardRx,50);	
  }
	
}

void Uart1HMISend(void)
{
	static uint8_t send_chance=0;
	if (send_chance==0)
	    send_chance=1;
  else
      send_chance=0;

	if (send_chance==0)
     Uart1SendError(TestStatus.error_code);
	if (send_chance>0) 
		 Uart1SendBar(TestStatus.test_percent);
}

void Uart1HMIReceive(void)
{
	uint8_t i,j;

	if (Signal.uart1_rx)
	{
		HAL_UART_DMAStop(&huart1);
		Signal.uart1_rx=0;
		j=0;
		for(i=0;i<6;i++)
			{j=j+HMIRx[i];}
		if ((HMIRx[0]==0x41)&&(HMIRx[1]==0x4b)&&(j==0))
			{
				LedBeeper.beeper=0x11;
				TestStatus.test_instruction = HMIRx[5];
			}
	  else
			HAL_UART_Init(&huart1);
		
	  HAL_UART_Receive_DMA(&huart1,HMIRx,6);
			
	}		

}

void Uart1SendBar(uint8_t value)
{
	static uint8_t Bar[]="process.j0.val=";
	static uint8_t BarArray[25];
	uint8_t i;
	for(i=0;i<15;i++)
	{
		BarArray[i]=Bar[i];
	}
	BarArray[15]=value/100+48;
	value=value%100;
	BarArray[16]=value/10+48;
	value=value%10;
	BarArray[17]=value+48;
	BarArray[18]=0xff;
	BarArray[19]=0xff;
	BarArray[20]=0xff;
	HAL_UART_Transmit_DMA(&huart1,BarArray,21);	
}

void Uart1SendError(uint8_t error)
{
	static uint8_t ErrorArray[40];
	uint8_t i;
  switch (error)
							{
									case 0://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E0[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);											
										break;
									case 1://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E1[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;
									case 2://
                     for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E2[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);								 				
										break;
									case 3://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E3[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);											
										break;
									case 4://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E4[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;
									case 5://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E5[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;	
									case 6://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E6[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;	
									case 7://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E7[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;	
									case 8://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E8[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;	
									case 9://
                    for(i=0;i<27;i++)
									  {
											ErrorArray[i]=E9[i];
										}
										ErrorArray[27]=0xff;
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,30);	
										break;	
									case 10://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E10[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 11://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E11[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 12://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E12[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;
									case 13://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E13[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 14://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E14[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 15://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E15[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;		
									case 16://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E16[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;
									case 17://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E17[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 18://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E18[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;	
									case 19://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E19[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;		
									case 20://
                    for(i=0;i<28;i++)
									  {
											ErrorArray[i]=E20[i];
										}
										ErrorArray[28]=0xff;
										ErrorArray[29]=0xff;
										ErrorArray[30]=0xff;
										HAL_UART_Transmit_DMA(&huart1,ErrorArray,31);	
										break;												
									default://
										break;
					}
		

}

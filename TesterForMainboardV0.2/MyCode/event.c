#include "stdint.h"
#include "gpio.h"
void task_countdown(void);
void task_scheduler(void);

extern void task0_AdSample(void);
extern void task1_TestHandle(void);
extern void task2_Uart3Debug(void);
extern void task3_Uart2Comm(void);
extern void task4_Uart1HMI(void);
extern void task5_PeripheralControl(void);
extern void task6_FeedDog(void);
/*task scheduler based on time distribution  

*/
//////////////////////////////////////////////////////////////////////
#define	MAX_TASK		7

uint16_t task_delay[MAX_TASK];

static void (*ptask[])()={task0_AdSample,task1_TestHandle,task2_Uart3Debug,task3_Uart2Comm,task4_Uart1HMI,task5_PeripheralControl,task6_FeedDog}; //fetch the PC of each task founction

void task_countdown(void)
{
	uint8_t i;
  for(i=0;i<MAX_TASK;i++)
	  {
			if((task_delay[i]>0)&&(task_delay[i]<0xffff))// Set task_delay[i] as 0xffff to suspend founction and never execute
				task_delay[i]--;
		}
}	
	
void task_scheduler(void) 
{
	uint8_t i;
	for(i=0;i<MAX_TASK;i++)
	     task_delay[i]=0;
	
  static uint8_t schedule_step;
	while (1) 
		{
       for(schedule_step=0;schedule_step<MAX_TASK;schedule_step++)
			     {
						 if (task_delay[schedule_step]==0)						 
						    {
									ptask[schedule_step]();						 
						      break;
								}
					 }
       
		}
	
}
//////////////////////////////////////////////////////////////////////


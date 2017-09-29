#include "stdint.h"
#include "iwdg.h"

extern uint16_t task_delay[];

void task6_FeedDog(void);

void task6_FeedDog(void)
{
	task_delay[6]=103;
	//HAL_IWDG_Refresh(&hiwdg);
}



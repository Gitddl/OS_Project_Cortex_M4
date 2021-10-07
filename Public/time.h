#ifndef __TIME_H
#define __TIME_H 	

#include <sys.h>	  

void time_systick_init(u8 SYSCLK);

void Time_delay_ms(u16 nms);
void Time_us(u32 nus);

#endif






























#ifndef _led_H
#define _led_H

// #include "sys.h"


#define led1 PFout(9)
#define led2 PFout(10)


void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_Trigger(void);

#endif

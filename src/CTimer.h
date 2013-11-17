#ifndef __TIMER_H__
#define __TIMER_H__
//***********************************************************************
// FILE: CTimer.h
// AUTHOR: Yuriy Skriyka
// DATE: 29-08-2010 
// DESCRIPTION: The timer class. Implements a driver over the hardware timer.
//************************************************************************

#include <inttypes.h>
#include <avr/interrupt.h>

// forward declaration of the interrupt
extern "C" void TIMER2_COMP_vect(void) __attribute__ ((signal));



class CTimer
{
public:

   static void init(void);

   static uint32_t getFullTimer(void);

   static uint16_t getLoTimer(void);

   static void sleep(uint16_t ms);

private:
   friend void TIMER2_COMP_vect(void); // the interrupt must be a friend to access private fields

   static volatile uint32_t mTicks;  // timer ticks in milliseconds
};

#endif


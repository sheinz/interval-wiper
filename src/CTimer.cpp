//***********************************************************************
// FILE: CTimer.cpp
// AUTHOR: Yuriy Skriyka
// DATE: 29-08-2010 
// DESCRIPTION: The timer class. Implements a driver over the hardware timer.
//************************************************************************
#include "CTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// ----------------------------------------------------------------------------

volatile uint32_t CTimer::mTicks; 

// ----------------------------------------------------------------------------

ISR(TIMER2_COMP_vect)
{
   CTimer::mTicks++;
}

// ----------------------------------------------------------------------------

void CTimer::init(void)
{
   TCNT2 = 0;
   OCR2 = 250;    // with prescaler F_CPU/32 each 250 tick will be exactly 1ms

   mTicks = 0;

	TCCR2 = _BV(CS21) | _BV(CS20);	// the prescaler is F_CPU/32
   TCCR2 |= _BV(WGM21);             // clear timer on compare match

	TIMSK |= _BV(OCIE2);    // enable compare match interrupt
}

// -----------------------------------------------

uint32_t CTimer::getFullTimer(void)
{
	uint32_t t;
	uint8_t sreg = SREG;

	cli();
	t = mTicks;
	
	SREG = sreg;

	return (t);
}

// -----------------------------------------------

uint16_t CTimer::getLoTimer(void)
{
	return ((uint16_t)getFullTimer());
}

// -----------------------------------------------

void CTimer::sleep(uint16_t ms)
{
	uint32_t t = getFullTimer() + ms;

	while (getFullTimer() < t);
}

//***********************************************************************
// FILE: CWiper.cpp
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The calss implements a driver over the relay that controls the wiper motor.
//************************************************************************

#include "CWiper.h"
#include <avr/io.h>


#define RELAY_PORT      PORTD
#define RELAY_DDR       DDRD
#define RELAY_PIN       PD5

#define RELAY_ON()      RELAY_PORT |= _BV(RELAY_PIN)
#define RELAY_OFF()     RELAY_PORT &= ~_BV(RELAY_PIN)

// ----------------------------------------------------------------------------
// The length of the impulse to start single wiper movement
#define PULSE_DELAY           (700)         // 700ms
#define FAST_MODE_DELAY       (3000)         // 3 sec
#define MIDDLE_MODE_DELAY     (7000)         // 7 sec
#define SLOW_MODE_DELAY       (15000)        // 15 sec

// ----------------------------------------------------------------------------

CWiper::CWiper(){}

// ----------------------------------------------------------------------------

void CWiper::init(void)
{
   RELAY_DDR |= _BV(RELAY_PIN);       // set pin to output
   
   RELAY_OFF();      // initial state

   //mMode = MODE_MIDDLE; // it really doesn't matter
}

// ----------------------------------------------------------------------------

void CWiper::turnOn(void)
{
   RELAY_ON();

   TaskMgr.Remove(this);         // stop delay mode
   TaskMgr.Remove(&mPulseGen);    // stop pulse generator as well
}

// ----------------------------------------------------------------------------

void CWiper::turnOff(void)
{
   RELAY_OFF();

   TaskMgr.Remove(this);         // stop delay mode
   TaskMgr.Remove(&mPulseGen);    // stop pulse generator as well
}

// ----------------------------------------------------------------------------

void CWiper::delayMode(const eMode mode)
{
   //mMode = mode;
   uint16_t delay;

   mPulseGen.pulse();

   switch (mode)
   {
   case MODE_FAST:
      delay = FAST_MODE_DELAY;
      break;
   case MODE_MIDDLE:
      delay = MIDDLE_MODE_DELAY;
      break;
   case MODE_SLOW:
      delay = SLOW_MODE_DELAY;
      break;
   default:
      delay = MIDDLE_MODE_DELAY;    // just in case
   };

   TaskMgr.Add(this, delay);
}

// ----------------------------------------------------------------------------

void CWiper::onExecute(void)
{
   mPulseGen.pulse();
}

// ----------------------------------------------------------------------------

CWiper::CPulseGen::CPulseGen() {}

// ----------------------------------------------------------------------------

void CWiper::CPulseGen::onExecute(void)
{
   RELAY_OFF();
   TaskMgr.Remove(this);      
}

// ----------------------------------------------------------------------------

void CWiper::CPulseGen::pulse(void)
{
   RELAY_ON();
   TaskMgr.Add(this, PULSE_DELAY);
}
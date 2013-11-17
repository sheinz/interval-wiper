//***********************************************************************
// FILE: CSwitch.cpp
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The class implements a driver that scans the switch of the rear wiper.
//************************************************************************
#include "CSwitch.h"
#include <avr/io.h>
#include "CTimer.h"

// version 2 with non-inverted input
#define VERSION_2

// ----------------------------------------------------------------------------

#define CLICK_DELAY     700     // 1sec

#define SWITCH_PORT     PINC
#define SWITCH_DDR      DDRC
#define SWITCH_PIN      PC5

#ifdef VERSION_2
   #define CHECK_SWITCH()     ( (SWITCH_PORT & _BV(SWITCH_PIN)) ? (true) : (false) )    
#else
   // The switch is inverted
   #define CHECK_SWITCH()     ( (SWITCH_PORT & _BV(SWITCH_PIN)) ? (false) : (true) )    
#endif  // VERSION_2
// ----------------------------------------------------------------------------

void CSwitch::init(ISwitchEvent *pEventReceiver)
{
   mpEventRecevier = pEventReceiver;

   TaskMgr.Add(this, 100);    // the period of scanning the switch

   SWITCH_DDR &= _BV(SWITCH_PIN);

   mSwitchState = false;      // don't check it here
   mTimeStamp = CTimer::getFullTimer();
}

// ----------------------------------------------------------------------------

void CSwitch::onExecute(void)
{
   bool currState = CHECK_SWITCH();
   uint32_t delay;

   if (currState != mSwitchState)   // if the state has changed
   {
      mSwitchState = currState;
      uint32_t timeStamp = CTimer::getFullTimer();

      if (mSwitchState)    // the switch has been turned ON
      {
         mpEventRecevier->onSwitchOn();
      }
      else           // the switch has been turned OFF
      {
         delay = timeStamp - mTimeStamp;

         if (delay < CLICK_DELAY)
         {
            mpEventRecevier->onSwitchClick();
         }
         else     // the delay is longer, it's not a click
         {
            mpEventRecevier->onSwitchOff();
         }
      }
      mTimeStamp = timeStamp;    // remember current time stamp
   }
}
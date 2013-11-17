//***********************************************************************
// FILE: CControl.h
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The class implements the main logic. It receives events form
// the switch and changes modes of the wiper.
//************************************************************************
#include "CControl.h"

// ----------------------------------------------------------------------------

void CControl::init(void)
{
   mSwitch.init(this);
   mWiper.init();

   mMode = MODE_OFF;
}

// ----------------------------------------------------------------------------

void CControl::onSwitchOn(void)
{
   // don't change the mode here as this event arrives each click

   mWiper.turnOn();
}

// ----------------------------------------------------------------------------

void CControl::onSwitchOff(void)
{
   mMode = MODE_OFF;

   mWiper.turnOff();
}

// ----------------------------------------------------------------------------

void CControl::onSwitchClick(void)
{
   switch (mMode)
   {
   case MODE_OFF:     // first click
      mMode = MODE_SLOW;
      break;
   case MODE_SLOW:   // second click
      mMode = MODE_MIDDLE;
      break;
   case MODE_MIDDLE:
      mMode = MODE_FAST;
      break;
   case MODE_FAST:
      mMode = MODE_SLOW;
      break;
   default:
      mMode = MODE_SLOW;      // this should not happen
   }

   mWiper.delayMode(mMode);
}
#ifndef __CCONTROL_H__
#define __CCONTROL_H__
//***********************************************************************
// FILE: CControl.h
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The class implements the main logic. It receives events form
// the switch and changes modes of the wiper.
//************************************************************************

#include <inttypes.h>
#include "CSwitch.h"
#include "CWiper.h"

class CControl : public ISwitchEvent
{
public:
   void init(void);

private:

   // This event occurs just as the switch is turned on
   virtual void onSwitchOn(void);

   // This event occurs only if the switch has been turned on for longer than 2 sec, 
   // otherwise "onSwitchClick" event occurs
   virtual void onSwitchOff(void);

   // This event occurs if the switch is turned off right after it was turned on. Like mouse click.
   // If this event occurs "onSwitchOff" will not happen
   virtual void onSwitchClick(void);


   // Declaration of the object that manages the switch
   CSwitch mSwitch;

   // Declaration of the object that manages the relay
   CWiper mWiper;

   // Current interval mode
   eMode mMode;
};

#endif // __CCONTROL_H__
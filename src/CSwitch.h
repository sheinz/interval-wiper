#ifndef __CSWITCH_H__
#define __CSWITCH_H__
//***********************************************************************
// FILE: CSwitch.h
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The class implements a driver that scans the switch of the rear wiper.
//************************************************************************

#include <inttypes.h>
#include "CTaskMgr.h"


// The interface class to receive events
class ISwitchEvent
{
public:
   // This event occurs just as the switch is turned on
   virtual void onSwitchOn(void) = 0;
   
   // This event occurs only if the switch has been turned on for longer than 2 sec, 
   // otherwise "onSwitchClick" event occurs
   virtual void onSwitchOff(void) = 0;
   
   // This event occurs if the switch is turned off right after it was turned on. Like mouse click.
   // If this event occurs "onSwitchOff" will not happen
   virtual void onSwitchClick(void) = 0;
};


// ----------------------------------------------------------------------------

class CSwitch : public ITask
{
public:
      
   void init(ISwitchEvent *pEventReceiver);

private:

   virtual void onExecute(void);


   // The pinter to the event receiver
   ISwitchEvent *mpEventRecevier;

   // The current state of the switch
   bool mSwitchState;

   // The time stamp of the last change of the switch state
   uint32_t mTimeStamp;
};


#endif // __CSWITCH_H__
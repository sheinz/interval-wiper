#ifndef __CWIPER_H__
#define __CWIPER_H__
//***********************************************************************
// FILE: CWiper.h
// AUTHOR: Yuriy Skriyka
// DATE: 23-01-2012 
// DESCRIPTION: The calss implements a driver over the relay that controls the wiper motor.
//************************************************************************

#include <inttypes.h>
#include "CTaskMgr.h"

// ----------------------------------------------------------------------------

enum eMode
{
   MODE_OFF = 0,
   MODE_ON,
   MODE_SLOW,
   MODE_MIDDLE,
   MODE_FAST,
};

// ----------------------------------------------------------------------------

class CWiper : public ITask
{
public:

   CWiper();

   virtual void init(void);

   void turnOn(void);

   void turnOff(void);

   void delayMode(const eMode mode);
   
private:

   virtual void onExecute(void);

   // The class generates single wiper swing
   class CPulseGen: public ITask
   {
   private:
      virtual void onExecute(void);

   public:
      CPulseGen();

      // Generate the pulse to start single swing of the wiper
      void pulse(void);
   };


   // current mode
   //Mode mMode;

   // The object to generate one pulse for the relay to make single wiper swing
   CPulseGen mPulseGen;
};


#endif // __CWIPER_H__
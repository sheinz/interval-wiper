
#include <inttypes.h>
#include <avr/interrupt.h>
#include "CTimer.h"
#include "CTaskMgr.h"
#include "CControl.h"
#include "avr/wdt.h"

// HFUSE=0x89   // WDTON = 0, CKOPT=0 (Increase oscillator amplitude) 

CControl Control;

int main(void)
{
   wdt_enable(WDTO_250MS);

   CTimer::init();    

   TaskMgr.init();
   
   sei();

   Control.init();      // initialize all

   while (true)
   {
      wdt_reset();
      TaskMgr.processTasks();
   }

   return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
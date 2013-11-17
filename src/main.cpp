
#include <inttypes.h>
#include <avr/interrupt.h>
#include "CTimer.h"
#include "CTaskMgr.h"
#include "CControl.h"
#include "avr/wdt.h"

FUSES =
{
   .low = (FUSE_SUT0 & FUSE_CKSEL3 & FUSE_CKSEL1 & FUSE_CKSEL0),
   .high = (FUSE_WDTON & FUSE_SPIEN & FUSE_CKOPT & FUSE_BOOTSZ1 & FUSE_BOOTSZ0),
};

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

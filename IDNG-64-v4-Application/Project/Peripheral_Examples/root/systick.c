#include "stm32f0xx.h"
#include "systick.h"
#include "wdt.h"
#include "daliUtils.h"
#include "ethTransceiver.h"
#include "historico.h"

static __IO uint32_t _sysTickTimingDelay;

void sysTickTick (void)
{
	if (_sysTickTimingDelay!= 0x00)
  { 
    _sysTickTimingDelay--;
  }
	
	ethTransceiverTick();
	ethICMPTick();
	daliUtilsTick ();
	historicoTick();
	botoneraTick();
	groupsTick();
	sensoresSysTick ();
}






void sysTickDelay (__IO uint32_t nTime)
{
	_sysTickTimingDelay = nTime;
  while(_sysTickTimingDelay != 0)
	{
		wdtDoTask();
	}		
}

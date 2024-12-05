#include "stm32f0xx.h"
#include "fallos.h"

unsigned char _wdtReboot;

unsigned char _wdtTickCnt;


void wdtReboot (void)
{
	_wdtReboot=0x01;
}



void wdtTick (void)
{
	if (_wdtTickCnt>0)	_wdtTickCnt--;
	
}


void wdtReloadEnable (void)
{
	_wdtTickCnt=30;	
}
	
	






void wdtInit (void)
{	
	
	
	  // @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready  
//             @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
 //           @arg RCC_FLAG_PLLRDY: PLL clock ready
    //         @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
     //       @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
   //         @arg RCC_FLAG_OBLRST: Option Byte Loader (OBL) reset 
   //         @arg RCC_FLAG_PINRST: Pin reset
   //         @arg RCC_FLAG_V18PWRRSTF:  V1.8 power domain reset  
    //         @arg RCC_FLAG_PORRST: POR/PDR reset
    //         @arg RCC_FLAG_SFTRST: Software reset
    //        @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
     //       @arg RCC_FLAG_WWDGRST: Window Watchdog reset
     //       @arg RCC_FLAG_LPWRRST: Low Power reset
     //       @arg RCC_FLAG_HSI14RDY: HSI14 oscillator clock ready  
	
	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    // IWDGRST flag set //
		//fallosFalloResetSet();		
  }
  else
  {
    // IWDGRST flag is not set //
  }	
	
	fallosFalloResetSet();		
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload(0xFFF);
	IWDG_ReloadCounter();
	IWDG_Enable();
	
	
}






void wdtDoTask (void)
{
	if (_wdtReboot==0x00)
	{
		IWDG_ReloadCounter();	
	}
}




void wdtReload (void)
{
	if (_wdtTickCnt>0)	wdtDoTask ();
}



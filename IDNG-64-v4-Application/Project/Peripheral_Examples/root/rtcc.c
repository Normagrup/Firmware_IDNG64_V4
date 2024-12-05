#include "stm32f0xx.h"
#include "polling.h"
#include "rtcc.h"
#include "temp.h"
#include "test.h"
#include "normalink.h"
#include "ethTransceiver.h"

void rtccInit (void)
{
	RTC_InitTypeDef  _RTC_InitStructure;


	RTC_AlarmTypeDef	_RTC_AlarmStructure;
	EXTI_InitTypeDef	_EXTI_InitStructure;
	NVIC_InitTypeDef	_NVIC_InitStructure;
	RCC_DeInit();
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);	
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* Configure the RTC data register and RTC prescaler */
  /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
	_RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	//_RTC_InitStructure.RTC_AsynchPrediv = 0x01;
  _RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  _RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&_RTC_InitStructure);
  RCC_RTCCLKCmd(ENABLE);
  RTC_WaitForSynchro();
	RTC_ExitInitMode();	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	RTC_AlarmStructInit(&_RTC_AlarmStructure);
	_RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes | RTC_AlarmMask_Seconds; 
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &_RTC_AlarmStructure);		
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
		
	RTC_CalibOutputCmd(ENABLE);
		
  /* EXTI configuration */		
  EXTI_ClearITPendingBit(EXTI_Line17);
  _EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  _EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  _EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  _EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&_EXTI_InitStructure);
	_NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  _NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  _NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&_NVIC_InitStructure);
  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);  
  /* Enable the RTC Alarm A interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);		
 /* Clear the Alarm A Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALRA);   
}

unsigned char rtccYearGet (void)
{
	unsigned char _year;
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);
	_year=_RTC_DateStruct.RTC_Year;
	return _year;	
}

void rtccYearSet (unsigned char year)
{
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);
	_RTC_DateStruct.RTC_Year=year;
	RTC_SetDate(RTC_Format_BCD, &_RTC_DateStruct);
}

unsigned char rtccMonthGet (void)
{
	unsigned char _month;
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);
	_month=_RTC_DateStruct.RTC_Month;
	return _month;	
}

void rtccMonthSet (unsigned char month)
{
	RTC_DateTypeDef	_RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);
	_RTC_DateStruct.RTC_Month=month;
	RTC_SetDate(RTC_Format_BCD, &_RTC_DateStruct);
}

unsigned char rtccWeekdayGet(void)
{
	unsigned char _weekday;	
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);	
	_weekday=_RTC_DateStruct.RTC_WeekDay;
	return _weekday;	
}

void rtccWeekdaySet(unsigned char weekday)
{	
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);	
	_RTC_DateStruct.RTC_WeekDay=weekday;
	RTC_SetDate(RTC_Format_BCD, &_RTC_DateStruct);	
}


unsigned char rtccDayGet(void)
{
	
	unsigned char _day;
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);	
	_day=_RTC_DateStruct.RTC_Date;	
	return _day;		
}

void rtccDaySet (unsigned char day)
{	
	RTC_DateTypeDef	 _RTC_DateStruct;
	RTC_GetDate(RTC_Format_BCD, &_RTC_DateStruct);	
	_RTC_DateStruct.RTC_Date=day;
	RTC_SetDate(RTC_Format_BCD, &_RTC_DateStruct);		
}

unsigned char rtccHourGet (void)
{
	unsigned char _hour;
	RTC_TimeTypeDef  _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);
	_hour=_RTC_TimeStruct.RTC_Hours;
	return _hour;
}

void rtccHourSet (unsigned char hour)
{
	RTC_TimeTypeDef  _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);	
	_RTC_TimeStruct.RTC_Hours=hour;
	RTC_SetTime(RTC_Format_BCD, &_RTC_TimeStruct);	
}

unsigned char rtccMinutesGet (void)
{
	unsigned char _minutes;
	RTC_TimeTypeDef _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);
	_minutes=_RTC_TimeStruct.RTC_Minutes;
	return _minutes;
}

void rtccMinutesSet (unsigned char minutes)
{
	RTC_TimeTypeDef  _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);	
	_RTC_TimeStruct.RTC_Minutes=minutes;	
	RTC_SetTime(RTC_Format_BCD, &_RTC_TimeStruct);		
}

unsigned char rtccSecondsGet (void)
{
	unsigned char _seconds;
	RTC_TimeTypeDef _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);
	_seconds=_RTC_TimeStruct.RTC_Seconds;
	return _seconds;
}

void rtccSecondsSet (unsigned char seconds)
{	
	RTC_TimeTypeDef  _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);	
	_RTC_TimeStruct.RTC_Seconds=seconds;	
	RTC_SetTime(RTC_Format_BCD, &_RTC_TimeStruct);
}


void rtccSecondsTick (void)
{
	RTC_TimeTypeDef _RTC_TimeStruct;	
	RTC_GetTime(RTC_Format_BCD, &_RTC_TimeStruct);
	 if (_RTC_TimeStruct.RTC_Seconds==0x00)
	 {
		 rtccMinutesTick ();
	 }
	
	 sensoresRtcTick();
		polling_Tick();
		daliLoopRtccTick();
		wdtTick();
		tempTick();
}




void rtccMinutesTick (void)
{
	testRTCC_Tick();
	tempCheck();

	normaLinkTick();
	ethTransceiverRTCC_Tick();
}









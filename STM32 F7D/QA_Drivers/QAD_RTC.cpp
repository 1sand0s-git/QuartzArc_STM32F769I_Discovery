/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: RTC Driver                                                      */
/*   Filename: QAD_RTC.cpp                                                 */
/*   Date: 18th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_RTC.hpp"

#include <string.h>


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

	//------------------------------
	//------------------------------
	//QAD_RTC Initialization Methods

//QAD_RTC::imp_init
//QAD_RTC Initialization Method
QA_Result QAD_RTC::imp_init(void) {
	if (m_eInitState)
		return QA_OK;

	//Enable RTC Clock
	__HAL_RCC_RTC_CLK_ENABLE();
	__HAL_RCC_RTC_ENABLE();

	//Init RTC Peripheral
	m_sHandle.Instance               = RTC;
	m_sHandle.Init.HourFormat        = RTC_HOURFORMAT_24;
	m_sHandle.Init.AsynchPrediv      = 127;
	m_sHandle.Init.SynchPrediv       = 255;
	m_sHandle.Init.OutPut            = RTC_OUTPUT_DISABLE;
	m_sHandle.Init.OutPutPolarity    = RTC_OUTPUT_POLARITY_HIGH;
	m_sHandle.Init.OutPutType        = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&m_sHandle) != HAL_OK) {
		__HAL_RCC_RTC_DISABLE();
		__HAL_RCC_RTC_CLK_DISABLE();
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


//QAD_RTC::imp_deinit
//QAD_RTC Initialization Method
void QAD_RTC::imp_deinit(void) {
  if (!m_eInitState) {
  	return;
  }

	//Deinit RTC Peripheral
	HAL_RTC_DeInit(&m_sHandle);

	//Disbale Clocks
	__HAL_RCC_RTC_DISABLE();
	__HAL_RCC_RTC_CLK_DISABLE();

}


  //-----------------------
	//-----------------------
	//QAD_RTC Control Methods

//QAD_RTC::imp_setTime
//QAD_RTC Control Method
QA_Result QAD_RTC::imp_setTime(uint8_t uHour, uint8_t uMinute, uint8_t uSecond) {

	m_sTime.Hours              = uHour;
	m_sTime.Minutes            = uMinute;
	m_sTime.Seconds            = uSecond;
	m_sTime.SecondFraction     = 0;
	m_sTime.DayLightSaving     = RTC_DAYLIGHTSAVING_NONE;
	m_sTime.StoreOperation     = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&m_sHandle, &m_sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


//QAD_RTC::imp_setDate
//QAD_RTC Control Method
QA_Result QAD_RTC::imp_setDate(uint8_t uWeekDay, uint8_t uDay, uint8_t uMonth, uint8_t uYear) {

	m_sDate.WeekDay    = uWeekDay;
	m_sDate.Date       = uDay;
	m_sDate.Month      = uMonth;
	m_sDate.Year       = uYear;
	if (HAL_RTC_SetDate(&m_sHandle, &m_sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


//QAD_RTC::imp_update
//QAD_RTC Control Method
QA_Result QAD_RTC::imp_update(void) {

	if (HAL_RTC_GetTime(&m_sHandle, &m_sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}
	if (HAL_RTC_GetDate(&m_sHandle, &m_sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


  //--------------------
	//--------------------
	//QAD_RTC Data Methods

//QAD_RTC::imp_getHour
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getHour(void) {
  return m_sTime.Hours;
}


//QAD_RTC::imp_getMinute
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getMinute(void) {
  return m_sTime.Minutes;
}


//QAD_RTC::imp_getSecond
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getSecond(void) {
  return m_sTime.Seconds;
}


//QAD_RTC::imp_getWeekDay
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getWeekday(void) {
  return m_sDate.WeekDay;
}


//QAD_RTC::imp_getWeekdayDayStr
//QAD_RTC Data Method
void QAD_RTC::imp_getWeekdayStr(char* strWeekday) {
	switch (m_sDate.WeekDay) {
		case (1):
			strcpy(strWeekday, "Monday");
			break;
		case (2):
			strcpy(strWeekday, "Tuesday");
			break;
		case (3):
			strcpy(strWeekday, "Wednesday");
			break;
		case (4):
			strcpy(strWeekday, "Thursday");
			break;
		case (5):
			strcpy(strWeekday, "Friday");
			break;
		case (6):
			strcpy(strWeekday, "Saturday");
			break;
		case (7):
			strcpy(strWeekday, "Sunday");
			break;
	}
}


//QAD_RTC::imp_getDay
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getDay(void) {
  return m_sDate.Date;
}


//QAD_RTC::imp_getMonth
//QAD_RTC Data Method
uint8_t QAD_RTC::imp_getMonth(void) {
  return m_sDate.Month;
}


//QAD_RTC::imp_getMonthStr
//QAD_RTC Data Method
void QAD_RTC::imp_getMonthStr(char* strMonth) {
  switch (m_sDate.Month) {
		case (1):
			strcpy(strMonth, "January");
			break;
		case (2):
			strcpy(strMonth, "February");
			break;
		case (3):
			strcpy(strMonth, "March");
			break;
		case (4):
			strcpy(strMonth, "April");
			break;
		case (5):
			strcpy(strMonth, "May");
			break;
		case (6):
			strcpy(strMonth, "June");
			break;
		case (7):
			strcpy(strMonth, "July");
			break;
		case (8):
			strcpy(strMonth, "August");
			break;
		case (9):
			strcpy(strMonth, "September");
			break;
		case (10):
			strcpy(strMonth, "October");
			break;
		case (11):
			strcpy(strMonth, "November");
			break;
		case (12):
			strcpy(strMonth, "December");
			break;
  }
}


//QAD_RTC::imp_getYear
//QAD_RTC Data Method
uint16_t QAD_RTC::imp_getYear(void) {
  return (m_sDate.Year + 2000);
}




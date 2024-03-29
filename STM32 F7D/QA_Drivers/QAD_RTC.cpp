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
//
//To be called by static method init()
//Used to initialize the RTC peripheral
//Returns QA_OK if the initialization is successful, or QA_Fail if initialization fails
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

	//Update m_sTime and m_sDate with values currently stored in RTC
	imp_update();

	//Return
	return QA_OK;
}


//QAD_RTC::imp_deinit
//QAD_RTC Initialization Method
//
//To be called by static deinit() method
//Used to deinitialize the RTC peripheral
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
//
//To be called by static setTime() method
//Used to set the current time
//uHour   - The current hour. A value between 0-23
//uMinute - The current minute. A value between 0-59
//uSecond - The current second. A value between 0-59
//Returns QA_OK if successful, or QA_Fail if setting of time fails
QA_Result QAD_RTC::imp_setTime(uint8_t uHour, uint8_t uMinute, uint8_t uSecond) {

	//Fill out current time details into m_sTime structure
	m_sTime.Hours              = uHour;
	m_sTime.Minutes            = uMinute;
	m_sTime.Seconds            = uSecond;
	m_sTime.SecondFraction     = 0;                        //Not used in this implementation, so is set to 0
	m_sTime.DayLightSaving     = RTC_DAYLIGHTSAVING_NONE;  //Daylight savings mode not being used
	m_sTime.StoreOperation     = RTC_STOREOPERATION_RESET; //

	//Sets the current time using details stored in m_sTime, with data being stored as binary values
	//Return QA_Fail if not successful
	if (HAL_RTC_SetTime(&m_sHandle, &m_sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


//QAD_RTC::imp_setDate
//QAD_RTC Control Method
//
//To be called by static setDate() method
//uWeekDay - The current day of the week. A value between 1-7
//uDay     - The current day of the month. A value between 1-31
//uMonth   - The current month. A value between 1-12
//uYear    - The last two digits of the current year. A value between 00-99
//Returns QA_OK if successful, or QA_Fail if setting of the date fails
QA_Result QAD_RTC::imp_setDate(uint8_t uWeekDay, uint8_t uDay, uint8_t uMonth, uint8_t uYear) {

	//Fill out current date details into m_sDate structure
	m_sDate.WeekDay    = uWeekDay;
	m_sDate.Date       = uDay;
	m_sDate.Month      = uMonth;
	m_sDate.Year       = uYear;

	//Sets the current date using details stored in m_sDate, with data being stored as binary values
	//Return QA_Fail if not successful
	if (HAL_RTC_SetDate(&m_sHandle, &m_sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Return
	return QA_OK;
}


//QAD_RTC::imp_update
//QAD_RTC Control Method
//
//To be called from static update() method
//Captures the current time and date and stores in m_sTime and m_sDate, so that values can
//be retrieved by the below data methods
//Returns QA_OK if successful, or QA_Fail if unable to get the current time/date
QA_Result QAD_RTC::imp_update(void) {

	//Retrieve current time values from RTC and store in m_sTime structure in binary format
	//Return QA_Fail if not successful
	if (HAL_RTC_GetTime(&m_sHandle, &m_sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return QA_Fail;
	}

	//Retrieve current date values from RTC and store in m_sDate structure in binary format
	//Return QA_Fail if not successful
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
//
//To be called by static getHour() method
//Used to retrieve the hour value from m_sTime
//Returns a value between 0-23
uint8_t QAD_RTC::imp_getHour(void) {
  return m_sTime.Hours;
}


//QAD_RTC::imp_getMinute
//QAD_RTC Data Method
//
//To be called by static getMinute() method
//Used to retrieve the minute value from m_sTime
//Returns a value between 0-59
uint8_t QAD_RTC::imp_getMinute(void) {
  return m_sTime.Minutes;
}


//QAD_RTC::imp_getSecond
//QAD_RTC Data Method
//
//To be called by static getSecond() method
//Used to retrieve the second value from m_sTime
//Returns a value between 0-59
uint8_t QAD_RTC::imp_getSecond(void) {
  return m_sTime.Seconds;
}


//QAD_RTC::imp_getWeekDay
//QAD_RTC Data Method
//
//To be called by static getWeekday() method
//Used to retrieve the current day-of-the-week value from m_sDate
//Returns a value between 1-7
uint8_t QAD_RTC::imp_getWeekday(void) {
  return m_sDate.WeekDay;
}


//QAD_RTC::imp_getWeekdayDayStr
//QAD_RTC Data Method
//
//To be called by the static getWeekdatStr() method
//strWeekday - a c-style string to copy the name of the day into. Must be able to fit at
//             least 10 characters
void QAD_RTC::imp_getWeekdayStr(char* strWeekday) {

	//Return if strWeekday points to NULL
	if (!strWeekday) {
		return;
	}

	//Copy the current name of the day-of-the-week, depending on current value in m_sDate
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
//
//To be called by static getDay() method
//Used to retrieve the current day-of-the-month value from m_sDate
//Returns a value between 1-31
uint8_t QAD_RTC::imp_getDay(void) {
  return m_sDate.Date;
}


//QAD_RTC::imp_getMonth
//QAD_RTC Data Method
//
//To be called by static getMonth() method
//Used to retrieve the current month value from m_sDate, as a string containing
//the name of the month.
//Returns a value between 1-12
uint8_t QAD_RTC::imp_getMonth(void) {
  return m_sDate.Month;
}


//QAD_RTC::imp_getMonthStr
//QAD_RTC Data Method
//
//To be called by static getMonthStr() method
//Used to retrieve the current month value from m_sDate, as a string containing
//the name of the month.
//strMonth - a c-style string to copy the name of the current month into. Must be able to
//           fit at least 10 characters
void QAD_RTC::imp_getMonthStr(char* strMonth) {

	//Return if strMonth points to NULL
	if (!strMonth) {
		return;
	}

	//Copy the current name of the month depending on the value in m_sDate
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
//
//To be called by static getYear() method
//Used to retrieve the current year value from m_sYear
//Returns a value between 2000-2099
uint16_t QAD_RTC::imp_getYear(void) {

	//Add 2000 to current two-digit value year value stored in m_sDate,
	//in order to return the full four-digit year value
	//NOTE: Not Y3K compliant :)
  return (m_sDate.Year + 2000);
}




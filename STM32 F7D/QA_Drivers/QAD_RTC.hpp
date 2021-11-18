/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: RTC Driver                                                      */
/*   Filename: QAD_RTC.hpp                                                 */
/*   Date: 18th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_RTC_HPP_
#define __QAD_RTC_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------
//QAD_RTC
class QAD_RTC {
private:

	QA_InitState       m_eInitState;

	RTC_HandleTypeDef  m_sHandle;

	RTC_TimeTypeDef    m_sTime;
	RTC_DateTypeDef    m_sDate;


	//------------
	//Constructors
	QAD_RTC() :
		m_eInitState(QA_NotInitialized),
		m_sHandle({0}) {}

public:

	//Delete copy constructor and assignment operator
	QAD_RTC(const QAD_RTC& other) = delete;
	QAD_RTC& operator=(const QAD_RTC& other) = delete;

	//-----------------
	//Singleton Methods
	static QAD_RTC& get(void) {
		static QAD_RTC instance;
		return instance;
	}


	//----------------------
	//Initialization Methods

	static QA_Result init(void) {
		return get().imp_init();
	}

	static void deinit(void) {
		get().imp_deinit();
	}


	//---------------
	//Control Methods

	static QA_Result setTime(uint8_t uHour, uint8_t uMinute, uint8_t uSecond) {
		return get().imp_setTime(uHour, uMinute, uSecond);
	}

	static QA_Result setDate(uint8_t uWeekDay, uint8_t uDay, uint8_t uMonth, uint8_t uYear) {
		return get().imp_setDate(uWeekDay, uDay, uMonth, uYear);
	}

	static QA_Result update(void) {
		return get().imp_update();
	}


	//------------
	//Data Methods

	static uint8_t getHour(void) {
		return get().imp_getHour();
	}

	static uint8_t getMinute(void) {
		return get().imp_getMinute();
	}

	static uint8_t getSecond(void) {
		return get().imp_getSecond();
	}

	static uint8_t getWeekday(void) {
		return get().imp_getWeekday();
	}

	static void getWeekDayStr(char* strWeekday) {
		get().imp_getWeekdayStr(strWeekday);
	}

	static uint8_t getDay(void) {
		return get().imp_getDay();
	}

	static uint8_t getMonth(void) {
		return get().imp_getMonth();
	}

	static void getMonthStr(char* strMonth) {
		get().imp_getMonthStr(strMonth);
	}

	static uint16_t getYear(void) {
		return get().imp_getYear();
	}


private:


	//----------------------
	//Initialization Methods

	QA_Result imp_init(void);
	void imp_deinit(void);


	//---------------
	//Control Methods

	QA_Result imp_setTime(uint8_t uHour, uint8_t uMinute, uint8_t uSecond);
	QA_Result imp_setDate(uint8_t uWeekDay, uint8_t uDay, uint8_t uMonth, uint8_t uYear);
	QA_Result imp_update(void);


	//------------
	//Data Methods
	uint8_t imp_getHour(void);
	uint8_t imp_getMinute(void);
	uint8_t imp_getSecond(void);

	uint8_t imp_getWeekday(void);
	void imp_getWeekdayStr(char* strWeekday);
	uint8_t imp_getDay(void);
	uint8_t imp_getMonth(void);
	void imp_getMonthStr(char* strMonth);
	uint16_t imp_getYear(void);


};


//Prevent Recursive Inclusion
#endif /* __QAD_RTC_HPP_ */














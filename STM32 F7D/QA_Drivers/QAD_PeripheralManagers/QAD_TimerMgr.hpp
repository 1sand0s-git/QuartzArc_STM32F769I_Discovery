/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Timer Management Driver                                         */
/*   Filename: QAD_TimerMgr.hpp                                            */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_TIMERMGR_HPP_
#define __QAD_TIMERMGR_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//----------------
//QAD_Timer_Periph
//
//Used to select which Timer peripheral is to be used, and index into peripheral array in Timer Manager
enum QAD_Timer_Periph : uint8_t {
	QAD_Timer1 = 0,
	QAD_Timer2,
	QAD_Timer3,
	QAD_Timer4,
	QAD_Timer5,
	QAD_Timer6,
	QAD_Timer7,
	QAD_Timer8,
	QAD_Timer9,
	QAD_Timer10,
	QAD_Timer11,
	QAD_Timer12,
	QAD_Timer13,
	QAD_Timer14,
	QAD_TimerNone
};


//---------------------
//QAD_Timer_PeriphCount
//
//Timer Peripheral Count
const uint8_t QAD_Timer_PeriphCount = QAD_TimerNone;


//---------------
//QAD_Timer_State
//
//Used to store whether a particular Timer peripheral is in use or not, and what purpose it is being used for
enum QAD_Timer_State : uint8_t {
	QAD_Timer_Unused = 0,
	QAD_Timer_InUse_IRQ,
	QAD_Timer_InUse_Encoder,
	QAD_Timer_InUse_PWM,
	QAD_Timer_InUse_ADC
};


//--------------
//QAD_Timer_Type
//
//Used to store whether a particular Timer peripheral has a 16bit or 32bit counter
enum QAD_Timer_Type : uint8_t {QAD_Timer_16bit = 0, QAD_Timer_32bit};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//--------------
//QAD_Timer_Data
//
//Structure used in array within QAD_TimerMgr class to hold information for Timer peripherals
typedef struct {

	QAD_Timer_Periph  eTimer;        //Used to store which Timer peripheral is represented by the structure, used for manager methods that find unused Timers

	QAD_Timer_State   eState;        //Stores whether the Timer peripheral is currently in use, and what purpose it is being used for

	uint32_t          uClockSpeed;   //Stores the input clock speed for the Timer peripheral
	QAD_Timer_Type    eType;         //Stores whether the particular timer has a 16bit or 32bit counter
	uint8_t           uChannels;     //Stores the number of channels supported by the particular Timer peripheral

	bool              bEncoder;      //Stores whether the Timer peripheral has support for rotary encoder mode
	bool              bADC;          //Stores whether the Timer peripheral has support for triggering ADC conversions

	TIM_TypeDef*      pInstance;     //Stores the TIM_TypeDef for the Timer peripheral (defined in stm32f769xx.h)

	IRQn_Type         eIRQ_Update;   //Stores the IRQ Handler enum for the Timer peripheral (defined in stm32f769xx.h)

} QAD_Timer_Data;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//------------
//QAD_TimerMgr
//
//Singleton class
//Used to allow management of Timer peripherals in order to make sure that a driver is prevented from accessing any
//Timer peripherals that are already being used by another driver
class QAD_TimerMgr {
private:

	//Timer Data
	QAD_Timer_Data  m_sTimers[QAD_Timer_PeriphCount];

	//------------
	//Constructors
	QAD_TimerMgr();

public:

	//------------------------------------------------------------------------------
	//Delete copy constructor and assignment operator due to being a singleton class
	QAD_TimerMgr(const QAD_TimerMgr& other) = delete;
	QAD_TimerMgr& operator=(const QAD_TimerMgr& other) = delete;


	//-----------------
	//Singleton Methods
	//
	//Used to retrieve a reference to the singleton class
	static QAD_TimerMgr& get(void) {
		static QAD_TimerMgr instance;
		return instance;
	}


	//------------
  //Data Methods

	//Used to retrieve the current state of a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the state for. Member of QAD_Timer_Periph
	//Returns member of QAD_TimerState enum
	static QAD_Timer_State getState(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eState;
	}

	//Used to retrieve the input clock speed of a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the state for. Member of QAD_Timer_Periph
	//Returns the frequency in Hz of the Timer's input clock
	static uint32_t getClockSpeed(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].uClockSpeed;
	}

	//Used to retrieve the counter type of a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the state for. Member of QAD_Timer_Periph
	//Returns a member of QAD_Timer_Type (QAD_Timer_16bit or QAD_Timer_32bit)
	static QAD_Timer_Type getType(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eType;
	}

	//Used to retrieve the number of channels supported by a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the state for. Member of QAD_Timer_Periph
	//Returns the number of channels supported by the Timer peripheral
	static uint8_t getChannels(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].uChannels;
	}

	//Used to retrieve whether a particular Timer peripheral has rotary encoder mode support
	//eTimer - The Timer peripheral to retrieve the rotary encoder mode support for. Member of QAD_Timer_Periph
	//Returns true if the timer has rotary encoder mode support, or false if not supported
	static bool getEncoder(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].bEncoder;
	}

	//Used to retrieve whether a particular Tiemr peripheral has triggering support for ADC conversions
	//eTimer - The Timer peripheral to retrieve the ADC support for. Member of QAD_Timer_Periph
	//Returns true if the timer has ADC conversion triggering support, or false if not supported
	static bool getADC(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].bADC;
	}

	//Used to retrieve an instance for a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the instance for. Member of QAD_Timer_Periph
	//Returns TIM_TypeDef, as defined in stm32f769xx.h
	static TIM_TypeDef* getInstance(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].pInstance;
	}

	//Used to retrieve an Update IRQ enum for a Timer peripheral
	//eTimer - The Timer peripheral to retrieve the IRQ enum for. Member of QAD_Timer_Periph
	//Returns member of IRQn_Type enum, as defined in stm32f769xx.h
	static IRQn_Type getUpdateIRQ(QAD_Timer_Periph eTimer) {
		return get().m_sTimers[eTimer].eIRQ_Update;
	}


	//------------------
	//Management Methods

	//Used to register a Timer peripheral as being used by a driver
	//eTimer - The Timer peripheral to be registered. Member of QAD_Timer_Periph
	//Returns QA_OK if registration is successful, or returns QA_Error_PeriphBusy if the selected Timer is already in use
	static QA_Result registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState) {
		return get().imp_registerTimer(eTimer, eState);
	}

	//Used to deregister a Timer peripheral to mark it as no longer being used by a driver
	//eTimer - The Timer peripheral to be registered. Member of QAD_Timer_Periph
	static void deregisterTimer(QAD_Timer_Periph eTimer) {
		get().imp_deregisterTimer(eTimer);
	}

	//Used to find an available timer with the selected counter type (16bit or 32bit)
	//If a 16bit counter type is selected, a 32bit timer can be returned due to 32bit timers having 16bit support
	//eType - A member of QAD_Timer_Type to select if a 16bit or 32bit counter is required
	//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
	static QAD_Timer_Periph findTimer(QAD_Timer_Type eType) {
		return get().imp_findTimer(eType);
	}

	//Used to find an available timer that has rotary encoder support
	//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
	static QAD_Timer_Periph findTimerEncoder(void) {
		return get().imp_findTimerEncoder();
	}

	//Used to find an available timer that has ADC triggering support
	//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
	static QAD_Timer_Periph findTimerADC(void) {
		return get().imp_findTimerADC();
	}


	//-------------
	//Clock Methods

	//Used to enable the clock for a specific Timer peripheral
	//eTimer - The Timer peripheral to enable the clock for
	static void enableClock(QAD_Timer_Periph eTimer) {
		get().imp_enableClock(eTimer);
	}

	//Used to disable the clock for a specific Timer peripheral
	//eTimer - The Timer peripheral to disable the clock for
	static void disableClock(QAD_Timer_Periph eTimer) {
		get().imp_disableClock(eTimer);
	}


	//--------------
	//Status Methods

	//Returns the number of Timer peripherals that are currently in-use (registered/active)
	static uint8_t getTimersActive(void) {
		return get().imp_getTimersActive();
	}

	//Returns the number of Timer peripherals that are currently not being used (deregistered/active)
	static uint8_t getTimersInactive(void) {
		return get().imp_getTimersInactive();
	}


private:

	//NOTE: See QAD_TimerMgr.cpp for details of the following methods

	//------------------
	//Management Methods

  QA_Result imp_registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState);
  void imp_deregisterTimer(QAD_Timer_Periph eTimer);

  QAD_Timer_Periph imp_findTimer(QAD_Timer_Type eType);
  QAD_Timer_Periph imp_findTimerEncoder(void);
  QAD_Timer_Periph imp_findTimerADC(void);


  //-------------
  //Clock Methods

  void imp_enableClock(QAD_Timer_Periph eTimer);
  void imp_disableClock(QAD_Timer_Periph eTimer);


  //--------------
  //Status Methods

  uint8_t imp_getTimersActive(void);
  uint8_t imp_getTimersInactive(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_TIMERMGR_HPP_ */

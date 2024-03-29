/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Timer Driver                                                    */
/*   Filename: QAD_Timer.hpp                                               */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_TIMER_HPP_
#define __QAD_TIMER_HPP_


//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------------
//QAD_TimerMode
//
//Used with QAD_Timer driver class to determine if update interrupt is to be triggered continuously, a set number of times of a single time.
enum QAD_TimerMode : uint8_t {
	QAD_TimerContinuous = 0,  //Update interrupt to be triggered continuously
	QAD_TimerMultiple,        //Update interrupt to be triggered a set number of times (based on counter target value)
	QAD_TimerSingle           //Update interrupt to be triggered a single time
};


//--------------------
//QAD_Timer_InitStruct
//
//This structure is used to be able to create the QAD_Timer driver class
typedef struct {

	QAD_Timer_Periph eTimer;           //Timer peripheral to be used. Member of QAD_Timer_Periph as defined in QAD_TimerMgr.hpp

	QAD_TimerMode    eMode;            //Update interrupt mode. Member of QAD_TimerMode

	uint32_t         uPrescaler;       //Prescaler to be used for selected timer
	uint32_t         uPeriod;          //Counter period to be used for selected timer

	uint8_t          uIRQPriority;     //IRQ Priority for update interrupt (a value between 0 and 15)

	uint16_t         uCounterTarget;   //Counter target to be used when eMode is set to QAD_TimerMultiple

} QAD_Timer_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//---------
//QAD_Timer
//
//Driver class used for triggering timer update interrupts at regular intervals
class QAD_Timer {
private:

	//Deinitialization mode to be used by periphDeinit() method
	enum DeinitMode : uint8_t {
		DeinitPartial = 0,        //Only to be used for partial deinitialization upon initialization failure in periphInit() method
		DeinitFull                //Used for full driver deinitialization when driver is in a fully initialized state
  };

	QAD_Timer_Periph  m_eTimer;        //Stores the particular timer peripheral to be used by the driver
	                                   //Member of QAD_Timer_Periph as defined in QAD_TimerMgr.hpp

	TIM_HandleTypeDef m_sHandle;       //Handle used by HAL functions to access Timer peripheral (defined in stm32f7xx_hal_tim.h)

	QAD_TimerMode     m_eMode;         //Member of QAD_TimerMode to determine update interrupt mode (see QAD_TimerMode definition for details)

	uint32_t          m_uPrescaler;    //Prescaler to be used for selected timer
	uint32_t          m_uPeriod;       //Counter period to be used for selected timer

	uint8_t           m_uIRQPriority;  //IRQ Priority for update interrupt (a value between 0 and 15)

	QA_InitState      m_eInitState;    //Stores whether the driver is currently initialized. Member of QA_InitState enum defined in setup.hpp

	QA_ActiveState    m_eState;        //Stores whether the driver is currently active. Member of QA_ActiveState enum defined in setup.hpp

	IRQn_Type         m_eIRQ;          //The IRQ used by the Timer peripheral being used (a member of IRQn_Type defined in stm32f769xx.h)

	QAD_IRQHandler_CallbackFunction m_pHandlerFunction;  //A pointer to the callback function to be called when update interrupt triggered
	                                                     //QAD_IRQHandler_CallbackFunction defined in setup.hpp

	QAD_IRQHandler_CallbackClass*   m_pHandlerClass;     //A pointer to the callback class to be called when update interrupt is triggered
	                                                     //The handler() pure virtual function is the method that is called, and is to be implemented by the parent class
	                                                     //QAD_IRQHandler_CallbackClass defined in setup.hpp

	uint16_t          m_uIRQCounterTarget;  //Counter target value to be used when m_eMode is set to QAD_TimerMultiple
	uint16_t          m_uIRQCounterValue;   //Current counter value to be used when m_eMode is set to QAD_TimerMultiple

public:

	//--------------------------
	//Constructors / Destructors

	QAD_Timer() = delete;                     //Delete the default class constructor, as we need an initialization structure to be provided on class creation

	QAD_Timer(QAD_Timer_InitStruct& sInit) :  //The class constructor to be used, which has a reference to an initialization structure passed to it
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_eMode(sInit.eMode),
		m_uPrescaler(sInit.uPrescaler),
		m_uPeriod(sInit.uPeriod),
		m_uIRQPriority(sInit.uIRQPriority),
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_pHandlerFunction(NULL),
		m_pHandlerClass(NULL),
		m_uIRQCounterTarget(sInit.uCounterTarget),
		m_uIRQCounterValue(0) {}

	~QAD_Timer() {         //Destructor to make sure peripheral is made inactive and deinitialized upon class destruction

		//Stop timer driver if currently active
		if (m_eState)
			stop();

		//Deinitialize timer driver if currently initialized
		if (m_eInitState)
			deinit();
	}


	//NOTE: See QAD_Timer.cpp for details of the following functions

	//----------------------
	//Initialization Methods

	QA_Result init(void);
	void deinit(void);


	//-------------------
	//IRQ Handler Methods

	void handler(void);


	//---------------
	//Control Methods

  void setHandlerFunction(QAD_IRQHandler_CallbackFunction pHandler);
  void setHandlerClass(QAD_IRQHandler_CallbackClass* pHandler);

  void setTimerMode(QAD_TimerMode eMode);
  QAD_TimerMode getTimerMode(void);

  void setIRQCounterTarget(uint16_t uTarget);
  uint16_t getIRQCounterTarget(void);

  uint16_t getIRQCounterValue(void);
  void clearIRQCounterValue(void);

  void start(void);
  void stop(void);

private:

  //------------------------------
  //Private Initialization Methods

  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eDeinitMode);

};


//Prevent Recursive Inclusion
#endif /* __QAD_TIMER_HPP */

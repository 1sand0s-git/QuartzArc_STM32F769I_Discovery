/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Timer Driver                                                    */
/*   Filename: QAD_Timer.cpp                                               */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_Timer.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //--------------------------------
  //--------------------------------
  //QAD_Timer Initialization Methods

//QAD_Timer::init
//QAD_Timer Initialization Method
//
//Used to initialize the timer driver
//Returns QA_OK if initialization successful, or an error if not successful (a member of QA_Result as defined in setup.hpp)
QA_Result QAD_Timer::init(void) {

	//Check if selected Timer peripheral is currently available
  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  //Register Timer peripheral as now being in use
  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_IRQ);

  //Initialize Timer peripheral
  QA_Result eRes = periphInit();

  //If initialization failed then deregister Timer peripheral
  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);

  //Return initialization result
  return eRes;
}


//QAD_Timer::deinit
//QAD_Timer Initialization Method
//
//Used to deinitialize the Timer driver
void QAD_Timer::deinit(void) {

	//Return if Timer driver is not currently initialized
  if (!m_eInitState)
  	return;

  //Deinitialize Timer driver
  periphDeinit(DeinitFull);

  //Deregister Timer peripheral
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //-----------------------------
  //-----------------------------
  //QAD_Timer IRQ Handler Methods

//QAD_Timer::handler
//QAD_Timer IRQ Handler Method
//
//This method is only to be called by the interrupt request handler function from handlers.cpp
void QAD_Timer::handler(void) {

	//Check if Update Interrupt has been triggered
  if (__HAL_TIM_GET_FLAG(&m_sHandle, TIM_FLAG_UPDATE)) {

  	//Process based on currently selected Timer Mode
  	switch (m_eMode) {
  	  case (QAD_TimerContinuous):  //If is in continuous mode then do nothing
  	  	break;
  	  case (QAD_TimerMultiple):    //If is in multiple mode then increment counter value, and if counter target has been reached then disable driver
  	  	m_uIRQCounterValue++;
  	    if (m_uIRQCounterValue >= m_uIRQCounterTarget) {
  	    	stop();
  	    }
  	    break;
  	  case (QAD_TimerSingle):      //If is in single mode then disable driver
  	  	stop();
  	    break;
  	}

  	//If a handler callback function has been assigned then call it
  	if (m_pHandlerFunction)
  		m_pHandlerFunction(NULL);

  	//If a handler callback class has been assigned then call it's handler() method
  	if (m_pHandlerClass)
  		m_pHandlerClass->handler(NULL);

  	//Clear Update Interrupt flag
  	__HAL_TIM_CLEAR_FLAG(&m_sHandle, TIM_FLAG_UPDATE);
  }
}


  //-------------------------
  //-------------------------
  //QAD_Timer Control Methods

//QAD_Timer::setHandlerFunction
//QAD_Timer Control Method
//
//Used to set the interrupt handler callback function to be called when the timer update interrupt is triggered
//pHandler - Pointer to callback function based on QAD_IRQHandler_CallbackFunction prototype defined in setup.hpp
void QAD_Timer::setHandlerFunction(QAD_IRQHandler_CallbackFunction pHandler) {
  m_pHandlerFunction = pHandler;
}


//QAD_Timer::setHandlerClass
//QAD_Timer Control Method
//
//Used to set the interrupt handler callback class to be called when the timer update interrupt is triggered
//pHandler - Pointer to callback class based on QAD_IRQHandler_CallbackClass defined in setup.hpp
void QAD_Timer::setHandlerClass(QAD_IRQHandler_CallbackClass* pHandler) {
  m_pHandlerClass = pHandler;
}


//QAD_Timer::setTimerMode
//QAD_Timer Control Method
//
//Sets the current update interrupt mode
//eMode - The current mode to be used. See QAD_TimerMode enum definition in QAD_Timer.hpp for details
void QAD_Timer::setTimerMode(QAD_TimerMode eMode) {
  m_eMode = eMode;
}


//QAD_Timer::getTimerMode
//QAD_Timer Control Method
//
//Returns the current update interrupt mode. See QAD_TimerMode enum definition in QAD_Timer.hpp for details
QAD_TimerMode QAD_Timer::getTimerMode(void) {
  return m_eMode;
}


//QAD_Timer::setIRQCounterTarget
//QAD_Timer Control Method
//
//Sets the current target of update interrupts to be triggered when m_eMode is set to QAD_TimerMultiple
//uTarget - The number of update interrupts to be triggered
void QAD_Timer::setIRQCounterTarget(uint16_t uTarget) {
  m_uIRQCounterTarget = uTarget;
}


//QAD_Timer::getIRQCounterTarget
//QAD_Timer Control Method
//
//Returns the current target number of update interrupts to be triggered when m_eMode is set to QAD_TimerMultiple
uint16_t QAD_Timer::getIRQCounterTarget(void) {
  return m_uIRQCounterTarget;
}


//QAD_Timer::getIRQCounterValue
//QAD_Timer Control Method
//
//Returns the current number of update interrupts that have been triggered when m_eMode is set to QAD_TimerMultiple
uint16_t QAD_Timer::getIRQCounterValue(void) {
  return m_uIRQCounterValue;
}


//QAD_Timer::clearIRQCounterValue
//QAD_Timer Control Method
//
//Used to reset the current count of update interrupts that have been triggered when m_eMode is set to QAD_TimerMultiple
void QAD_Timer::clearIRQCounterValue(void) {
  m_uIRQCounterValue = 0;
}


//QAD_Timer::start
//QAD_Timer Control Method
//
//Starts the timer driver
void QAD_Timer::start(void) {

	//Check if driver is initialized and is currently not active
  if ((m_eInitState) && (!m_eState)) {

  	//Reset counter value
  	m_uIRQCounterValue = 0;

  	//Enable Timer Update interrupt
  	__HAL_TIM_ENABLE_IT(&m_sHandle, TIM_IT_UPDATE);

  	//Enable Timer peripheral
  	__HAL_TIM_ENABLE(&m_sHandle);

  	//Set current driver state to active
  	m_eState = QA_Active;
  }
}


//QAD_Timer::stop
//QAD_Timer Control Method
//
//Stops the timer driver
void QAD_Timer::stop(void) {

	//Check if driver is initialized and is currently active
  if ((m_eInitState) && (m_eState)) {

  	//Disable the timer peripheral
  	__HAL_TIM_DISABLE(&m_sHandle);

  	//Disabled the timer update interrupt
  	__HAL_TIM_DISABLE_IT(&m_sHandle, TIM_IT_UPDATE);

  	//Set current driver state in inactive
  	m_eState = QA_Inactive;
  }
}


  //----------------------------------------
  //----------------------------------------
  //QAD_Timer Private Initialization Methods

//QAD_Timer::periphInit
//QAD_Timer Private Initialization Method
//
//Used to initialize the timer peripheral clock, and the timer peripheral itself as well as enabling timer interrupt and setting interrupt priority
//In the case of a failed initialization, a partial deinitialization will be performed to make sure the peripheral and clock are all in the
//uninitialized state
//Returns QA_OK if successful, or QA_Fail if initialization fails
QA_Result QAD_Timer::periphInit(void) {

	//Enable Timer Clock
	QAD_TimerMgr::enableClock(m_eTimer);

	//Initialize Timer Peripheral
	m_sHandle.Instance               = QAD_TimerMgr::getInstance(m_eTimer); //Set instance for required Timer peripheral
	m_sHandle.Init.Prescaler         = m_uPrescaler;                        //Set timer prescaler
	m_sHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;                  //Set timer counter mode to count up
	m_sHandle.Init.Period            = m_uPeriod;                           //Set timer counter period
	m_sHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;              //Unused
	m_sHandle.Init.RepetitionCounter = 0x0;                                 //
	m_sHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;       //Enable preload of the timer's auto-reload register

	//Initialize Timer, performing a partial deinitialization if the initialization fails
	if (HAL_TIM_Base_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Set Timer IRQ priority and enable IRQ
	m_eIRQ = QAD_TimerMgr::getUpdateIRQ(m_eTimer);
	HAL_NVIC_SetPriority(m_eIRQ, m_uIRQPriority, 0);
	HAL_NVIC_EnableIRQ(m_eIRQ);

	//Set driver states
	m_eState     = QA_Inactive;    //Set driver state as initialized
	m_eInitState = QA_Initialized; //Set driver as currently inactive

	//Return
	return QA_OK;
}


//QAD_Timer::periphDeinit
//QAD_Timer Private Initialization Method
//
//Used to deinitialize timer peripheral clock and the peripheral itself, as well as disabling the interrupt
//eDeinitMode - Set to DeinitPartial to perform a partial deinitialization (only to be used by periphInit() method
//              in a case where peripheral initialization has failed
//            - Set to DeinitFull to perform a full deinitialization in a case where the driver is fully initialized
void QAD_Timer::periphDeinit(QAD_Timer::DeinitMode eDeinitMode) {

	//Check if full deinitialization is required
	if (eDeinitMode) {

		//Disable timer IRQ
		HAL_NVIC_DisableIRQ(m_eIRQ);

		//Deinitialize Timer peripheral
		HAL_TIM_Base_DeInit(&m_sHandle);
	}

	//Disable Timer Clock
	QAD_TimerMgr::disableClock(m_eTimer);

	//Set States
	m_eState     = QA_Inactive;       //Set driver as currently inactive
	m_eInitState = QA_NotInitialized; //Set driver state as not initialized
}


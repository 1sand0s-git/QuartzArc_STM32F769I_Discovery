/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Timer Management Driver                                         */
/*   Filename: QAD_TimerMgr.cpp                                            */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //-------------------------
  //-------------------------
	//QAD_TimerMgr Constructors

//QAD_TimerMgr::QAD_TimerMgr
//QAD_TimerMgr Constructor
//
//Fills out details for the system's Tiemr peripherals
//As this is a private method in a singleton class, this method will be called the first time the class's get() method is called
QAD_TimerMgr::QAD_TimerMgr() {

  for (uint8_t i=0; i < QAD_Timer_PeriphCount; i++) {
  	m_sTimers[i].eState   = QAD_Timer_Unused;
  	m_sTimers[i].bEncoder = (i <= QAD_Timer5) || (i == QAD_Timer8);
  }

  //Set Timer Periph ID
  m_sTimers[QAD_Timer1].eTimer  = QAD_Timer1;
  m_sTimers[QAD_Timer2].eTimer  = QAD_Timer2;
  m_sTimers[QAD_Timer3].eTimer  = QAD_Timer3;
  m_sTimers[QAD_Timer4].eTimer  = QAD_Timer4;
  m_sTimers[QAD_Timer5].eTimer  = QAD_Timer5;
  m_sTimers[QAD_Timer6].eTimer  = QAD_Timer6;
  m_sTimers[QAD_Timer7].eTimer  = QAD_Timer7;
  m_sTimers[QAD_Timer8].eTimer  = QAD_Timer8;
  m_sTimers[QAD_Timer9].eTimer  = QAD_Timer9;
  m_sTimers[QAD_Timer10].eTimer = QAD_Timer10;
  m_sTimers[QAD_Timer11].eTimer = QAD_Timer11;
  m_sTimers[QAD_Timer12].eTimer = QAD_Timer12;
  m_sTimers[QAD_Timer13].eTimer = QAD_Timer13;
  m_sTimers[QAD_Timer14].eTimer = QAD_Timer14;

	//Set clock speeds
	m_sTimers[QAD_Timer1].uClockSpeed  = HAL_RCC_GetPCLK2Freq() * 2;
	m_sTimers[QAD_Timer2].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer3].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer4].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer5].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer6].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer7].uClockSpeed  = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer8].uClockSpeed  = HAL_RCC_GetPCLK2Freq() * 2;
	m_sTimers[QAD_Timer9].uClockSpeed  = HAL_RCC_GetPCLK2Freq() * 2;
	m_sTimers[QAD_Timer10].uClockSpeed = HAL_RCC_GetPCLK2Freq() * 2;
	m_sTimers[QAD_Timer11].uClockSpeed = HAL_RCC_GetPCLK2Freq() * 2;
	m_sTimers[QAD_Timer12].uClockSpeed = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer13].uClockSpeed = HAL_RCC_GetPCLK1Freq() * 2;
	m_sTimers[QAD_Timer14].uClockSpeed = HAL_RCC_GetPCLK1Freq() * 2;

	//Set types
	m_sTimers[QAD_Timer1].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer2].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer3].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer4].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer5].eType  = QAD_Timer_32bit;
	m_sTimers[QAD_Timer6].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer7].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer8].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer9].eType  = QAD_Timer_16bit;
	m_sTimers[QAD_Timer10].eType = QAD_Timer_16bit;
	m_sTimers[QAD_Timer11].eType = QAD_Timer_16bit;
	m_sTimers[QAD_Timer12].eType = QAD_Timer_16bit;
	m_sTimers[QAD_Timer13].eType = QAD_Timer_16bit;
	m_sTimers[QAD_Timer14].eType = QAD_Timer_16bit;

	//Set channel counts
	m_sTimers[QAD_Timer1].uChannels  = 4;
	m_sTimers[QAD_Timer2].uChannels  = 4;
	m_sTimers[QAD_Timer3].uChannels  = 4;
	m_sTimers[QAD_Timer4].uChannels  = 4;
	m_sTimers[QAD_Timer5].uChannels  = 4;
	m_sTimers[QAD_Timer6].uChannels  = 0;
	m_sTimers[QAD_Timer7].uChannels  = 0;
	m_sTimers[QAD_Timer8].uChannels  = 4;
	m_sTimers[QAD_Timer9].uChannels  = 2;
	m_sTimers[QAD_Timer10].uChannels = 1;
	m_sTimers[QAD_Timer11].uChannels = 1;
	m_sTimers[QAD_Timer12].uChannels = 2;
	m_sTimers[QAD_Timer13].uChannels = 1;
	m_sTimers[QAD_Timer14].uChannels = 1;

	//Set Instances
	m_sTimers[QAD_Timer1].pInstance  = TIM1;
	m_sTimers[QAD_Timer2].pInstance  = TIM2;
	m_sTimers[QAD_Timer3].pInstance  = TIM3;
	m_sTimers[QAD_Timer4].pInstance  = TIM4;
	m_sTimers[QAD_Timer5].pInstance  = TIM5;
	m_sTimers[QAD_Timer6].pInstance  = TIM6;
	m_sTimers[QAD_Timer7].pInstance  = TIM7;
	m_sTimers[QAD_Timer8].pInstance  = TIM8;
	m_sTimers[QAD_Timer9].pInstance  = TIM9;
	m_sTimers[QAD_Timer10].pInstance = TIM10;
	m_sTimers[QAD_Timer11].pInstance = TIM11;
	m_sTimers[QAD_Timer12].pInstance = TIM12;
	m_sTimers[QAD_Timer13].pInstance = TIM13;
	m_sTimers[QAD_Timer14].pInstance = TIM14;

	//Set Update IRQs
	m_sTimers[QAD_Timer1].eIRQ_Update  = TIM1_UP_TIM10_IRQn;
	m_sTimers[QAD_Timer2].eIRQ_Update  = TIM2_IRQn;
	m_sTimers[QAD_Timer3].eIRQ_Update  = TIM3_IRQn;
	m_sTimers[QAD_Timer4].eIRQ_Update  = TIM4_IRQn;
	m_sTimers[QAD_Timer5].eIRQ_Update  = TIM5_IRQn;
	m_sTimers[QAD_Timer6].eIRQ_Update  = TIM6_DAC_IRQn;
	m_sTimers[QAD_Timer7].eIRQ_Update  = TIM7_IRQn;
	m_sTimers[QAD_Timer8].eIRQ_Update  = TIM8_UP_TIM13_IRQn;
	m_sTimers[QAD_Timer9].eIRQ_Update  = TIM1_BRK_TIM9_IRQn;
	m_sTimers[QAD_Timer10].eIRQ_Update = TIM1_UP_TIM10_IRQn;
	m_sTimers[QAD_Timer11].eIRQ_Update = TIM1_TRG_COM_TIM11_IRQn;
	m_sTimers[QAD_Timer12].eIRQ_Update = TIM8_BRK_TIM12_IRQn;
	m_sTimers[QAD_Timer13].eIRQ_Update = TIM8_UP_TIM13_IRQn;
	m_sTimers[QAD_Timer14].eIRQ_Update = TIM8_TRG_COM_TIM14_IRQn;

	//Set ADCs
	m_sTimers[QAD_Timer1].bADC  = true;
	m_sTimers[QAD_Timer2].bADC  = true;
	m_sTimers[QAD_Timer3].bADC  = false;
	m_sTimers[QAD_Timer4].bADC  = true;
	m_sTimers[QAD_Timer5].bADC  = true;
	m_sTimers[QAD_Timer6].bADC  = true;
	m_sTimers[QAD_Timer7].bADC  = false;
	m_sTimers[QAD_Timer8].bADC  = true;
	m_sTimers[QAD_Timer9].bADC  = false;
	m_sTimers[QAD_Timer10].bADC = false;
	m_sTimers[QAD_Timer11].bADC = false;
	m_sTimers[QAD_Timer12].bADC = false;
	m_sTimers[QAD_Timer13].bADC = false;
	m_sTimers[QAD_Timer14].bADC = false;

}


  //-------------------------------
  //-------------------------------
  //QAD_TimerMgr Management Methods

//QAD_TimerMgr::imp_registerTimer
//QAD_TimerMgr Management Method
//
//To be called from static method registerTimer()
//Used to register a Timer peripheral as being used by a driver
//eTimer - The timer peripheral to be registered. A member of QAD_Timer_Periph
//eState - The purpose the timer is to be used for. A member of QAD_Timer_State
//         QAD_Timer_InUse_IRQ     - Specifies timer as being used to trigger regular update IRQ
//         QAD_Timer_InUse_Encoder - Specifies timer as being used in rotary encoder mode
//         QAD_Timer_InUse_PWM     - Specifies timer as being used to generate PWM signals
//         QAD_Timer_InUse_ADC     - Specifies timer as being used to trigger ADC conversions
//Returns QA_OK if registration is successful.
//        QA_Fail if eState is set to QAD_Timer_Unused.
//        QA_Error_PeriphBusy if selected Timer is already in use
QA_Result QAD_TimerMgr::imp_registerTimer(QAD_Timer_Periph eTimer, QAD_Timer_State eState) {
  if (m_sTimers[eTimer].eState)
  	return QA_Error_PeriphBusy;

  if (!eState)
  	return QA_Fail;

  m_sTimers[eTimer].eState = eState;
  return QA_OK;
}


//QAD_TimerMgr::imp_deregisterTimer
//QAD_TimerMgr Management Method
//
//To be called from static method registerTimer()
//Used to deregister a Timer peripheral to mark it as no longer being used by a driver
//eTimer - The Timer peripheral to be deregistered. A member of QAD_Timer_Periph
void QAD_TimerMgr::imp_deregisterTimer(QAD_Timer_Periph eTimer) {
  m_sTimers[eTimer].eState = QAD_Timer_Unused;
}


//QAD_TimerMgr::imp_findTimer
//QAD_TimerMgr Management Method
//
//To be called from static method findTimer()
//Used to find an available timer with the selected counter type (16bit or 32bit)
//If a 16bit counter type is selected, a 32bit timer can be returned due to 32bit timers having 16bit support
//eType - A member of QAD_Timer_Type to select if a 16bit or 32bit counter is required
//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
QAD_Timer_Periph QAD_TimerMgr::imp_findTimer(QAD_Timer_Type eType) {

	for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
		if ((eType <= m_sTimers[i].eType) && (!m_sTimers[i].eState))
			return m_sTimers[i].eTimer;
	}
	return QAD_TimerNone;
}


//QAD_TimerMgr::imp_findTimerEncoder
//QAD_TimerMgr Management Method
//
//To be called from static method findTimerEncoder()
//Used to find an available timer with rotary encoder support
//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
QAD_Timer_Periph QAD_TimerMgr::imp_findTimerEncoder(void) {

	for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
		if ((!m_sTimers[i].eState) && (m_sTimers[i].bEncoder))
			return m_sTimers[i].eTimer;
	}
	return QAD_TimerNone;
}


//QAD_TimerMgr::imp_findTimerADC
//QAD_TimerMgr Management Method
//
//To be called from static method findTimerADC()
//Used to find an available timer with ADC conversion triggering support
//Returns QAD_TimerNone if no available timer is found, or another member of QAD_Timer_Periph for the available timer that has been found
QAD_Timer_Periph QAD_TimerMgr::imp_findTimerADC(void) {

	for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
		if ((!m_sTimers[i].eState) && (m_sTimers[i].bADC))
			return m_sTimers[i].eTimer;
	}
	return QAD_TimerNone;
}


  //--------------------------
  //--------------------------
  //QAD_TimerMgr Clock Methods


//QAD_TimerMgr::imp_enableClock
//QAD_TimerMgr Clock Method
//
//To be called by enableClock()
//Used to enable the clock for a specific Timer peripheral
//eTimer - the Timer peripheral to enable the clock for
void QAD_TimerMgr::imp_enableClock(QAD_Timer_Periph eTimer) {
  switch (eTimer) {
    case (QAD_Timer1):
    	__HAL_RCC_TIM1_CLK_ENABLE();
      __HAL_RCC_TIM1_FORCE_RESET();
      __HAL_RCC_TIM1_RELEASE_RESET();
      break;
    case (QAD_Timer2):
    	__HAL_RCC_TIM2_CLK_ENABLE();
			__HAL_RCC_TIM2_FORCE_RESET();
			__HAL_RCC_TIM2_RELEASE_RESET();
      break;
    case (QAD_Timer3):
    	__HAL_RCC_TIM3_CLK_ENABLE();
			__HAL_RCC_TIM3_FORCE_RESET();
			__HAL_RCC_TIM3_RELEASE_RESET();
      break;
    case (QAD_Timer4):
    	__HAL_RCC_TIM4_CLK_ENABLE();
			__HAL_RCC_TIM4_FORCE_RESET();
			__HAL_RCC_TIM4_RELEASE_RESET();
      break;
    case (QAD_Timer5):
    	__HAL_RCC_TIM5_CLK_ENABLE();
			__HAL_RCC_TIM5_FORCE_RESET();
			__HAL_RCC_TIM5_RELEASE_RESET();
      break;
    case (QAD_Timer6):
    	__HAL_RCC_TIM6_CLK_ENABLE();
			__HAL_RCC_TIM6_FORCE_RESET();
			__HAL_RCC_TIM6_RELEASE_RESET();
      break;
    case (QAD_Timer7):
    	__HAL_RCC_TIM7_CLK_ENABLE();
			__HAL_RCC_TIM7_FORCE_RESET();
			__HAL_RCC_TIM7_RELEASE_RESET();
      break;
    case (QAD_Timer8):
    	__HAL_RCC_TIM8_CLK_ENABLE();
			__HAL_RCC_TIM8_FORCE_RESET();
			__HAL_RCC_TIM8_RELEASE_RESET();
      break;
    case (QAD_Timer9):
    	__HAL_RCC_TIM9_CLK_ENABLE();
			__HAL_RCC_TIM9_FORCE_RESET();
			__HAL_RCC_TIM9_RELEASE_RESET();
      break;
    case (QAD_Timer10):
    	__HAL_RCC_TIM10_CLK_ENABLE();
			__HAL_RCC_TIM10_FORCE_RESET();
			__HAL_RCC_TIM10_RELEASE_RESET();
      break;
    case (QAD_Timer11):
    	__HAL_RCC_TIM11_CLK_ENABLE();
			__HAL_RCC_TIM11_FORCE_RESET();
			__HAL_RCC_TIM11_RELEASE_RESET();
      break;
    case (QAD_Timer12):
    	__HAL_RCC_TIM12_CLK_ENABLE();
			__HAL_RCC_TIM12_FORCE_RESET();
			__HAL_RCC_TIM12_RELEASE_RESET();
      break;
    case (QAD_Timer13):
    	__HAL_RCC_TIM13_CLK_ENABLE();
			__HAL_RCC_TIM13_FORCE_RESET();
			__HAL_RCC_TIM13_RELEASE_RESET();
      break;
    case (QAD_Timer14):
    	__HAL_RCC_TIM14_CLK_ENABLE();
			__HAL_RCC_TIM14_FORCE_RESET();
			__HAL_RCC_TIM14_RELEASE_RESET();
      break;
    case (QAD_TimerNone):
    	break;
  }
}


//QAD_TimerMgr::imp_disableClock
//QAD_TimerMgr Clock Method
//
//To be called by disableClock()
//Used to disable the clock for a specific Timer peripheral
//eTimer - The timer peripheral to disable the clock for
void QAD_TimerMgr::imp_disableClock(QAD_Timer_Periph eTimer) {
  switch (eTimer) {
    case (QAD_Timer1):
    	__HAL_RCC_TIM1_CLK_DISABLE();
      break;
    case (QAD_Timer2):
    	__HAL_RCC_TIM2_CLK_DISABLE();
      break;
    case (QAD_Timer3):
    	__HAL_RCC_TIM3_CLK_DISABLE();
      break;
    case (QAD_Timer4):
    	__HAL_RCC_TIM4_CLK_DISABLE();
      break;
    case (QAD_Timer5):
    	__HAL_RCC_TIM5_CLK_DISABLE();
      break;
    case (QAD_Timer6):
    	__HAL_RCC_TIM6_CLK_DISABLE();
      break;
    case (QAD_Timer7):
    	__HAL_RCC_TIM7_CLK_DISABLE();
      break;
    case (QAD_Timer8):
    	__HAL_RCC_TIM8_CLK_DISABLE();
      break;
    case (QAD_Timer9):
    	__HAL_RCC_TIM9_CLK_DISABLE();
      break;
    case (QAD_Timer10):
    	__HAL_RCC_TIM10_CLK_DISABLE();
      break;
    case (QAD_Timer11):
    	__HAL_RCC_TIM11_CLK_DISABLE();
      break;
    case (QAD_Timer12):
    	__HAL_RCC_TIM12_CLK_DISABLE();
      break;
    case (QAD_Timer13):
    	__HAL_RCC_TIM13_CLK_DISABLE();
      break;
    case (QAD_Timer14):
    	__HAL_RCC_TIM14_CLK_DISABLE();
      break;
    case (QAD_TimerNone):
    	break;
  }
}


  //---------------------------
  //---------------------------
  //QAD_TimerMgr Status Methods


//QAD_TimerMgr::imp_getTimersActive
//QAD_TimerMgr Status Method
//
//To be called by getTimersActive()
//Returns the number of Timer peripherals that are currently in-use (registered/active)
uint8_t QAD_TimerMgr::imp_getTimersActive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
  	if (m_sTimers[i].eState)
  		uCount++;
  }
  return uCount;
}


//QAD_TimerMgr::getTimersInactive
//QAD_TimerMgr Status Method
//
//To be called by getTimersInactive()
//Returns the number of Timer peripherals that are currently not being used (deregistered/inactive)
uint8_t QAD_TimerMgr::imp_getTimersInactive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_Timer_PeriphCount; i++) {
  	if (!m_sTimers[i].eState)
  		uCount++;
  }
  return uCount;
}












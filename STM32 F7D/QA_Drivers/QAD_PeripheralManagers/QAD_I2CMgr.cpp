/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: I2C Management Driver                                           */
/*   Filename: QAD_I2CMgr.cpp                                              */
/*   Date: 27th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_I2CMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


	//-----------------------
	//-----------------------
	//QAD_I2CMgr Constructors

//QAD_I2CMgr::QAD_I2CMgr
//QAD_I2CMgr Constructor
//
//Fills out details for the system's I2C peripherals
//As this is a private method in a singleton class, this method will be called the first time the class's static get() method is called.
QAD_I2CMgr::QAD_I2CMgr() {

	for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
		//Set current state of peripheral to unused
		m_sI2Cs[i].eState = QAD_I2C_Unused;
	}

	//Set Peripheral IDs
	m_sI2Cs[QAD_I2C1].eI2C = QAD_I2C1;
	m_sI2Cs[QAD_I2C2].eI2C = QAD_I2C2;
	m_sI2Cs[QAD_I2C3].eI2C = QAD_I2C3;
	m_sI2Cs[QAD_I2C4].eI2C = QAD_I2C4;

	//Set Instances
	m_sI2Cs[QAD_I2C1].pInstance = I2C1;
	m_sI2Cs[QAD_I2C2].pInstance = I2C2;
	m_sI2Cs[QAD_I2C3].pInstance = I2C3;
	m_sI2Cs[QAD_I2C4].pInstance = I2C4;

  //Set Event IRQs
	m_sI2Cs[QAD_I2C1].eIRQ_Event = I2C1_EV_IRQn;
	m_sI2Cs[QAD_I2C2].eIRQ_Event = I2C2_EV_IRQn;
	m_sI2Cs[QAD_I2C3].eIRQ_Event = I2C3_EV_IRQn;
	m_sI2Cs[QAD_I2C4].eIRQ_Event = I2C4_EV_IRQn;

	//Set Error IRQs
	m_sI2Cs[QAD_I2C1].eIRQ_Error = I2C1_ER_IRQn;
	m_sI2Cs[QAD_I2C2].eIRQ_Error = I2C2_ER_IRQn;
	m_sI2Cs[QAD_I2C3].eIRQ_Error = I2C3_ER_IRQn;
	m_sI2Cs[QAD_I2C4].eIRQ_Error = I2C4_ER_IRQn;

}


	//-----------------------------
	//-----------------------------
	//QAD_I2CMgr Management Methods

//QAD_I2CMgr::imp_registerI2C
//QAD_I2CMgr Management Method
//
//To be called from static method registerI2C()
//Used to register an I2C peripheral as being used by a driver
//eI2C - the I2C peripheral to be registered
//Returns QA_OK if successful, or returns QA_Error_PeriphBusy if the selected I2C is already in use
QA_Result QAD_I2CMgr::imp_registerI2C(QAD_I2C_Periph eI2C, QAD_I2C_State eMode) {
  if (eI2C >= QAD_I2CNone)
  	return QA_Fail;

  if (m_sI2Cs[eI2C].eState)
  	return QA_Error_PeriphBusy;

  m_sI2Cs[eI2C].eState = eMode;
  return QA_OK;
}


//QAD_I2CMgr::imp_deregisterI2C
//QAD_I2CMgr Management Method
//
//To be called from static method deregisterI2C()
//Used to deregister an I2C to mark it as no longer being used by a driver
//eI2C - the I2C Peripheral to be deregistered
void QAD_I2CMgr::imp_deregisterI2C(QAD_I2C_Periph eI2C) {
  if (eI2C >= QAD_I2CNone)
  	return;

  m_sI2Cs[eI2C].eState = QAD_I2C_Unused;
}


	//------------------------
	//------------------------
	//QAD_I2CMgr Clock Methods

//QAD_I2CMgr::imp_enableClock
//QAD_I2CMgr Clock Method
//
//To be called by static method enableClock()
//Used to enable the clock for a specific I2C peripheral
//eI2C - the I2C peripheral to enable the clock for
void QAD_I2CMgr::imp_enableClock(QAD_I2C_Periph eI2C) {
  switch (eI2C) {
    case (QAD_I2C1):
    	__HAL_RCC_I2C1_CLK_ENABLE();
      __HAL_RCC_I2C1_FORCE_RESET();
      __HAL_RCC_I2C1_RELEASE_RESET();
    	break;
    case (QAD_I2C2):
    	__HAL_RCC_I2C2_CLK_ENABLE();
      __HAL_RCC_I2C2_FORCE_RESET();
      __HAL_RCC_I2C2_RELEASE_RESET();
    	break;
    case (QAD_I2C3):
    	__HAL_RCC_I2C3_CLK_ENABLE();
      __HAL_RCC_I2C3_FORCE_RESET();
      __HAL_RCC_I2C3_RELEASE_RESET();
    	break;
    case (QAD_I2C4):
    	__HAL_RCC_I2C4_CLK_ENABLE();
      __HAL_RCC_I2C4_FORCE_RESET();
      __HAL_RCC_I2C4_RELEASE_RESET();
    	break;
    case (QAD_I2CNone):
    	break;
  }
}


//QAD_I2CMgr::imp_disableClock
//QAD_I2CMgr Clock Method
//
//To be called from static method disableClock()
//Used to disable the clock for a specific I2C peripheral
//eI2C - the I2C peripheral to disable the clock for
void QAD_I2CMgr::imp_disableClock(QAD_I2C_Periph eI2C) {
  switch (eI2C) {
    case (QAD_I2C1):
    	__HAL_RCC_I2C1_CLK_DISABLE();
    	break;
    case (QAD_I2C2):
    	__HAL_RCC_I2C2_CLK_DISABLE();
    	break;
    case (QAD_I2C3):
    	__HAL_RCC_I2C3_CLK_DISABLE();
    	break;
    case (QAD_I2C4):
    	__HAL_RCC_I2C4_CLK_DISABLE();
    	break;
    case (QAD_I2CNone):
    	break;
  }
}


	//-------------------------
	//-------------------------
	//QAD_I2CMgr Status Methods

//QAD_I2CMgr::imp_getI2CsActive
//QAD_I2CMgr Status Method
//
//To be called from static method getI2CsActive
//Returns the number of I2C peripherals that are currently in-use (registered/active)
uint8_t QAD_I2CMgr::imp_getI2CsActive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
  	if (m_sI2Cs[i].eState)
  		uCount++;
  }
  return uCount;
}


//QAD_I2CMgr::imp_getI2CsInactive
//QAD_I2CMgr Status Method
//
//To be called from statuc method getI2CsInactive
//Returns the number of I2C peripherals that are currently not being used (deregistered/inactive)
uint8_t QAD_I2CMgr::imp_getI2CsInactive(void) {
  uint8_t uCount = 0;
  for (uint8_t i=0; i<QAD_I2C_PeriphCount; i++) {
  	if (!m_sI2Cs[i].eState)
  		uCount++;
  }
  return uCount;
}

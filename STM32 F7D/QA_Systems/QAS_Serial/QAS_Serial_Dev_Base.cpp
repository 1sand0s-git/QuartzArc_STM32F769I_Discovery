/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device Base Class                                        */
/*   Filename: QAS_Serial_Dev_Base.cpp                                     */
/*   Date: 28th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAS_Serial_Dev_Base.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //-----------------------------------------
  //QAS_Serial_Dev_Base Initialization Methods

//QAS_Serial_Dev_Base::init
//QAS_Serial_Dev_Base Initialization Method
QA_Result QAS_Serial_Dev_Base::init(void* p) {
  if (m_eInitState)
  	return QA_OK;

  if (imp_init(p))
  	return QA_Fail;

  //
  m_eInitState = QA_Initialized;
  return QA_OK;
}


//QAS_Serial_Dev_Base::deinit
//QAS_Serial_Dev_Base Initialization Method
void QAS_Serial_Dev_Base::deinit(void) {

	imp_deinit();

	m_eInitState = QA_NotInitialized;
}


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //----------------------------------
  //QAS_Serial_Dev_Base Handler Methods

//QAS_Serial_Dev_Base::handler
//QAS_Serial_Dev_Base Handler Method
void QAS_Serial_Dev_Base::handler(void* p) {
  imp_handler(p);
}


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //----------------------------------
  //QAS_Serial_Dev_Base Control Methods

//QAS_Serial_Dev_Base::getType
//QAS_Serial_Dev_Base Control Method
QAS_Serial_Dev_Base::DeviceType QAS_Serial_Dev_Base::getType(void) {
  return m_eDeviceType;
}


//QAS_Serial_Dev_Base::rxStart
//QAS_Serial_Dev_Base Control Method
void QAS_Serial_Dev_Base::rxStart(void) {
  if (m_eRXState)
  	return;
  imp_rxStart();
  m_eRXState = PeriphActive;
}


//QAS_Serial_Dev_Base::rxStop
//QAS_Serial_Dev_Base Control Method
void QAS_Serial_Dev_Base::rxStop(void) {
  imp_rxStop();
  m_eRXState = PeriphInactive;
}


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //-----------------------------------
  //QAS_Serial_Dev_Base Transmit Methods

//QAS_Serial_Dev_Base::
//QAS_Serial_Dev_Base Transmit Method
void QAS_Serial_Dev_Base::txString(const char* str) {
  uint16_t uLen = strlen(str);
  for (uint16_t i=0; i<uLen; i++) {
  	m_pTXFIFO->push(str[i]);
  }
  imp_txStart();
}


//QAS_Serial_Dev_Base::
//QAS_Serial_Dev_Base Transmit Method
void QAS_Serial_Dev_Base::txStringCR(const char* str) {
  uint16_t uLen = strlen(str);
  for (uint16_t i=0; i<uLen; i++) {
  	m_pTXFIFO->push(str[i]);
  }
  m_pTXFIFO->push(13);
  imp_txStart();
}


//QAS_Serial_Dev_Base::
//QAS_Serial_Dev_Base Transmit Method
void QAS_Serial_Dev_Base::txCR(void) {
  m_pTXFIFO->push(13);
  imp_txStart();
}


//QAS_Serial_Dev_Base::
//QAS_Serial_Dev_Base Transmit Method
void QAS_Serial_Dev_Base::txData(const uint8_t* pData, uint16_t uSize) {
  for (uint16_t i=0; i<uSize; i++)
  	m_pTXFIFO->push(pData[i]);
  imp_txStart();
}


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //----------------------------------
  //QAS_Serial_Dev_Base Receive Methods


//QAS_Serial_Dev_Base::rxHasData
//QAS_Serial_Dev_Base Receive Methods
QAS_Serial_Dev_Base::DataState QAS_Serial_Dev_Base::rxHasData(void) {
  if (m_pRXFIFO->empty())
  	return NoData;

  return HasData;
}


//QAS_Serial_Dev_Base::rxPop
//QAS_Serial_Dev_Base Transmit Method
uint8_t QAS_Serial_Dev_Base::rxPop(void) {
  return m_pRXFIFO->pop();
}


//QAS_Serial_Dev_Base::rxData
//QAS_Serial_Dev_Base Transmit Method
QA_Result QAS_Serial_Dev_Base::rxData(uint8_t* pData, uint16_t& uSize) {
  uSize = m_pRXFIFO->pending();
  if (!uSize)
  	return QA_Fail;

  for (uint16_t i=0; i<uSize; i++) {
  	pData[i] = m_pRXFIFO->pop();
  }
  return QA_OK;
}




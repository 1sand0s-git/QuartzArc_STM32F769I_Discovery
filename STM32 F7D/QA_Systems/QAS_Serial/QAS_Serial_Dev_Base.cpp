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
  //-----------------------------------------
  //QAS_Serial_Dev_Base Initialization Methods

//QAS_Serial_Dev_Base::init
//QAS_Serial_Dev_Base Initialization Method
//
//System Class initialization method
//Calls imp_init() pure virtual function, which is to be implemented by inheriting class
//p - void pointer containing a pointer to any data that may be needed by the imp_init method of the inheriting class
//Returns QA_OK if initialization successful, or QA_Fail or other QA_Result error if initialization fails
QA_Result QAS_Serial_Dev_Base::init(void* p) {
  if (m_eInitState)
  	return QA_OK;

  QA_Result eRes = imp_init(p);
  if (eRes)
  	return eRes;

  //
  m_eInitState = QA_Initialized;
  return QA_OK;
}


//QAS_Serial_Dev_Base::deinit
//QAS_Serial_Dev_Base Initialization Method
//
//System class deinitialization method
//Calls imp_deinit() pure virtual function, which is to be implemented by inheriting class
void QAS_Serial_Dev_Base::deinit(void) {

	imp_deinit();

	m_eInitState = QA_NotInitialized;
}


  //----------------------------------
  //----------------------------------
  //QAS_Serial_Dev_Base Handler Methods

//QAS_Serial_Dev_Base::handler
//QAS_Serial_Dev_Base Handler Method
//
//Interrupt Handler method to be called by system IRQ handler
//Calls imp_handler() pure virtual function, which is to be implemented by inheriting class
void QAS_Serial_Dev_Base::handler(void* p) {
  imp_handler(p);
}


  //----------------------------------
  //----------------------------------
  //QAS_Serial_Dev_Base Control Methods

//QAS_Serial_Dev_Base::getType
//QAS_Serial_Dev_Base Control Method
//
//Returns type of serial device. A member for QAD_Serial_Dev_Base::DeviceType
QAS_Serial_Dev_Base::DeviceType QAS_Serial_Dev_Base::getType(void) {
  return m_eDeviceType;
}


//QAS_Serial_Dev_Base::rxStart
//QAS_Serial_Dev_Base Control Method
//
//Starts receive component of system
//Calls imp_rxStart() pure virtual function, which is to be implemented by inheriting class
void QAS_Serial_Dev_Base::rxStart(void) {
  if (m_eRXState)
  	return;
  imp_rxStart();
  m_eRXState = QA_Active;
}


//QAS_Serial_Dev_Base::rxStop
//QAS_Serial_Dev_Base Control Method
//
//Stops receive component of system
//Calls imp_rxStop() pure virtual function, which is to be implemented by inheriting class
void QAS_Serial_Dev_Base::rxStop(void) {
  imp_rxStop();
  m_eRXState = QA_Inactive;
}


  //-----------------------------------
  //-----------------------------------
  //QAS_Serial_Dev_Base Transmit Methods

//QAS_Serial_Dev_Base::txString
//QAS_Serial_Dev_Base Transmit Method
//
//Used to transmit a c-style string
//Calls imp_txStart() pure virtual function to begin transmission, which is to be implemented by the inheriting class
//str - the null terminated c-style string to be transmitted
void QAS_Serial_Dev_Base::txString(const char* str) {
  uint16_t uLen = strlen(str);
  for (uint16_t i=0; i<uLen; i++) {
  	m_pTXFIFO->push(str[i]);
  }
  imp_txStart();
}


//QAS_Serial_Dev_Base::txStringCR
//QAS_Serial_Dev_Base Transmit Method
//
//Used to transmit a c-style string, followed by a carriage return character (ASCII #13)
//Calls imp_txStart() pure virtual function to begin transmission, which is to be implemented by the inheriting class
//str - the null terminated c-style string to be transmitted
void QAS_Serial_Dev_Base::txStringCR(const char* str) {
  uint16_t uLen = strlen(str);
  for (uint16_t i=0; i<uLen; i++) {
  	m_pTXFIFO->push(str[i]);
  }
  m_pTXFIFO->push(13);
  imp_txStart();
}


//QAS_Serial_Dev_Base::txCR
//QAS_Serial_Dev_Base Transmit Method
//
//Used to transmit a carriage return character (ASCII #13)
//Calls imp_txStart() pure virtual function to begin transmission, which is to be implemented by the inheriting class
void QAS_Serial_Dev_Base::txCR(void) {
  m_pTXFIFO->push(13);
  imp_txStart();
}


//QAS_Serial_Dev_Base::txData
//QAS_Serial_Dev_Base Transmit Method
//
//Used to transmit raw data
//Calls imp_txStart() pure virtual function to begin transmission, which is to be implemented by the inheriting class
//pData - pointer to the array of bytes to be transmitted
//uSize - size in bytes of the data to be transmitted
void QAS_Serial_Dev_Base::txData(const uint8_t* pData, uint16_t uSize) {
  for (uint16_t i=0; i<uSize; i++)
  	m_pTXFIFO->push(pData[i]);
  imp_txStart();
}


  //----------------------------------
  //----------------------------------
  //QAS_Serial_Dev_Base Receive Methods


//QAS_Serial_Dev_Base::rxHasData
//QAS_Serial_Dev_Base Receive Methods
//
//uSize - Pointer to a uint16_t that is filled with the nunber of received bytes currently pending in the RX FIFO buffer
//        This can also be NULL when calling the function if you just want to confirm if pending data is present and are not
//        concerned with how many bytes are pending.
//Returns a member of the QAS_Serial_Dev_Base::DataState enum to indicate if the RX FIFO buffer contains received data
QAS_Serial_Dev_Base::DataState QAS_Serial_Dev_Base::rxHasData(uint16_t* uSize) {
  if (m_pRXFIFO->empty())
  	return NoData;

  if (uSize)
  	*uSize = m_pRXFIFO->pending();

  return HasData;
}


//QAS_Serial_Dev_Base::rxPop
//QAS_Serial_Dev_Base Transmit Method
//
//Returns a single byte of data from the RX FIFO buffer
uint8_t QAS_Serial_Dev_Base::rxPop(void) {
  return m_pRXFIFO->pop();
}


//QAS_Serial_Dev_Base::rxData
//QAS_Serial_Dev_Base Transmit Method
//
//Retrieves all received bytes currently waiting in the RX FIFO buffer
//pData - pointer to an array of bytes to be filled with the received data
//uSize - pointer to a uint16_t that is filled with the number of bytes that were received
//Returns QA_OK if received data was available, or QA_Fail if no data was available
QA_Result QAS_Serial_Dev_Base::rxData(uint8_t* pData, uint16_t* uSize) {
  *uSize = m_pRXFIFO->pending();
  if (!(*uSize))
  	return QA_Fail;

  for (uint16_t i=0; i<(*uSize); i++) {
  	pData[i] = m_pRXFIFO->pop();
  }
  return QA_OK;
}




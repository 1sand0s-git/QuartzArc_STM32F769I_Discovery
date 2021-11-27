/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: I2C Driver                                                      */
/*   Filename: QAD_I2C.cpp                                                 */
/*   Date: 27th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_I2C.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------




  //------------------------------
	//------------------------------
	//QAD_I2C Initialization Methods

//QAD_I2C::init
//QAD_I2C Initialization Method
QA_Result QAD_I2C::init(void) {
  if (QAD_I2CMgr::getState(m_eI2C))
  	return QA_Error_PeriphBusy;

  QAD_I2CMgr::registerI2C(m_eI2C, QAD_I2C_InUse_Master);
  QA_Result eRes = periphInit();

  if (eRes)
  	QAD_I2CMgr::deregisterI2C(m_eI2C);
  return eRes;
}


//QAD_I2C::deinit
//QAD_I2C Initialization Method
void QAD_I2C::deinit(void) {
  if (!m_eInitState)
  	return;

  periphDeinit(DeinitFull);
  QAD_I2CMgr::deregisterI2C(m_eI2C);
}


//QAD_I2C::getInitState
//QAD_I2C Initialization Method
QA_InitState QAD_I2C::getInitState(void) {
  return m_eInitState;
}


//QAD_I2C::getHandle
//QAD_I2C Initialization Method
I2C_HandleTypeDef& QAD_I2C::getHandle(void) {
  return m_sHandle;
}


  //-----------------------
	//-----------------------
	//QAD_I2C Control Methods

//QAD_I2C::start
//QAD_I2C Control Method
void QAD_I2C::start(void) {
  __HAL_I2C_ENABLE(&m_sHandle);
  m_eState = QA_Active;
}


//QAD_I2C::stop
//QAD_I2C Control Method
void QAD_I2C::stop(void) {
  __HAL_I2C_DISABLE(&m_sHandle);
  m_eState = QA_Inactive;
}


//QAD_I2C::getState
//QAD_I2C Control Method
QA_ActiveState QAD_I2C::getState(void) {
  return m_eState;
}


	//--------------------------
  //--------------------------
	//QAD_I2C Transceive Methods

//QAD_I2C::write8Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::write8Bit(uint16_t uAddr, uint8_t uReg, uint8_t uValue) {
  return write((uint16_t)uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, (uint8_t*)&uValue, 1);
}


//QAD_I2C::write16Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::write16Bit(uint16_t uAddr, uint16_t uReg, uint16_t uValue) {
	uint16_t uProcVal = ((uint16_t)(uValue >> 8) & 0x00FF) | ((uint16_t)(uValue << 8) & 0xFF00);
  return write((uint16_t)uAddr, uReg, m_uMemAddrSize16Bit, (uint8_t*)&uProcVal, 2);
}


//QAD_I2C::writeMultiple8Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::writeMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength) {
  return write((uint16_t)uAddr, uReg, m_uMemAddrSize8Bit, pData, uLength);
}


//QAD_I2C::read8Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::read8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pValue) {
  return read(uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, pValue, 1);
}


//QAD_I2C::read16Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::read16Bit(uint16_t uAddr, uint16_t uReg, uint16_t* pValue) {
  uint16_t uProcVal;
  if (read(uAddr, uReg, m_uMemAddrSize16Bit, (uint8_t*)&uProcVal, 2))
  	return QA_Fail;

  *pValue = ((uint16_t)(uProcVal >> 8) & 0x00FF) | ((uint16_t)(uProcVal << 8) & 0xFF00);
  return QA_OK;
}


//QAD_I2C::readMultiple8Bit
//QAD_I2C Transceive Method
QA_Result QAD_I2C::readMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength) {
  return read(uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, pData, uLength);
}


  //--------------------------------------
	//--------------------------------------
	//QAD_I2C Private Initialization Methods

//QAD_I2C::periphInit
//QAD_I2C Private Initialization Method
QA_Result QAD_I2C::periphInit(void) {
  GPIO_InitTypeDef GPIO_Init = {0};

  //Init SCK Pin
  GPIO_Init.Pin       = m_uSCL_Pin;
  GPIO_Init.Mode      = GPIO_MODE_AF_OD;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate = m_uSCL_AF;
  HAL_GPIO_Init(m_pSCL_GPIO, &GPIO_Init);

  //Init SDA Pin
  GPIO_Init.Pin       = m_uSDA_Pin;
  GPIO_Init.Mode      = GPIO_MODE_AF_OD;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate = m_uSDA_AF;
  HAL_GPIO_Init(m_pSDA_GPIO, &GPIO_Init);


  //Enable I2C Clock
  QAD_I2CMgr::enableClock(m_eI2C);


  //Initialize I2C Peripheral
  m_sHandle.Instance             = QAD_I2CMgr::getInstance(m_eI2C);
  m_sHandle.Init.Timing          = m_uTiming;
  m_sHandle.Init.AddressingMode  = m_eAddressingMode;
  m_sHandle.Init.DualAddressMode = m_eDualAddressingMode;
  m_sHandle.Init.GeneralCallMode = m_eGeneralCallMode;
  m_sHandle.Init.NoStretchMode   = m_eNoStretchMode;
  m_sHandle.Init.OwnAddress1     = m_uOwnAddress1;
  m_sHandle.Init.OwnAddress2     = m_uOwnAddress2;
  if (HAL_I2C_Init(&m_sHandle) != HAL_OK) {
  	periphDeinit(DeinitPartial);
  	return QA_Fail;
  }


  //Enable I2C Interrupts
  HAL_NVIC_SetPriority(QAD_I2CMgr::getIRQEvent(m_eI2C), m_uIRQPriority_Event, 0x0);
  HAL_NVIC_EnableIRQ(QAD_I2CMgr::getIRQEvent(m_eI2C));

  HAL_NVIC_SetPriority(QAD_I2CMgr::getIRQError(m_eI2C), m_uIRQPriority_Error, 0x0);
  HAL_NVIC_EnableIRQ(QAD_I2CMgr::getIRQError(m_eI2C));


  //Set Driver States
  m_eInitState = QA_Initialized;
  m_eState     = QA_Inactive;


  //Return
  return QA_OK;
}


//QAD_I2C::periphDeinit
//QAD_I2C Private Initialization Method
void QAD_I2C::periphDeinit(DeinitMode eDeinitMode) {

	if (eDeinitMode) {

		stop();

		HAL_NVIC_DisableIRQ(QAD_I2CMgr::getIRQError(m_eI2C));
		HAL_NVIC_DisableIRQ(QAD_I2CMgr::getIRQEvent(m_eI2C));

		HAL_I2C_DeInit(&m_sHandle);
	}

	//Disable I2C Clock
	QAD_I2CMgr::disableClock(m_eI2C);

	//Deinit GPIOs
	HAL_GPIO_DeInit(m_pSCL_GPIO, m_uSCL_Pin);
	HAL_GPIO_DeInit(m_pSDA_GPIO, m_uSDA_Pin);

  //Set Driver States
	m_eState     = QA_Inactive;
	m_eInitState = QA_NotInitialized;

}


	//----------------------------------
  //----------------------------------
	//QAD_I2C Private Transceive Methods

//QAD_I2C::write
//QAD_I2C Private Transceive Method
QA_Result QAD_I2C::write(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength) {
  if (HAL_I2C_Mem_Write(&m_sHandle, uAddr, (uint16_t)uReg, uMemAddress, pData, uLength, m_uTimeout) != HAL_OK)
  	return QA_Fail;

  return QA_OK;
}


//QAD_I2C::read
//QAD_I2C Private Transceive Method
QA_Result QAD_I2C::read(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength) {
  if (HAL_I2C_Mem_Read(&m_sHandle, uAddr, (uint16_t)uReg, uMemAddress, pData, uLength, m_uTimeout) != HAL_OK)
  	return QA_Fail;

  return QA_OK;
}

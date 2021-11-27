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
//
//Used to initialize the I2C driver
//Returns QA_OK if initialization successful, or an error if not successful (a member of QA_Result aas defined in setup.hpp)
QA_Result QAD_I2C::init(void) {

	//Check if selected I2C peripheral is currently available
  if (QAD_I2CMgr::getState(m_eI2C))
  	return QA_Error_PeriphBusy;

  //Register I2C peripheral as now being in use
  QAD_I2CMgr::registerI2C(m_eI2C, QAD_I2C_InUse_Master);

  //Initialize I2C peripheral
  QA_Result eRes = periphInit();

  //If initialization failed then deregister I2C peripheral
  if (eRes)
  	QAD_I2CMgr::deregisterI2C(m_eI2C);

  //Return initialization result
  return eRes;
}


//QAD_I2C::deinit
//QAD_I2C Initialization Method
//
//Used to deinitialize the I2C driver
void QAD_I2C::deinit(void) {

	//Return if driver is not initialized
  if (!m_eInitState)
  	return;

  //Deinitialize peripheral
  periphDeinit(DeinitFull);

  //Deregister I2C peripheral
  QAD_I2CMgr::deregisterI2C(m_eI2C);
}


//QAD_I2C::getInitState
//QAD_I2C Initialization Method
//
//Used to return whether the driver is currently initialized
//Returns a member of QA_InitState (defined in setup.hpp).
QA_InitState QAD_I2C::getInitState(void) {
  return m_eInitState;
}


//QAD_I2C::getHandle
//QAD_I2C Initialization Method
//
//Used to return a reference to the HAL peripheral handle for the I2C peripheral
I2C_HandleTypeDef& QAD_I2C::getHandle(void) {
  return m_sHandle;
}


  //-----------------------
	//-----------------------
	//QAD_I2C Control Methods

//QAD_I2C::start
//QAD_I2C Control Method
//
//Used to enable the I2C driver
void QAD_I2C::start(void) {
  __HAL_I2C_ENABLE(&m_sHandle);
  m_eState = QA_Active;
}


//QAD_I2C::stop
//QAD_I2C Control Method
//
//Used to disable the I2C driver
void QAD_I2C::stop(void) {
  __HAL_I2C_DISABLE(&m_sHandle);
  m_eState = QA_Inactive;
}


//QAD_I2C::getState
//QAD_I2C Control Method
//
//Used to return whether the driver is currently active
//Returns a member of QA_ActiveState (defined in setup.hpp)
QA_ActiveState QAD_I2C::getState(void) {
  return m_eState;
}


	//--------------------------
  //--------------------------
	//QAD_I2C Transceive Methods

//QAD_I2C::write8Bit
//QAD_I2C Transceive Method
//
//Used to write a value to an I2C device connected to the bus, using an 8bit register address and 8bit data
//uAddr  - The address of the I2C device to write to
//uReg   - The register address to access on the I2C device that is being written to
//uValue - The value to write to the I2C device
//Returns QA_OK if successful, or QA_Fail if unable to perform write
QA_Result QAD_I2C::write8Bit(uint16_t uAddr, uint8_t uReg, uint8_t uValue) {
  return write((uint16_t)uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, (uint8_t*)&uValue, 1);
}


//QAD_I2C::write16Bit
//QAD_I2C Transceive Method
//
//Used to write a value to an I2C device connected to the bus, using a 16bit register address and 16bit data
//uAddr   - The address of the I2C device to write to
//uReg    - The register address to access on the I2C deevice that is being written to
//uValue  - The value to write to the I2C device
//Returns QA_OK if successful, or QA_Fail if unable to perform write
QA_Result QAD_I2C::write16Bit(uint16_t uAddr, uint16_t uReg, uint16_t uValue) {
	uint16_t uProcVal = ((uint16_t)(uValue >> 8) & 0x00FF) | ((uint16_t)(uValue << 8) & 0xFF00);
  return write((uint16_t)uAddr, uReg, m_uMemAddrSize16Bit, (uint8_t*)&uProcVal, 2);
}


//QAD_I2C::writeMultiple8Bit
//QAD_I2C Transceive Method
//
//Used to write data to an I2C device connected to the bus, using an 8bit register address
//uAddr   - The address of the I2C device to write to
//uReg    - The register address to access on the I2C device that is being written to
//pData   - A pointer to an array of bytes containing the data to be written
//uLength - The number of bytes to be written
//Returns QA_OK if successful, or QA_Fail if unable to perform write
QA_Result QAD_I2C::writeMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength) {
  return write((uint16_t)uAddr, uReg, m_uMemAddrSize8Bit, pData, uLength);
}


//QAD_I2C::read8Bit
//QAD_I2C Transceive Method
//
//Used to read a value from an I2C device connected to the bus, using an 8bit register address and 8bit data
//uAddr  - The address of the I2C device to read from
//uReg   - The register address to access on the I2C device that is being read from
//pValue - A pointer to a uint8_t where the read data will be stored
//Returns QA_OK if successful, or QA_Fail if unable to perform read
QA_Result QAD_I2C::read8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pValue) {
  return read(uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, pValue, 1);
}


//QAD_I2C::read16Bit
//QAD_I2C Transceive Method
//
//Used to read a value from an I2C device connected to the bus, using a 16bit register address and 16bit data
//uAddr  - The address of the I2C device to read from
//uReg   - The register address to access on the I2C device that is being read from
//pValue - A pointer to a uint16_t where the read data will be stored
//Returns QA_OK if successful, or QA_Fail if unable to perform read
QA_Result QAD_I2C::read16Bit(uint16_t uAddr, uint16_t uReg, uint16_t* pValue) {
  uint16_t uProcVal;
  if (read(uAddr, uReg, m_uMemAddrSize16Bit, (uint8_t*)&uProcVal, 2))
  	return QA_Fail;

  *pValue = ((uint16_t)(uProcVal >> 8) & 0x00FF) | ((uint16_t)(uProcVal << 8) & 0xFF00);
  return QA_OK;
}


//QAD_I2C::readMultiple8Bit
//QAD_I2C Transceive Method
//
//Used to read data from an I2C device connected to the bus, using an 8bit register address
//uAddr  - The address of the I2C device to read from
//uReg   - The register address to access on the I2C device that is being read from
//pData   - A pointer to an array of bytes that will contain the data to be read
//uLength - The number of bytes to be read
//Returns QA_OK if successful, or QA_Fail if unable to perform read
QA_Result QAD_I2C::readMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength) {
  return read(uAddr, (uint16_t)uReg, m_uMemAddrSize8Bit, pData, uLength);
}


  //--------------------------------------
	//--------------------------------------
	//QAD_I2C Private Initialization Methods

//QAD_I2C::periphInit
//QAD_I2C Private Initialization Method
//
//Used to initialize the GPIOs, peripheral clock, and the I2C peripheral itself as well as setting interrupt priorities and enabling the interrupts
//In the case of a failed initialization, a partial deinitialization will be performed to make sure the peripheral, clock and GPIOs are all in the
//uninitialized state
//Returns QA_OK if successful, or QA_Fail if initialization fails
QA_Result QAD_I2C::periphInit(void) {
  GPIO_InitTypeDef GPIO_Init = {0};

  //Init SCK GPIO Pin
  GPIO_Init.Pin       = m_uSCL_Pin;                //Set pin number
  GPIO_Init.Mode      = GPIO_MODE_AF_OD;           //Set clock pin as alternate function in open-drain mode
  GPIO_Init.Pull      = GPIO_PULLUP;               //Enable pull-up resistor on pin
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH; //Set GPIO pin speed
  GPIO_Init.Alternate = m_uSCL_AF;                 //Set alternate function to suit required I2C peripheral
  HAL_GPIO_Init(m_pSCL_GPIO, &GPIO_Init);

  //Init SDA GPIO Pin
  GPIO_Init.Pin       = m_uSDA_Pin;                //Set pin number
  GPIO_Init.Mode      = GPIO_MODE_AF_OD;           //Set clock pin as alternate function in open-drain mode
  GPIO_Init.Pull      = GPIO_PULLUP;               //Enable pull-up resistor on pin
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH; //Set GPIO pin speed
  GPIO_Init.Alternate = m_uSDA_AF;                 //Set alternate function to suit required I2C peripheral
  HAL_GPIO_Init(m_pSDA_GPIO, &GPIO_Init);


  //Enable I2C Clock
  QAD_I2CMgr::enableClock(m_eI2C);


  //Initialize I2C Peripheral
  m_sHandle.Instance             = QAD_I2CMgr::getInstance(m_eI2C);  //Set instance for required I2C peripheral
  m_sHandle.Init.Timing          = m_uTiming;                        //Set selected clock speed
  m_sHandle.Init.AddressingMode  = m_eAddressingMode;                //Set selected addressing mode
  m_sHandle.Init.DualAddressMode = m_eDualAddressingMode;            //Set whether dual addressing mode is being used
  m_sHandle.Init.GeneralCallMode = m_eGeneralCallMode;               //Set whether general call addressing mode is being used
  m_sHandle.Init.NoStretchMode   = m_eNoStretchMode;                 //Set whether clock stretching is currently disabled
  m_sHandle.Init.OwnAddress1     = m_uOwnAddress1;                   //Set primary peripheral I2C address
  m_sHandle.Init.OwnAddress2     = m_uOwnAddress2;                   //Set secondary peripheral I2C address
  if (HAL_I2C_Init(&m_sHandle) != HAL_OK) {
  	periphDeinit(DeinitPartial);
  	return QA_Fail;
  }


  //Enable I2C Interrupt priorties and enable IRQs
  HAL_NVIC_SetPriority(QAD_I2CMgr::getIRQEvent(m_eI2C), m_uIRQPriority_Event, 0x0);
  HAL_NVIC_EnableIRQ(QAD_I2CMgr::getIRQEvent(m_eI2C));

  HAL_NVIC_SetPriority(QAD_I2CMgr::getIRQError(m_eI2C), m_uIRQPriority_Error, 0x0);
  HAL_NVIC_EnableIRQ(QAD_I2CMgr::getIRQError(m_eI2C));


  //Set Driver States
  m_eInitState = QA_Initialized; //Set driver state as initialized
  m_eState     = QA_Inactive;    //Set driver as currently inactive


  //Return
  return QA_OK;
}


//QAD_I2C::periphDeinit
//QAD_I2C Private Initialization Method
//
//Used to deinitialize the GPIOs, peripheral clock, and the peripheral itself, as well as disabling interrupts
//eDeinitMode - Set to DeinitPartial to perform a partial deinitialization (only to be used by periphInit() method
//              in a case where peripheral initialization has failed).
//              Set to DeinitFull to perform a full deinitialization in a case where the driver is fully initialized
void QAD_I2C::periphDeinit(DeinitMode eDeinitMode) {

	//Check if full deinitialization is required
	if (eDeinitMode) {

		//Deactivate the peripheral
		stop();

		//Disable the interrupts
		HAL_NVIC_DisableIRQ(QAD_I2CMgr::getIRQError(m_eI2C));
		HAL_NVIC_DisableIRQ(QAD_I2CMgr::getIRQEvent(m_eI2C));

		//Deinitialize the peripheral
		HAL_I2C_DeInit(&m_sHandle);
	}

	//Disable peripheral clock
	QAD_I2CMgr::disableClock(m_eI2C);

	//Deinitialize GPIOs
	HAL_GPIO_DeInit(m_pSCL_GPIO, m_uSCL_Pin);
	HAL_GPIO_DeInit(m_pSDA_GPIO, m_uSDA_Pin);

  //Set Driver States
	m_eState     = QA_Inactive;       //Set driver as being inactive
	m_eInitState = QA_NotInitialized; //Set driver as not being initialized

}


	//----------------------------------
  //----------------------------------
	//QAD_I2C Private Transceive Methods

//QAD_I2C::write
//QAD_I2C Private Transceive Method
//
//Tool method to be used to make it easier when performing writes to external I2C devices
//uAddr       - The address of the I2C device to write to
//uReg        - The register address to access on the I2C device that is being written to
//uMemAddress - The number of bytes in size of the register/memory address
//pData       - A pointer to an array of bytes containing the data to be written
//uLength     - The number of bytes to be written
//Returns QA_OK if successful, or QA_Fail if unable to perform write
QA_Result QAD_I2C::write(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength) {
  if (HAL_I2C_Mem_Write(&m_sHandle, uAddr, (uint16_t)uReg, uMemAddress, pData, uLength, m_uTimeout) != HAL_OK)
  	return QA_Fail;

  return QA_OK;
}


//QAD_I2C::read
//QAD_I2C Private Transceive Method
//
//Tool method to be used to make it easier when performing reads from external I2C devices
//uAddr       - The address of the I2C device to read from
//uReg        - The register address to access on the I2C device that is being read from
//uMemAddress - The number of bytes in size of the register/memory address
//pData       - A pointer to an array of bytes to contain the data to be read
//uLength     - The number of bytes to be read
//Returns QA_OK if successful, or QA_Fail if unable to perform read
QA_Result QAD_I2C::read(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength) {
  if (HAL_I2C_Mem_Read(&m_sHandle, uAddr, (uint16_t)uReg, uMemAddress, pData, uLength, m_uTimeout) != HAL_OK)
  	return QA_Fail;

  return QA_OK;
}

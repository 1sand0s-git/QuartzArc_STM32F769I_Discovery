/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: SDMMC / SD Card Driver                                          */
/*   Filename: QAD_SDMMC.cpp                                               */
/*   Date: 10th February 2022                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_SDMMC.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


	//--------------------------------
	//--------------------------------
	//QAD_SDMMC Initialization Methods

//QAD_SDMMC::imp_init
//QAD_SDMMC Initialization Method
QA_Result QAD_SDMMC::imp_init(void) {
  if (m_eInitState)
  	return QA_OK;

  //Initialization Peripheral
  if (imp_periphInit())
  	return QA_Fail;

  //Set Driver States
  m_eInitState   = QA_Initialized;
  m_eCardState   = QAD_SDMMC_State_NoCard;

  //Return
  return QA_OK;
}


//QAD_SDMMC::imp_deinit
//QAD_SDMMC Initialization Method
void QAD_SDMMC::imp_deinit(void) {
  if (!m_eInitState)
  	return;

  //Deinitialize Peripheral
  imp_periphDeinit(DeinitFull);

  //Set Driver States
  m_eInitState   = QA_NotInitialized;
  m_eCardState   = QAD_SDMMC_State_NoCard;

}


	//-------------------------------------------
	//-------------------------------------------
	//QAD_SDMMC Peripheral Initialization Methods

//QAD_SDMMC::imp_periphInit
//QAD_SDMMC Peripheral Initialization Method
QA_Result QAD_SDMMC::imp_periphInit(void) {
	GPIO_InitTypeDef GPIO_Init = {0};

	//----------------
	//Initialize GPIOs

		//Card Detect
	GPIO_Init.Pin       = QAD_SDMMC_CARDDETECT_PIN;
	GPIO_Init.Mode      = GPIO_MODE_INPUT;
	GPIO_Init.Pull      = GPIO_PULLUP;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(QAD_SDMMC_CARDDETECT_PORT, &GPIO_Init);

		//SDMMC Clock
	GPIO_Init.Pin       = QAD_SDMMC_CLK_PIN;
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_PULLUP;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = QAD_SDMMC_CLK_AF;
	HAL_GPIO_Init(QAD_SDMMC_CLK_PORT, &GPIO_Init);

		//SDMMC Command
	GPIO_Init.Pin       = QAD_SDMMC_CMD_PIN;
	GPIO_Init.Alternate = QAD_SDMMC_CMD_AF;
	HAL_GPIO_Init(QAD_SDMMC_CMD_PORT, &GPIO_Init);

		//SDMMC Data 0
	GPIO_Init.Pin       = QAD_SDMMC_DATA0_PIN;
	GPIO_Init.Alternate = QAD_SDMMC_DATA0_AF;
	HAL_GPIO_Init(QAD_SDMMC_DATA0_PORT, &GPIO_Init);

		//SDMMC Data 1
	GPIO_Init.Pin       = QAD_SDMMC_DATA1_PIN;
	GPIO_Init.Alternate = QAD_SDMMC_DATA1_AF;
	HAL_GPIO_Init(QAD_SDMMC_DATA1_PORT, &GPIO_Init);

		//SDMMC Data 2
	GPIO_Init.Pin       = QAD_SDMMC_DATA2_PIN;
	GPIO_Init.Alternate = QAD_SDMMC_DATA2_AF;
	HAL_GPIO_Init(QAD_SDMMC_DATA2_PORT, &GPIO_Init);

		//SDMMC Data 3
	GPIO_Init.Pin       = QAD_SDMMC_DATA3_PIN;
	GPIO_Init.Alternate = QAD_SDMMC_DATA3_AF;
	HAL_GPIO_Init(QAD_SDMMC_DATA3_PORT, &GPIO_Init);


	//------------------
	//Enable SDMMC Clock
	__HAL_RCC_SDMMC2_CLK_ENABLE();

	//Return
	return QA_OK;
}


//QAD_SDMMC::imp_periphDeinit
//QAD_SDMMC Peripheral Initialization Method
void QAD_SDMMC::imp_periphDeinit(DeinitMode eMode) {

	//Check if full deinitialization is required
	if (eMode) {

		//Unmount card
		imp_unmount();

	}

	//Disable SDMMC Clock
	__HAL_RCC_SDMMC2_CLK_ENABLE();

	//Deinit GPIOs
	HAL_GPIO_DeInit(QAD_SDMMC_CARDDETECT_PORT, QAD_SDMMC_CARDDETECT_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_CLK_PORT, QAD_SDMMC_CLK_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_CMD_PORT, QAD_SDMMC_CMD_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_DATA0_PORT, QAD_SDMMC_DATA0_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_DATA1_PORT, QAD_SDMMC_DATA1_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_DATA2_PORT, QAD_SDMMC_DATA2_PIN);
	HAL_GPIO_DeInit(QAD_SDMMC_DATA3_PORT, QAD_SDMMC_DATA3_PIN);

}


	//----------------------------
	//----------------------------
	//QAD_SDMMC Processing Methods

//QAD_SDMMC::imp_process
//QAD_SDMMC Processing Method
QA_Result QAD_SDMMC::imp_process(void) {
	if (!m_eInitState)
		return QA_Fail;

	//
	if (imp_getCardPresent() == QAD_SDMMC_CardPresent) {

		if (m_eCardState == QAD_SDMMC_State_NoCard) {
			if (imp_mount())
				return QA_Fail;
		}

	} else {

		if (m_eCardState != QAD_SDMMC_State_NoCard) {
			imp_unmount();
		}

	}

	//Return
	return QA_OK;
}


//----------------------
//----------------------
//QAD_SDMMC Card Methods

//QAD_SDMMC::imp_getCardPresent
//QAD_SDMMC Card Method
QAD_SDMMC_CardPresence QAD_SDMMC::imp_getCardPresent(void) {

	if (HAL_GPIO_ReadPin(QAD_SDMMC_CARDDETECT_PORT, QAD_SDMMC_CARDDETECT_PIN) == GPIO_PIN_SET) {
		return QAD_SDMMC_CardNotPresent;
	}

	return QAD_SDMMC_CardPresent;
}


//QAD_SDMMC::imp_getCardInfo
//QAD_SDMMC Card Method
QA_Result QAD_SDMMC::imp_getCardInfo(void) {
	HAL_SD_CardInfoTypeDef sCardInfo;

	//Get Card Info
	if (HAL_SD_GetCardInfo(&m_sHandle, &sCardInfo) != HAL_OK) {

		m_uSize       = 0;
		m_uSizeMB     = 0;
		m_uBlockSize  = 0;
		m_uBlockCount = 0;

		m_uType       = 0;
		m_uVersion    = 0;
		m_uClass      = 0;

		return QA_Fail;
	}

	//Store Card Info
	uint64_t uBlockCount = sCardInfo.BlockNbr;
	uint64_t uBlockSize  = sCardInfo.BlockSize;
	m_uSize       = uBlockCount * uBlockSize;
	m_uSizeMB     = m_uSize / 1048576;
	m_uBlockCount = uBlockCount;
	m_uBlockSize  = uBlockSize;

	m_uType       = sCardInfo.CardType;
	m_uVersion    = sCardInfo.CardVersion;
	m_uClass      = sCardInfo.Class;

	return QA_OK;
}


	//-----------------------
	//-----------------------
	//QAD_SDMMC Mount Methods

//QAD_SDMMC::imp_mount
//QAD_SDMMC Mount Method
QA_Result QAD_SDMMC::imp_mount(void) {

	//Initialize Peripheral
	m_sHandle.Instance                 = SDMMC2;
	m_sHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
	m_sHandle.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
	m_sHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	m_sHandle.Init.BusWide             = SDMMC_BUS_WIDE_1B;
	m_sHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	m_sHandle.Init.ClockDiv            = SDMMC_TRANSFER_CLK_DIV;
	if (HAL_SD_Init(&m_sHandle) != HAL_OK) {
		m_eCardState = QAD_SDMMC_State_PeripheralError;
		return QA_Fail;
	}

	//Configure Bus Width
	if (HAL_SD_ConfigWideBusOperation(&m_sHandle, SDMMC_BUS_WIDE_4B) != HAL_OK) {
		m_eCardState = QAD_SDMMC_State_PeripheralError;
		return QA_Fail;
	}

	//Get Card Info
	if (imp_getCardInfo()) {
		m_eCardState = QAD_SDMMC_State_CardError;
		return QA_Fail;
	}

	//Set State
	m_eCardState = QAD_SDMMC_State_Mounted;

	//Return
	return QA_OK;
}


//QAD_SDMMC::imp_unmount
//QAD_SDMMC Mount Method
void QAD_SDMMC::imp_unmount(void) {

  //Set Card Info
  m_uSize       = 0;
  m_uSizeMB     = 0;
  m_uBlockCount = 0;
  m_uBlockSize  = 0;

  m_uType       = 0;
  m_uVersion    = 0;
  m_uClass      = 0;

  m_eCardState  = QAD_SDMMC_State_NoCard;

  //Deinit Peripheral
  HAL_SD_DeInit(&m_sHandle);

}


	//----------------------
	//----------------------
	//QAD_SDMMC Data Methods

//QAD_SDMMC::imp_readBlocks
//QAD_SDMMC Data Method
QA_Result QAD_SDMMC::imp_readBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount) {
	if (m_eCardState != QAD_SDMMC_State_Mounted) {
		return QA_Fail;
	}

	if (HAL_SD_ReadBlocks(&m_sHandle, pData, uAddr, uCount, m_uTimeout) != HAL_OK)
		return QA_Fail;

	//Return
	return QA_OK;
}


//QAD_SDMMC::imp_writeBlocks
//QAD_SDMMC Data Method
QA_Result QAD_SDMMC::imp_writeBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount) {
	if (m_eCardState != QAD_SDMMC_State_Mounted) {
		return QA_Fail;
	}

	if (HAL_SD_WriteBlocks(&m_sHandle, pData, uAddr, uCount, m_uTimeout) != HAL_OK)
		return QA_Fail;

	//Return
	return QA_OK;
}


//QAD_SDMMC::imp_erase
//QAD_SDMMC Data Method
QA_Result QAD_SDMMC::imp_erase(uint32_t uAddrStart, uint32_t uAddrEnd) {
	if (m_eCardState != QAD_SDMMC_State_Mounted) {
		return QA_Fail;
	}

	if (HAL_SD_Erase(&m_sHandle, uAddrStart, uAddrEnd) != HAL_OK)
		return QA_Fail;

	//Return
	return QA_OK;
}

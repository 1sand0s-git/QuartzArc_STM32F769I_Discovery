/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: SDMMC / SD Card Driver                                          */
/*   Filename: QAD_SDMMC.hpp                                               */
/*   Date: 10th February 2022                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_SDMMC_HPP_
#define __QAD_SDMMC_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//QAD_SDMMC_CardPresence
enum QAD_SDMMC_CardPresence : uint8_t {
	QAD_SDMMC_CardNotPresent = 0,
	QAD_SDMMC_CardPresent
};


//QAD_SDMMC_CardState
enum QAD_SDMMC_CardState : uint8_t {
	QAD_SDMMC_State_NoCard = 0,
	QAD_SDMMC_State_CardError,
	QAD_SDMMC_State_PeripheralError,
	QAD_SDMMC_State_Mounted
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//---------
//---------
//QAD_SDMMC
class QAD_SDMMC {
private:

	//Deinitization Mode to be used by imp_periphDeinit() method
	enum DeinitMode : uint8_t {
		DeinitPartial = 0,
		DeinitFull
	};


	//----------------
	//Driver Constants
	const uint32_t           m_uTimeout = 100;


	//-----------------
	//Driver State Data
	QA_InitState             m_eInitState;

	QAD_SDMMC_CardState      m_eCardState;


	//---------
	//Card Info
	uint64_t                 m_uSize;
	uint32_t                 m_uSizeMB;
	uint32_t                 m_uBlockCount;
	uint32_t                 m_uBlockSize;

	uint32_t                 m_uType;
	uint32_t                 m_uVersion;
	uint32_t                 m_uClass;


	//-----------------
	//Peripheral Handle
	SD_HandleTypeDef         m_sHandle;


	//------------
	//Constructors

	QAD_SDMMC() :
	  m_eInitState(QA_NotInitialized),
		m_eCardState(QAD_SDMMC_State_NoCard),
		m_uSize(0),
		m_uSizeMB(0),
		m_uBlockCount(0),
		m_uBlockSize(0),
		m_uType(0),
		m_uVersion(0),
		m_uClass(0) {}

public:

	//Delete copy constructor and assignment operator
	QAD_SDMMC(const QAD_SDMMC& other) = delete;
	QAD_SDMMC& operator=(const QAD_SDMMC& other) = delete;


	//-----------------
	//Singleton Methods
	static QAD_SDMMC& get(void) {
		static QAD_SDMMC instance;
		return instance;
	}


	//----------------------
	//Initialization Methods

	static QA_Result init(void) {
		return get().imp_init();
	}

	static void deinit(void) {
		get().imp_deinit();
	}


	//-----------------
	//Processing Method

	static QA_Result process(void) {
		return get().imp_process();
	}


	//------------
	//Card Methods

	static QAD_SDMMC_CardPresence getCardPresent(void) {
		return get().imp_getCardPresent();
	}

	static QAD_SDMMC_CardState getCardState(void) {
		return get().m_eCardState;
	}

	static uint64_t getCardSize(void) {
		return get().m_uSize;
	}

	static uint32_t getCardSizeMB(void) {
		return get().m_uSizeMB;
	}

	static uint32_t getBlockSize(void) {
		return get().m_uBlockSize;
	}

	static uint32_t getBlockCount(void) {
		return get().m_uBlockCount;
	}

	static uint32_t getCardType(void) {
		return get().m_uType;
	}

	static uint32_t getCardVersion(void) {
		return get().m_uVersion;
	}

	static uint32_t getCardClass(void) {
		return get().m_uClass;
	}


	//------------
	//Data Methods

	static QA_Result readBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount) {
		return get().imp_readBlocks(pData, uAddr, uCount);
	}

	static QA_Result writeBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount) {
		return get().imp_writeBlocks(pData, uAddr, uCount);
	}

	static QA_Result erase(uint32_t uAddrStart, uint32_t uAddrEnd) {
		return get().imp_erase(uAddrStart, uAddrEnd);
	}


private:

	//----------------------
	//Initialization Methods
	QA_Result imp_init(void);
	void imp_deinit(void);


	//---------------------------------
	//Peripheral Initialization Methods
	QA_Result imp_periphInit(void);
	void imp_periphDeinit(DeinitMode eMode);


	//------------------
	//Processing Methods
	QA_Result imp_process(void);


	//------------
	//Card Methods
	QAD_SDMMC_CardPresence imp_getCardPresent(void);
	QA_Result imp_getCardInfo(void);


	//-------------
	//Mount Methods
	QA_Result imp_mount(void);
	void imp_unmount(void);


	//------------
	//Data Methods
	QA_Result imp_readBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount);
	QA_Result imp_writeBlocks(uint8_t* pData, uint32_t uAddr, uint32_t uCount);
	QA_Result imp_erase(uint32_t uAddrStart, uint32_t uAddrEnd);

};


//Prevent Recursive Inclusion
#endif /* __QAD_SDMMC_HPP_ */

















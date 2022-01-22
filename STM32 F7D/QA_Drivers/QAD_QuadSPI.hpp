/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Quad SPI / MX25L512 Flash Driver                                */
/*   Filename: QAD_QuadSPI.hpp                                             */
/*   Date: 22nd January 2022                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_QUADSPI_HPP_
#define __QAD_QUADSPI_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


enum QAD_QuadSPI_Status : uint8_t {
	QAD_QuadSPI_Status_Ready = 0,
	QAD_QuadSPI_Status_Busy,
	QAD_QuadSPI_Status_Error
};


enum QAD_QuadSPI_MemoryMapped : uint8_t {
	QAD_QuadSPI_MemoryMapped_Disabled = 0,
	QAD_QuadSPI_MemoryMapped_Enabled
};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-----------
//QAD_QuadSPI
class QAD_QuadSPI {
private:

	//Deinitialization mode to be used by periphDeinit() method
	enum DeinitMode : uint8_t {
		DeinitPartial = 0,
		DeinitFull
	};


	//---------------
	//Flash Constants
	uint32_t         m_uFlashSize      = 0x400000; //64 MB
	uint32_t         m_uSectorSize     = 0x10000;  //64 kB
	uint32_t         m_uSubsectorSize  = 0x1000;   //4 kB
	uint32_t         m_uPageSize       = 0x100;    //256 bytes

	uint32_t         m_uSectorCount    = 1024;
	uint32_t         m_uSubsectorCount = 16384;
	uint32_t         m_uPageCount      = 262144;

	uint32_t         m_uMemoryMappedBaseAddr = 0x90000000; //Address space 0x90000000 - 0x93FFFFFF


	//-----------
	//Driver Data

	QA_InitState             m_eInitState;

	QAD_QuadSPI_MemoryMapped m_eMemoryMappedState;

	QSPI_HandleTypeDef       m_sHandle;


	//------------
	//Constructors

	QAD_QuadSPI() :
		m_eInitState(QA_NotInitialized),
		m_eMemoryMappedState(QAD_QuadSPI_MemoryMapped_Disabled) {}

public:

	//-----------------------------------------------
	//Delete copy constructor and assignment operator
	QAD_QuadSPI(const QAD_QuadSPI& other) = delete;
	QAD_QuadSPI& operator=(const QAD_QuadSPI& other) = delete;


	//-----------------
	//Singleton Methods
	static QAD_QuadSPI& get(void) {
		static QAD_QuadSPI instance;
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


	//--------------------------
	//Memory Mapped Mode Methods

	static QA_Result enterMemoryMapped(void) {
		return get().imp_enterMemoryMapped();
	}

	static QA_Result exitMemoryMapped(void) {
		return get().imp_exitMemoryMapped();
	}

	static QAD_QuadSPI_MemoryMapped getMemoryMappedState(void) {
		return get().m_eMemoryMappedState;
	}

	static uint32_t getMemoryMappedBaseAddress(void) {
		return get().m_uMemoryMappedBaseAddr;
	}


	//------------
	//Size Methods

	static uint32_t getFlashSize(void) {
		return get().m_uFlashSize;
	}

	static uint32_t getSectorSize(void) {
		return get().m_uSectorSize;
	}

	static uint32_t getSubsectorSize(void) {
		return get().m_uSubsectorSize;
	}

	static uint32_t getSectorCount(void) {
		return get().m_uSectorCount;
	}

	static uint32_t getSubsectorCount(void) {
		return get().m_uSubsectorCount;
	}


	//------------
	//Data Methods

	  //Read

	static QA_Result read(uint32_t uAddr, uint8_t* pData, uint32_t uSize) {
		return get().imp_read(uAddr, pData, uSize);
	}

	static QA_Result readSubsector(uint32_t uIdx, uint8_t* pData) {
		return get().imp_readSubsector(uIdx, pData);
	}

	static QA_Result readSector(uint32_t uIdx, uint8_t* pData) {
		return get().imp_readSector(uIdx, pData);
	}

	  //Write

	static QA_Result write(uint32_t uAddr, uint8_t* pData, uint32_t uSize) {
		return get().imp_write(uAddr, pData, uSize);
	}

	static QA_Result eraseAndWriteSubsector(uint32_t uIdx, uint8_t* pData) {
		return get().imp_eraseAndWriteSubsector(uIdx, pData);
	}

	static QA_Result eraseAndWriteSector(uint32_t uIdx, uint8_t* pData) {
		return get().imp_eraseAndWriteSector(uIdx, pData);
	}

	  //Erase

	static QA_Result eraseSubsectorAddr(uint32_t uAddr) {
		return get().imp_eraseSubsectorAddr(uAddr);
	}

	static QA_Result eraseSectorAddr(uint32_t uAddr) {
		return get().imp_eraseSectorAddr(uAddr);
	}

	static QA_Result eraseSubsector(uint32_t uIdx) {
		return get().imp_eraseSubsector(uIdx);
	}

	static QA_Result eraseSector(uint32_t uIdx) {
		return get().imp_eraseSector(uIdx);
	}

	static QA_Result eraseChip(void) {
		return get().imp_eraseChip();
	}

	  //Status

	static QAD_QuadSPI_Status getStatus(void) {
		return get().imp_getStatus();
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


	//--------------------------
	//Memory Mapped Mode Methods
	QA_Result imp_enterMemoryMapped(void);
	QA_Result imp_exitMemoryMapped(void);


	//------------
	//Data Methods
	QA_Result imp_read(uint32_t uAddr, uint8_t* pData, uint32_t uSize);
	QA_Result imp_readSubsector(uint32_t uIdx, uint8_t* pData);
	QA_Result imp_readSector(uint32_t uIdx, uint8_t* pData);

	QA_Result imp_write(uint32_t uAddr, uint8_t* pData, uint32_t uSize);
	QA_Result imp_eraseAndWriteSubsector(uint32_t uIdx, uint8_t* pData);
	QA_Result imp_eraseAndWriteSector(uint32_t uIdx, uint8_t* pData);

	QA_Result imp_eraseSubsectorAddr(uint32_t uAddr);
	QA_Result imp_eraseSectorAddr(uint32_t uAddr);
	QA_Result imp_eraseSubsector(uint32_t uIdx);
	QA_Result imp_eraseSector(uint32_t uIdx);
	QA_Result imp_eraseChip(void);

	QAD_QuadSPI_Status imp_getStatus(void);


	//------------
	//Tool Methods
	QA_Result imp_resetMemory(void);
	QA_Result imp_enterFourByteAddress(void);
	QA_Result imp_dummyCyclesCfg(void);
	QA_Result imp_enterMemoryQPI(void);
	QA_Result imp_exitMemoryQPI(void);
	QA_Result imp_outputDriveStrengthCfg(void);
	QA_Result imp_writeEnable(void);
	QA_Result imp_autoPollingMemReady(uint32_t uTimeout);

};



//Prevent Recursive Inclusion
#endif /* __QAD_QUADSPI_HPP_ */










/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device Base Class                                        */
/*   Filename: QAS_Serial_Dev_Base.hpp                                     */
/*   Date: 28th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_SERIAL_DEV_BASE_HPP_
#define __QAS_SERIAL_DEV_BASE_HPP_


//Includes
#include "setup.hpp"

#include <memory>
#include <string.h>

#include "QAT_FIFO.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------
//QAS_Serial_Dev_Base
class QAS_Serial_Dev_Base {
public:
	enum PeriphState : uint8_t {PeriphInactive = 0, PeriphActive};
	enum DataState : uint8_t {NoData = 0, HasData};
	enum DeviceType : uint8_t {DT_UART = 0, DT_File = 1, DT_Unknown};

public:

	std::unique_ptr<QAT_FIFOBuffer> m_pTXFIFO;
	std::unique_ptr<QAT_FIFOBuffer> m_pRXFIFO;

	QA_InitState m_eInitState;

	PeriphState m_eTXState;
	PeriphState m_eRXState;

	DeviceType  m_eDeviceType;

public:

	//Disable default constructor
	QAS_Serial_Dev_Base() = delete;

	//Constructor
	QAS_Serial_Dev_Base(uint16_t uTXFIFOSize, uint16_t uRXFIFOSize, DeviceType eDeviceType) :
		m_pTXFIFO(std::make_unique<QAT_FIFOBuffer>(uTXFIFOSize)),
		m_pRXFIFO(std::make_unique<QAT_FIFOBuffer>(uRXFIFOSize)),
		m_eInitState(QA_NotInitialized),
		m_eTXState(PeriphInactive),
		m_eRXState(PeriphInactive),
		m_eDeviceType(eDeviceType) {}


	//Initialization Methods
	QA_Result init(void* p);
	void deinit(void);

	//Handler Methods
	void handler(void* p);

	//Control Methods
	DeviceType getType(void);

	void rxStart(void);
	void rxStop(void);

	//Transmit Methods
	void txString(const char* str);
	void txStringCR(const char* str);
	void txCR(void);
	void txData(const uint8_t* pData, uint16_t uSize);

	//Receive Methods
	DataState rxHasData(void);

	uint8_t rxPop(void);
	QA_Result rxData(uint8_t* pData, uint16_t& uSize);

private:

	virtual QA_Result imp_init(void* p) = 0;
	virtual void imp_deinit(void) = 0;

	virtual void imp_handler(void* p) = 0;

	virtual void imp_txStart(void) = 0;
	virtual void imp_txStop(void) = 0;
	virtual void imp_rxStart(void) = 0;
	virtual void imp_rxStop(void) = 0;

};


//Prevent Recursive Inclusion
#endif /* __QAS_SERIAL_DEV_BASE_HPP_*/












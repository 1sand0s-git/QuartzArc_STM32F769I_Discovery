/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: FMC / SDRAM Driver                                              */
/*   Filename: QAD_FMC.hpp                                                 */
/*   Date: 30th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_FMC_HPP_
#define __QAD_FMC_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-----------------------
//QAD_FMC_32BITWORD_COUNT
//
//Defines the number of 32bit words within the 16 megabyte SDRAM module
#define QAD_FMC_32BITWORD_COUNT ((uint32_t)4194304)


//------------------
//QAD_FMC_BYTE_COUNT
//
//Defines the number of bytes within the 16 megabyte SDRAM module
#define QAD_FMC_BYTE_COUNT ((uint32_t)QAD_FMC_32BITWORD_COUNT * 4)


//------------------
//QAD_FMC_DataStruct
//
//Structure used to provide array based access to SDRAM memory
typedef struct {
	union {
	  __IO uint32_t words[QAD_FMC_32BITWORD_COUNT];
	  __IO uint8_t  bytes[QAD_FMC_BYTE_COUNT];
	};
} QAD_FMC_DataStruct;


//------------
//QAD_FMC_Data
//
//Pointer to QAD_FMC_DataStruct, located within SDRAM address space, starting at address 0xC0000000
#define QAD_FMC_Data ((QAD_FMC_DataStruct*)0xC0000000)



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------
//QAD_FMC
//
//Singleton class
//Driver class for flexible memory controller (FMC) peripheral, which is used to provide an interface to
//the 16 megabyte SDRAM module included on the STM32F769I-DISCO / STM32F769I-DISC1 development boards
//This is setup as a singleton class due to only one FMC and one SDRAM module being available on the
//STM32F769I Discovery board, meaning that potential conflicts are prevented by not allowing more than
//one instance of the class to exist.
class QAD_FMC {
private:

	//The following constants aid in setting the SDRAM IC's mode register
	const uint32_t m_uModeReg_BurstLength_1 = 0x00000000;       //Used to set a read burst length of 1 32bit word
	const uint32_t m_uModeReg_BurstLength_2 = 0x00000001;       //Used to set a read burst length of 2 32bit words
	const uint32_t m_uModeReg_BurstLength_4 = 0x00000002;       //Used to set a read burst length of 4 32bit words
	const uint32_t m_uModeReg_BurstLength_8 = 0x00000004;       //Used to set a read burst length of 8 32bit words

	const uint32_t m_uModeReg_BurstType_Seq = 0x00000000;       //Used to select a sequential burst type
	const uint32_t m_uModeReg_BurstType_Int = 0x00000008;       //Used to select an interleaved burst type

	const uint32_t m_uModeReg_CASLatency_2  = 0x00000020;       //Used to select a Column Address Select (CAS) latency of 2 clock cycles
	const uint32_t m_uModeReg_CASLatency_3  = 0x00000030;       //Used to select a Column Address Select (CAS) latency of 3 clock cycles

	const uint32_t m_uModeReg_OpMode_Std    = 0x00000000;       //Used to select a standard operating mode

	const uint32_t m_uModeReg_WriteBurst_Prog   = 0x00000000;   //Used to select data writes as using same burst length as set for data reads
	const uint32_t m_uModeReg_WriteBurst_Single = 0x00000200;   //Used to select data writes as using a burst length of 1 32bit word


	//const uint32_t m_uTimeout  = 0xFFFF;

	//m_uRefresh determines the required refresh rate for the SDRAM in order to maintain data integrity
	const uint32_t m_uRefresh  = 0x0603;

	static const uint32_t m_uBaseAddr   = 0xC0000000; //Base address of SDRAM
	static const uint32_t m_uSize       = 0x01000000; //Size in hex of SDRAM (16 megabytes)

	SDRAM_HandleTypeDef m_sHandle;  //Handle used by HAL functions to access the FMC peripheral
	QA_InitState        m_eState;   //Stores whether the FMC driver is currently initialized. Member of QA_InitState defined in setup.hpp


	//------------
	//Constructors

	//As this is a private method in a singleton class, this method will be called the first time the class's get() method is called
	QAD_FMC() :
	  m_eState(QA_NotInitialized) {}

public:

	//----------------------------------------------------------------------------------
	//Delete the copy constructor and assignment operator due to being a singleton class
	QAD_FMC(const QAD_FMC& other) = delete;
	QAD_FMC& operator=(const QAD_FMC& other) = delete;


	//-----------------
	//Singleton Methods
  //
	//Used to retrieve a reference to the singleton class
  static QAD_FMC& get(void) {
  	static QAD_FMC instance;
  	return instance;
  }


  //----------------------
  //Initialization Methods

  //NOTE: No deinitialization method is supplied due to SDRAM being seen as a primary system component
  //in this codebase and is required for operation of systems such as LCD among others

  //Used to initialize the FMC peripheral and SDRAM module
  static QA_Result init(void) {
  	return get().imp_init();
  }


  //------------
  //Data Methods

  //Used to retrueve the current state of the FMC driver
  //Returns QA_OK if initialization successful, or QA_Fail if initialization failed.
  static QA_InitState getState(void) {
  	return get().m_eState;
  }

  //Returns base address of SDRAM
  static uint32_t getBaseAddr(void) {
  	return m_uBaseAddr;
  }

  //Returns size in bytes of SDRAM
  static uint32_t getSize(void) {
  	return m_uSize;
  }


  //----------------------
  //Memory Testing Methods

  //Used to perform a memory test of the SDRAM module
  //Performs a full read and write test of all 16 megabytes of the SDRAM
  //Returns QA_OK if memory test is successful, or QA_Fail if not successful
  static QA_Result test(void) {
  	return get().imp_test();
  }

private:

  //NOTE: See QAD_FMC.cpp for details of the following functions

  //----------------------
  //Initialization Methods

  QA_Result imp_init();


  //----------------------
  //Memory Testing Methods

  QA_Result imp_test(void);


  //----------------------------
  //SDRAM Module Command Methods

  void cmd(FMC_SDRAM_CommandTypeDef& pCmd);
};


//Prevent Recursive Inclusion
#endif /* __QAD_FMC_HPP_ */

/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: I2C Driver                                                      */
/*   Filename: QAD_I2C.hpp                                                 */
/*   Date: 27th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_I2C_HPP_
#define __QAD_I2C_HPP_

//Includes
#include "setup.hpp"

#include "QAD_I2CMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------------------
//QAD_I2C_AddrssingMode
//
//Enum used to define if 7-bit or 10-bit addressing mode is to be used
enum QAD_I2C_AddressingMode : uint32_t {
	QAD_I2C_AddressingMode_7Bit  = I2C_ADDRESSINGMODE_7BIT,
	QAD_I2C_AddressingMode_10Bit = I2C_ADDRESSINGMODE_10BIT
};


//--------------------------
//QAD_I2C_DualAddressingMode
//
//Enum used to define if peripheral is to operate in single or dual addressing mode
enum QAD_I2C_DualAddressingMode : uint32_t {
	QAD_I2C_DualAddressingMode_Disable  = I2C_DUALADDRESS_DISABLE,
	QAD_I2C_DualAddressingMode_Enable   = I2C_DUALADDRESS_ENABLE
};


//---------------------------------
//QAD_I2C_GeneralCallMode
//
//Enum used to define if general call addressing mode is to be enabled
enum QAD_I2C_GeneralCallMode : uint32_t {
  QAD_I2C_GeneralCallMode_Disable  = I2C_GENERALCALL_DISABLE,
  QAD_I2C_GeneralCallMode_Enable   = I2C_GENERALCALL_ENABLE
};


//---------------------------------
//QAD_I2C_NoStretchMode
//
//Enum used to define if clock stretching is disabled (only when peripheral is in slave mode)
enum QAD_I2C_NoStretchMode : uint32_t {
	QAD_I2C_NoStretchMode_Disable  = I2C_NOSTRETCH_DISABLE,
	QAD_I2C_NoStretchMode_Enable   = I2C_NOSTRETCH_ENABLE
};


//------------------------------------------
//------------------------------------------
//------------------------------------------

//------------------
//QAD_I2C_InitStruct
//
//This structure is used to be able to create the QAD_I2C driver class
typedef struct {

	QAD_I2C_Periph              eI2C;                 //I2C peripheral to be used (member of QAD_I2C_Periph, as defined in QAD_I2CMgr.hpp)
	uint8_t                     uIRQPriority_Event;   //IRQ Priority to be used for Event interrupts
	uint8_t                     uIRQPriority_Error;   //IRQ Priority to be used for Error interrupts

	uint32_t                    uTiming;              //Timing value to determine clock speed
                                                    //Low Speed mode is up to 100kHz
	                                                  //High Speed mode is up to 400kHz

	QAD_I2C_AddressingMode      eAddressingMode;      //Addressing mode to be used (member of QAD_I2C_AddressingMode enum)
	QAD_I2C_DualAddressingMode  eDualAddressingMode;  //Enable/disable dual addressing mode (member of QAD_I2C_DualAddressingMode enum)
	QAD_I2C_GeneralCallMode     eGeneralCallMode;     //Enable/disable general call adressing mode (member of QAD_I2C_GeneralCallMode enum)
	QAD_I2C_NoStretchMode       eNoStretchMode;       //Enable/disable no-clock-stretching mode (member of QAD_I2C_NoStretchMode enum)

	uint16_t                    uOwnAddress1;         //Primary I2C peripheral address
	uint16_t                    uOwnAddress2;         //Secondary I2C peripheral address (used in dual-addressing mode)

	GPIO_TypeDef*               pSCL_GPIO;            //GPIO port to be used for clock pin
	uint16_t                    uSCL_Pin;             //Pin number to be used for clock pin
	uint8_t                     uSCL_AF;              //Alternate function to be used for clock pin

	GPIO_TypeDef*               pSDA_GPIO;            //GPIO port to be used for the data pin
	uint16_t                    uSDA_Pin;             //Pin number to be used for the data pin
	uint8_t                     uSDA_AF;              //Alternate function to be used for data pin

} QAD_I2C_InitStruct;


//------------------------------------------
//------------------------------------------
//------------------------------------------

//-------
//QAD_I2C
//
//Driver class to access I2C peripherals
class QAD_I2C {
private:

	//Deinitialization mode to be used by periphDeinit() method
	enum DeinitMode : uint8_t {
		DeinitPartial = 0,        //Only to be used for partial deinitialization upon initialization failure in periphInit() method
		DeinitFull                //Used for full driver deinitialization when driver is in a fully initialized state
	};

	const uint16_t              m_uTimeout = 1000;       //Stores the timeout value to be used when transmitting/receiving

	const uint8_t               m_uMemAddrSize8Bit  = 1; //Stores the number of bytes taken up by a Memory/Register 8bit address
	const uint8_t               m_uMemAddrSize16Bit = 2; //Stores the number of bytes taken up by a Memory/Register 16bit address

	QA_InitState                m_eInitState;            //Stores whether the driver is currently initialized. Member of QA_InitState enum defined in setup.hpp
	QA_ActiveState              m_eState;

	QAD_I2C_Periph              m_eI2C;                  //Stores the particular I2C peripheral being used by the driver
	uint8_t                     m_uIRQPriority_Event;    //Stores the IRQ priority to be used by Event interrupts
	uint8_t                     m_uIRQPriority_Error;    //Stores the IRQ priority to be used by Error interrupts

	uint32_t                    m_uTiming;               //Stores the timing value used to determine the clock speed

	QAD_I2C_AddressingMode      m_eAddressingMode;       //Stores whether 7-bit or 10-bit addressing is being used
	QAD_I2C_DualAddressingMode  m_eDualAddressingMode;   //Stores whether single or dual addressing mode is being used
	QAD_I2C_GeneralCallMode     m_eGeneralCallMode;      //Stores whether general call addressing mode is being used
	QAD_I2C_NoStretchMode       m_eNoStretchMode;        //Stores whether clock stretching is to be disabled or not

	uint16_t                    m_uOwnAddress1;          //Primary peripheral address (used in single addressing mode)
	uint16_t                    m_uOwnAddress2;          //Secondary peripheral address (used in dual-addressing mode)

	GPIO_TypeDef*               m_pSCL_GPIO;             //GPIO port used by the clock pin
	uint16_t                    m_uSCL_Pin;              //Pin number used by the clock pin
	uint8_t                     m_uSCL_AF;               //Alternate function used by the clock pin

	GPIO_TypeDef*               m_pSDA_GPIO;             //GPIO port used by the data pin
	uint16_t                    m_uSDA_Pin;              //Pin number used by the data pin
	uint8_t                     m_uSDA_AF;               //Alternate function used by the data pin

	IRQn_Type                   m_eIRQ_Event;            //The IRQ used by the I2C peripheral when Event interrupts are triggered
	IRQn_Type                   m_eIRQ_Error;            //The IRQ used by the I2C peripheral when Error interrupts are triggered
	                                                     //These are members of IRQn_Type defined in stm32f4769xx.h

	I2C_HandleTypeDef           m_sHandle;               //Handle used by HAL functions to access I2C peripheral (defined in stm32f7xx_hal_i2c.h)

public:

		//--------------------------
		//Constructors / Destructors

	QAD_I2C() = delete;                      //Delete the default class constructor, as we need an initialization structure to be provided on class creation.

	QAD_I2C(QAD_I2C_InitStruct& sInit) :     //The class constructor to be used, which has a reference to an initialization structure passed to it
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_eI2C(sInit.eI2C),
		m_uIRQPriority_Event(sInit.uIRQPriority_Event),
		m_uIRQPriority_Error(sInit.uIRQPriority_Error),
		m_uTiming(sInit.uTiming),
		m_eAddressingMode(sInit.eAddressingMode),
		m_eDualAddressingMode(sInit.eDualAddressingMode),
		m_eGeneralCallMode(sInit.eGeneralCallMode),
		m_eNoStretchMode(sInit.eNoStretchMode),
		m_uOwnAddress1(sInit.uOwnAddress1),
		m_uOwnAddress2(sInit.uOwnAddress2),
		m_pSCL_GPIO(sInit.pSCL_GPIO),
		m_uSCL_Pin(sInit.uSCL_Pin),
		m_uSCL_AF(sInit.uSCL_AF),
		m_pSDA_GPIO(sInit.pSDA_GPIO),
		m_uSDA_Pin(sInit.uSDA_Pin),
		m_uSDA_AF(sInit.uSDA_AF),
		m_eIRQ_Event(I2C1_EV_IRQn),
		m_eIRQ_Error(I2C1_ER_IRQn),
		m_sHandle({0}) {}


	~QAD_I2C() {                             //Destructor to make sure peripheral is made inactive and deinitialized upon class destruction

		//Stop Driver if currently active
		if (m_eState)
			stop();

		if (m_eInitState)
			deinit();
	}


	  //NOTE: See QAD_I2C.cpp for details of the following methods


		//----------------------
		//Initialization Methods

	QA_Result init(void);
	void deinit(void);

	QA_InitState getInitState(void);
	I2C_HandleTypeDef& getHandle(void);


		//---------------
		//Control Methods

	void start(void);
	void stop(void);
	QA_ActiveState getState(void);


		//------------------
		//Transceive Methods

	QA_Result write8Bit(uint16_t uAddr, uint8_t uReg, uint8_t uValue);
	QA_Result write16Bit(uint16_t uAddr, uint16_t uReg, uint16_t uValue);
	QA_Result writeMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength);

	QA_Result read8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pValue);
	QA_Result read16Bit(uint16_t uAddr, uint16_t uReg, uint16_t* pValue);
	QA_Result readMultiple8Bit(uint16_t uAddr, uint8_t uReg, uint8_t* pData, uint16_t uLength);


private:


		//----------------------
		//Initialization Methods

	QA_Result periphInit(void);
	void periphDeinit(DeinitMode eDeinitMode);


		//------------------
		//Transceive Methods

	QA_Result write(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength);
	QA_Result read(uint16_t uAddr, uint16_t uReg, uint16_t uMemAddress, uint8_t* pData, uint16_t uLength);

};


//Prevent Recursive Inclusion
#endif /* __QAD_I2C_HPP_ */

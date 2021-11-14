/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: Encoder Driver                                                  */
/*   Filename: QAD_Encoder.hpp                                             */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_ENCODER_HPP_
#define __QAD_ENCODER_HPP_


//Includes
#include "setup.hpp"

#include "QAD_TimerMgr.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------------
//QAD_EncoderMode
//
//Used with QAD_Encoder driver class to determine type of encoder output data
enum QAD_EncoderMode : uint8_t {
	QAD_EncoderMode_Linear = 0,  //Linear mode - each "click" of the encoder results in a change in value of +/- 1
			                         //Best used when needing to scroll through a small number of values/settings

	QAD_EncoderMode_Exp          //Exponential mode - speed at which encoder is rotated results in smaller or larger value changes
	                             //Best used when needing to scroll through a large number of values/settings
};


//----------------------
//QAD_Encoder_InitStruct
//
//This structure is used to be able to create the QAD_Encoder driver class
typedef struct {

	GPIO_TypeDef*     pCh1_GPIO; //GPIO port to be used for channel 1 of the encoder's quadrature signal
	uint16_t          uCh1_Pin;  //Pin number to be used for channel 1 of the encoder's quadrature signal
	uint8_t           uCh1_AF;   //Alternate function to be used to link GPIO pin to Timer peripheral

	GPIO_TypeDef*     pCh2_GPIO; //GPIO port to be used for channel 2 of the encoder's quadrature signal
	uint16_t          uCh2_Pin;  //Pin number to be used for channel 2 of the encoder's quadrature signal
	uint8_t           uCh2_AF;   //Alternate function to be used to link GPIO pin to Timer peripheral

	QAD_Timer_Periph  eTimer;    //Timer peripheral to be used (member of QAD_Timer_Periph, as defined in QAD_TimerMgr.hpp)
	                             //Note that the selected timer must have rotary encoder mode support

	QAD_EncoderMode   eMode;     //Encoder output data mode (QAD_EncoderMode_Linear or QAD_EncoderMode_Exp)

} QAD_Encoder_InitStruct;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-----------
//QAD_Encoder
//
//Driver class for using timer peripheral in rotary encoder mode
class QAD_Encoder {
private:

	//Deinitialization mode to be used by periphDeinit() method
	enum DeinitMode : uint8_t {
		DeinitPartial = 0,    //Only to be used for partial deinitialization upon initialization failure in periphInit() method
		DeinitFull            //Used for full driver deinitialization when driver is in a fully initialized state
	};

	const uint16_t     m_uMaxVal = 1024;  //Value used for timer period and encoder value calculation

	GPIO_TypeDef*      m_pCh1_GPIO;       //GPIO port to be used for channel 1 of the encoder's quadrature signal
	uint16_t           m_uCh1_Pin;        //Pin number to be used for channel 1 of the encoder's quadrature signal
	uint8_t            m_uCh1_AF;         //Alternate function to be used to link GPIO pin to timer peripheral

	GPIO_TypeDef*      m_pCh2_GPIO;       //GPIO port to be used for channel 2 of the encoder's quadrature signal
	uint16_t           m_uCh2_Pin;        //Pin number to be used for channel 2 of the encoder's quadrature signal
	uint8_t            m_uCh2_AF;         //Alternate function to be used to link GPIO pin to timer peripheral

	QAD_Timer_Periph   m_eTimer;          //Stores the particular timer peripheral to be used by the driver.
	                                      //Member of QAD_Timer_Periph as defined in QAD_TimerMgr.hpp

	TIM_HandleTypeDef  m_sHandle;         //Handle used by HAL functions to access Timer peripheral

	QA_InitState       m_eInitState;      //Stores whether the driver is currently initialized. Member of QA_InitState enum defined in setup.hpp

	QA_ActiveState     m_eState;          //Stores whether the driver is currently active. Member of QA_ActiveState enum defined in setup.hpp

	uint16_t           m_uValueOld;       //Stores the previous timer counter value
	uint16_t           m_uValueNew;       //Stores the current timer counter value
	int16_t            m_iValue;          //Stores the difference in between the previous and current timer counter values
	uint16_t           m_uAccel;          //Stores the acceleration value of the encoder (how fast the encoder is being rotated)

	QAD_EncoderMode    m_eMode;           //Stores whether the encoder data output is in linear or exponential mode
	                                      //See QAD_EncoderMode definition for further details

public:

	//--------------------------
	//Constructors / Destructors

  QAD_Encoder() = delete;                        //Delete the default class constructor, as we need an initialization structor to be provided on class creation

  QAD_Encoder(QAD_Encoder_InitStruct& sInit) :  //The class constructor to be used, which has a reference to an initialization structure passed to it
  	m_pCh1_GPIO(sInit.pCh1_GPIO),
		m_uCh1_Pin(sInit.uCh1_Pin),
		m_uCh1_AF(sInit.uCh1_AF),
		m_pCh2_GPIO(sInit.pCh2_GPIO),
		m_uCh2_Pin(sInit.uCh2_Pin),
		m_uCh2_AF(sInit.uCh2_AF),
		m_eTimer(sInit.eTimer),
		m_sHandle({0}),
		m_eInitState(QA_NotInitialized),
		m_eState(QA_Inactive),
		m_uValueOld(0),
		m_uValueNew(0),
		m_iValue(0),
		m_uAccel(0),
		m_eMode(sInit.eMode) {};

  ~QAD_Encoder() {       //Destructor to make sure peripheral is mode inactive and deinitialized upon class destruction

  	//Stop encoder driver if currently active
  	if (m_eState)
  		stop();

  	//Deinitialize encoder driver if currently initialized
  	if (m_eInitState)
  		deinit();
  }


  //NOTE: See QAD_Encoder.cpp for details of the following functions

  //----------------------
  //Initialization Methods

  QA_Result init(void);
  void deinit(void);


  //---------------
  //Control Methods

  void start(void);
  void stop(void);

  void update(uint32_t uTicks);

  QA_ActiveState getState(void);
  int16_t getValue(void);
  uint16_t getAccel(void);

  void setMode(QAD_EncoderMode eMode);
  QAD_EncoderMode getMode(void);

private:

  //----------------------
  //Initialization Methods

  QA_Result periphInit(void);
  void periphDeinit(DeinitMode eDeinitMode);


  //------------
  //Tool Methods

  void clearData(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_ENCODER_HPP_ */

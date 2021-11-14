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

//Includes
#include "QAD_Encoder.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //----------------------------------
  //----------------------------------
  //QAD_Encoder Initialization Methods

//QAD_Encoder::init
//QAD_Encoder Initialization Method
//
//Used to initialize the encoder driver
//Returns QA_OK if initialization successful, or an error if not successful (a member of QA_Result as defined in setup.hpp)
QA_Result QAD_Encoder::init(void) {

	//Check if selected Timer peripheral supports rotary encoder mode
  if (!QAD_TimerMgr::getEncoder(m_eTimer)) {
  	return QA_Error_PeriphNotSupported;
  }

  //Check if selected Timer peripheral is currently available
  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  //Register Timer peripheral as now being in use
  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_Encoder);

  //Initialize the Timer peripheral
  QA_Result eRes = periphInit();

  //If initialization failed then deregister the timer peripheral
  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);

  //Return initialization result
  return eRes;
}


//QAD_Encoder::deinit
//QAD_Encoder Initialization Method
//
//Used to deinitialize the encoder driver
void QAD_Encoder::deinit(void) {

	//Return if encoder driver is not currently initialized
  if (!m_eInitState)
  	return;

  //Deinitialize encoder driver
  periphDeinit(DeinitFull);

  //Deregister the Timer peripheral
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //---------------------------
  //---------------------------
  //QAD_Encoder Control Methods

//QAD_Encoder::start
//QAD_Encoder Control Method
//
//This method is used to start the timer peripheral in encoder mode
void QAD_Encoder::start(void) {

	//Check encoder driver is initialized and is not currently active
  if ((m_eInitState) && (!m_eState)) {

  	//Clear encoder/counter data
  	clearData();

  	//Start Timer peripheral in encoder mode
  	HAL_TIM_Encoder_Start(&m_sHandle, TIM_CHANNEL_ALL);

  	//Set driver state to active
  	m_eState = QA_Active;
  }
}


//QAD_Encoder::stop
//QAD_Encoder Control Method
//
//This method is used to stop the timer peripheral
void QAD_Encoder::stop(void) {

	//Check encoder driver is initialized and is currently active
  if ((m_eInitState) && (m_eState)) {

  	//Stop Timer peripheral
  	HAL_TIM_Encoder_Stop(&m_sHandle, TIM_CHANNEL_ALL);

  	//Set driver state to inactive
  	m_eState = QA_Inactive;
  }
}


//QAD_Encoder::update
//QAD_Encoder Control Method
//
//This method is used to update the encoder/counter data and should be called at a regular rate
//This method needs to be called prior to calling the getValue() method in order for getValue() to return
//the most recent encoder value.
//
//uTicks - The time (in milliseconds) that has passed since this method was last called
//         This is used to be able to calculate the encoder acceleration value
void QAD_Encoder::update(uint32_t uTicks) {

	//Check that encoder driver is active
	if (m_eState) {

		m_uValueOld = m_uValueNew;                        //Store previous current value into old value
		m_uValueNew = __HAL_TIM_GET_COUNTER(&m_sHandle);  //Update current value from Timer's counter register

		//The following code is used to determine if the encoder has been moved in a clockwise or counter-clockwise rotation,
		//while also taking into account whether the counter register value has wrapped around
		uint16_t uDiffA;
		uint16_t uDiffB;
		bool bValComp = (m_uValueNew < m_uValueOld);
		if (bValComp) {
			uDiffA = m_uValueOld - m_uValueNew;
			uDiffB = m_uValueNew + (m_uMaxVal - m_uValueOld);
		} else {
			uDiffA = m_uValueNew - m_uValueOld;
			uDiffB = m_uValueOld + (m_uMaxVal - m_uValueNew);
		}
		uint16_t uDiff = (uDiffA < uDiffB) ? uDiffA : uDiffB;
		m_iValue += (bValComp ? 0-uDiff : uDiff);

		//Calculate encoder acceleration value
		m_uAccel = uDiff * 1000 / uTicks;
	}
}


//QAD_Encoder::getState
//QAD_Encoder Control Method
//
//Returns the current state of the encoder driver (QA_Active or QA_Inactive)
QA_ActiveState QAD_Encoder::getState(void) {
  return m_eState;
}


//QAD_Encoder::getValue
//QAD_Encoder Control Method
//
//Returns the current change in encoder value
//Takes into account if the encoder mode is set to linear (QAD_EncoderMode_Linear) or exponential (QAD_EncoderMode_Exp)
//NOTE: the update() method needs to be called prior to getValue() in order to obtain the most recent value
//
//Returns a positive number if encoder is turned clockwise, or a negative number if encoder is turned anti-clockwise
//If you get opposite results than this then try swapping the two quadrature signal wires
int16_t QAD_Encoder::getValue(void) {

	//Check that driver is currently active
  if (m_eInitState) {

  	//As each click of the encoder generates four quadrature signal 'edges', a single click of the encoder will change the timer
  	//counter register value by a value of +/- 4. The following code is used to take this into account.
  	int16_t iOutVal = m_iValue / 4;
  	m_iValue = m_iValue % 4;

  	//Return the current value based on the currently selected encoder mode
  	return (m_eMode) ? (iOutVal * iOutVal * iOutVal) : iOutVal;
  }

  //Return a value of 0 if the driver is currently inactive
  return 0;
}


//QAD_Encoder::getAccel
//QAD_Encoder Control Method
//
//Returns the current acceleration value of the encoder (how fast the encoder is being rotated)
//This will return a positive value regardless of whether the encoder is being rotated clockwise or anti-clockwise
//NOTE: the update() method needs to be called prior to getAccel() in order to obtain the most recent value
uint16_t QAD_Encoder::getAccel(void) {
  return (m_eInitState) ? m_uAccel : 0;
}


//QAD_Encoder::setMode
//QAD_Encoder Control Method
//
//Sets the current encoder mode
//eMode - A member of QAD_EncoderMode as defined in QAD_Encoder.hpp
void QAD_Encoder::setMode(QAD_EncoderMode eMode) {
  m_eMode = eMode;
}


//QAD_Encoder::getMode
//QAD_Encoder Control Method
//
//Returns the current encoder mode. A member of QAD_EncoderMode as defined in QAD_Encoder.hpp
QAD_EncoderMode QAD_Encoder::getMode(void) {
  return m_eMode;
}


  //------------------------------------------
  //------------------------------------------
  //QAD_Encoder Private Initialization Methods

//QAD_Encoder::periphInit
//QAD_Encoder Private Initialization Method
//
//Used to initialize the GPIOs, timer peripheral clock, the timer peripheral itself in encoder mode
//In the case of a failed initialization, a partial deinitialization will be performed to make sure that the periphal, clock and GPIOs
//are all in the uninitialized state
//Returns QA_OK if successful, or QA_Fail if initialization fails
QA_Result QAD_Encoder::periphInit(void) {
  GPIO_InitTypeDef GPIO_Init = {0};

  //Encoder Quadrature Channel 1 GPIO Initialization
  GPIO_Init.Pin       = m_uCh1_Pin;            //Set pin number
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;       //Set pin as Alternate Function in push/pull mode
  GPIO_Init.Pull      = GPIO_NOPULL;           //Disable pull-up and pull-down resistors
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;  //Unused due to pin being used as peripheral input
  GPIO_Init.Alternate = m_uCh1_AF;             //Set alternate function to suit required timer peripheral
  HAL_GPIO_Init(m_pCh1_GPIO, &GPIO_Init);

  //Encoder Quadrature Channel 2 GPIO Initialization
  GPIO_Init.Pin       = m_uCh2_Pin;            //Set pin number
  GPIO_Init.Alternate = m_uCh2_AF;             //Set alternate function to suit required timer peripheral
  HAL_GPIO_Init(m_pCh2_GPIO, &GPIO_Init);

  //Enable Timer Clock
  QAD_TimerMgr::enableClock(m_eTimer);

  //Initialize Timer
  m_sHandle.Instance               = QAD_TimerMgr::getInstance(m_eTimer);  //Set instance for required timer peripheral
  m_sHandle.Init.Prescaler         = 0;                                    //Prescaler is unused as timer counter is clocked by the encoder's quadrature signal
  m_sHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;                   //Counter mode is unused as timer counter is updated based on quadrature signal
  m_sHandle.Init.Period            = m_uMaxVal;                            //Set timer period
  m_sHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;               //Unused
  m_sHandle.Init.RepetitionCounter = 0x0;                                  //
  m_sHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;        //Enable preload of the timer's auto-reload register

  TIM_Encoder_InitTypeDef ENC_Init = {0};                                  //Fill out encoder mode initialization structure
  ENC_Init.EncoderMode   = TIM_ENCODERMODE_TI12;                           //Set mode to counting on both TI1 and TI2
  ENC_Init.IC1Polarity   = TIM_ICPOLARITY_RISING;                          //Set IC1 polarity to rising edge
  ENC_Init.IC1Selection  = TIM_ICSELECTION_DIRECTTI;                       //Set IC1 to direct connection mode
  ENC_Init.IC1Prescaler  = TIM_ICPSC_DIV1;                                 //IC1 capture performed on each edge
  ENC_Init.IC1Filter     = 0x0;                                            //Disable IC1 input capture filter
  ENC_Init.IC2Polarity   = TIM_ICPOLARITY_RISING;                          //Set IC2 polarity to rising edge
  ENC_Init.IC2Selection  = TIM_ICSELECTION_DIRECTTI;                       //Set IC2 to direct connection mode
  ENC_Init.IC2Prescaler  = TIM_ICPSC_DIV1;                                 //IC2 capture performed on each edge
  ENC_Init.IC2Filter     = 0x0;                                            //Disable IC2 input capture filter

  //Initialize Timer in encoder mode, performing a partial deinitialization if the initialization fails
  if (HAL_TIM_Encoder_Init(&m_sHandle, &ENC_Init) != HAL_OK) {
  	periphDeinit(DeinitPartial);
  	return QA_Fail;
  }

  //Set Driver States
  m_eInitState = QA_Initialized; //Set driver state as initialized
  m_eState     = QA_Inactive;    //Set driver as currently inactive

  //Return
  return QA_OK;
}


//QAD_Encoder::periphDeinit
//QAD_Encoder Private Initialization Method
//
//Used to deinitialize the GPIOs, timer peripheral clock and the peripheral itself
//eDeinitMode - Set to DeinitPartial to perform a partial deinitialization (only to be used by periphInit() method
//              in a case where peripheral initialization has failed
//            - Set to DeinitFull to perform a full deinitialization in a case where the driver is fully initialized
void QAD_Encoder::periphDeinit(QAD_Encoder::DeinitMode eDeinitMode) {

	//Check if full deinitialization is required
	if (eDeinitMode) {

		//Deinitialize Timer Peripheral
		HAL_TIM_Encoder_DeInit(&m_sHandle);
	}

	//Disable Timer Clock
	QAD_TimerMgr::disableClock(m_eTimer);

	//Deinit GPIOs
	HAL_GPIO_DeInit(m_pCh1_GPIO, m_uCh1_Pin);
	HAL_GPIO_DeInit(m_pCh2_GPIO, m_uCh2_Pin);

	//Set States
	m_eState     = QA_Inactive;        //Set driver as currently inactive
	m_eInitState = QA_NotInitialized;  //Set driver state as not initialized
}


  //--------------------------------
  //--------------------------------
  //QAD_Encoder Private Tool Methods

//QAD_Encoder::clearData
//QAD_Encoder Private Tool Method
//
//Used to clear the encoder/counter data, as well as clearing the Timer's counter register to 0
void QAD_Encoder::clearData(void) {
  m_uValueOld = 0;
  m_uValueNew = 0;
  m_iValue    = 0;
  m_uAccel    = 0;
  __HAL_TIM_SET_COUNTER(&m_sHandle, 0);
}

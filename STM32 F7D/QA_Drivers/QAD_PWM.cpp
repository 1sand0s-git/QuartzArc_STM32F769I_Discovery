/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: PWM Driver                                                      */
/*   Filename: QAD_PWM.cpp                                                 */
/*   Date: 14th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_PWM.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //------------------------------
  //------------------------------
  //QAD_PWM Initialization Methods

//QAD_PWM::init
//QAD_PWM Initialization Method
//
//Used to initialize the PWM driver
//Returns QA_OK if initialization successful, or an error if not successful (a member of QA_Result as defined in setup.hpp)
QA_Result QAD_PWM::init(void) {

	//Check if selected Timer peripheral is currently available
  if (QAD_TimerMgr::getState(m_eTimer))
  	return QA_Error_PeriphBusy;

  //Register Timer peripheral as now being in use
  QAD_TimerMgr::registerTimer(m_eTimer, QAD_Timer_InUse_PWM);

  //Initialize the Timer peripheral
  QA_Result eRes = periphInit();

  //If initialization failed then deregister the Timer peripheral
  if (eRes)
  	QAD_TimerMgr::deregisterTimer(m_eTimer);

  //Return initialization result
  return eRes;
}


//QAD_PWM::deinit
//QAD_PWM Initialization Method
//
//Used to deinitialize the PWM driver
void QAD_PWM::deinit(void) {

	//Return if PWM driver is not currently initialized
  if (!m_eInitState)
  	return;

  //Deinitialize PWM driver
  periphDeinit(DeinitFull);

  //Deregister Timer peripheral
  QAD_TimerMgr::deregisterTimer(m_eTimer);
}


  //-----------------------
  //-----------------------
  //QAD_PWM Control Methods

//QAD_PWM::start
//QAD_PWM Control Method
//
//Starts the PWM driver
void QAD_PWM::start(void) {

	//Iterate through the number of channels supported by the specific timer peripheral
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {

		//If channel is set to active then start PWM on that channel
		if (m_sChannels[i].eActive)
			HAL_TIM_PWM_Start(&m_sHandle, m_uChannelSelect[i]);
	}

	//Set PWM driver state to active
	m_eState = QA_Active;
}


//QAD_PWM::stop
//QAD_PWM Control Method
//
//Stops the PWM driver
void QAD_PWM::stop(void) {

	//Iterate through the number of channels supported by the specific timer peripheral
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {

		//If channel is currently active then stop PWM on that channel
		if (m_sChannels[i].eActive)
			HAL_TIM_PWM_Stop(&m_sHandle, m_uChannelSelect[i]);
	}

	//Set PWM driver state to inactive
	m_eState = QA_Inactive;
}


//QAD_PWM::setPWMVal
//QAD_PWM Control Method
//
//Sets the current PWM value for a specific channel
//eChannel - The PWM channel to set the value for. A member of QAD_PWM_Channel as defined in QAD_PWM.hpp
//uVal     - The PWM value to set. This value should not be larger than the timer period set within the driver initialization structure
void QAD_PWM::setPWMVal(QAD_PWM_Channel eChannel, uint16_t uVal) {

	//Return if the selected channel is higher than the number of channels supported by the selected timer peripheral
  if (eChannel >= QAD_TimerMgr::getChannels(m_eTimer))
  	return;

  //Set new PWM value to compare register for selected channel of timer peripheral
  __HAL_TIM_SET_COMPARE(&m_sHandle, m_uChannelSelect[eChannel], uVal);
}


  //--------------------------------------
  //--------------------------------------
  //QAD_PWM Private Initialization Methods


//QAD_PWM::periphInit
//QAD_PWM Private Initialization Method
//
//Used to initialize the GPIOs, timer peripheral clock, the timer peripheral itself and the respective PWM channels
//In the case of a failed initialization, a partial deinitialization will be performed to make sure the peripheral and clock
//are all in the uninitialized state.
//Returns QA_OK if successful, or QA_Fail if initialization fails
QA_Result QAD_PWM::periphInit(void) {

	//Init GPIOs
	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Mode     = GPIO_MODE_AF_PP;      //Set pin to Alternate Function - Push/Pull mode
	GPIO_Init.Pull     = GPIO_NOPULL;          //Disable pull-up and pull-down resistors
	GPIO_Init.Speed    = GPIO_SPEED_FREQ_HIGH; //Set pin output speed to high

	//Iterate through specific GPIOs per channel and initialize each in turn
	//Will only iterate through the number of channels supported by the specific timer peripheral
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {

		//If channel is set to be active then initialize GPIO pin
		if (m_sChannels[i].eActive) {
			GPIO_Init.Pin         = m_sChannels[i].uPin; //Set pin number
			GPIO_Init.Alternate   = m_sChannels[i].uAF;  //Set alternate function to suit required timer peripheral
			HAL_GPIO_Init(m_sChannels[i].pGPIO, &GPIO_Init);
		}
	}

	//Enable Timer Clock
	QAD_TimerMgr::enableClock(m_eTimer);

	//Init Timer PWM Mode
	m_sHandle.Instance                     = QAD_TimerMgr::getInstance(m_eTimer);  //Set instance for required timer peripheral
	m_sHandle.Init.Prescaler               = m_uPrescaler;                         //Set timer prescaler
	m_sHandle.Init.Period                  = m_uPeriod;                            //Set timer counter period
	m_sHandle.Init.CounterMode             = TIM_COUNTERMODE_UP;                   //Set counter mode to up
	m_sHandle.Init.ClockDivision           = TIM_CLOCKDIVISION_DIV1;               //Unused
	m_sHandle.Init.RepetitionCounter       = 0x0;                                  //
	m_sHandle.Init.AutoReloadPreload       = TIM_AUTORELOAD_PRELOAD_ENABLE;        //Enable preload of the timer's auto-reload register

  //Initialize Timer in PWM mode, performing a partial deinitialization if the initialization fails
  if (HAL_TIM_PWM_Init(&m_sHandle) != HAL_OK) {
		periphDeinit(DeinitPartial);
		return QA_Fail;
	}

	//Init PWM Channels
	TIM_OC_InitTypeDef TIM_OC_Init;
	//Iterate through number of channels supported by selected timer peripheral
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		//If channel is set to active then initialize PWM channel
		if (m_sChannels[i].eActive) {
			TIM_OC_Init = {0};
			TIM_OC_Init.OCMode        = TIM_OCMODE_PWM1;        //Set Output Compare mode to PWM1
			TIM_OC_Init.OCIdleState   = TIM_OCIDLESTATE_SET;    //Set Output Compare Idle State to Set
			TIM_OC_Init.Pulse         = 0;                      //
			TIM_OC_Init.OCPolarity    = TIM_OCPOLARITY_HIGH;    //Set Output Compare Polarity to High
			TIM_OC_Init.OCFastMode    = TIM_OCFAST_ENABLE;      //Enable Output Compare Fast Mode

			//Configure PWM Channel, performing a full deinitialization if the configuration fails
			if (HAL_TIM_PWM_ConfigChannel(&m_sHandle, &TIM_OC_Init, m_uChannelSelect[i]) != HAL_OK) {
				periphDeinit(DeinitFull);
				return QA_Fail;
			}
		}
	}

	//Set Driver States
	m_eInitState = QA_Initialized; //Set driver state as initialized
	m_eState     = QA_Inactive;    //Set driver as currently inactive

	//Return
	return QA_OK;
}


//QAD_PWM::periphDeinit
//QAD_PWM Private Initialization Method
//
//Used to deinitialize the GPIOs, timer peripheral clock and the timer peripheral itself
//eDeinitMode - Set to DeinitPartial to perform a partial deinitialization (only to be used by periphInit() method
//              in a case where peripheral initialization has failed
//            - Set to DeinitFull to perform a full deinitialization in a case where the driver is fully initialized
void QAD_PWM::periphDeinit(QAD_PWM::DeinitMode eDeinitMode) {

	//Check if a full deinitialization is required
	if (eDeinitMode) {

		//Deinitialize Timer Peripheral
		HAL_TIM_PWM_DeInit(&m_sHandle);

		//Disable Timer Clock
		QAD_TimerMgr::disableClock(m_eTimer);

	}

	//Deinitialize GPIOs
	for (uint8_t i=0; i<QAD_TimerMgr::getChannels(m_eTimer); i++) {
		if (m_sChannels[i].eActive)
			HAL_GPIO_DeInit(m_sChannels[i].pGPIO, m_sChannels[i].uPin);
	}

	//Set Driver States
	m_eState     = QA_Inactive;        //Set driver as currently inactive
	m_eInitState = QA_NotInitialized;  //Set driver state as not initialized
}

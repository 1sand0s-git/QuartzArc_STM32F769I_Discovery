/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: GPIO Driver                                                     */
/*   Filename: QAD_GPIO.cpp                                                */
/*   Date: 24th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_GPIO.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


  //---------------
  //---------------
  //QAD_GPIO_Output

//QAD_GPIO_Output::QAD_GPIO_Output
//QAD_GPIO_Output Constructor
//
//This method will initialize the required GPIO pin in Push Pull mode, with Pull Up and Pull Down resistors disabled, and in low speed mode
//pGPIO - The GPIO port for the required pin. A member of GPIO_TypeDef as defined in stm32f769xx.h
//uPin  - The pin number for the required pin. A member of GPIO_pins_define as defined in stm32f7xx_hal_gpio.h
QAD_GPIO_Output::QAD_GPIO_Output(GPIO_TypeDef* pGPIO, uint16_t uPin) :
	m_pGPIO(pGPIO),
	m_uPin(uPin),
	m_eOutputMode(QAD_GPIO_OutputMode_PushPull),
	m_ePullMode(QAD_GPIO_PullMode_NoPull),
	m_eSpeed(QAD_GPIO_Speed_Low),
	m_eState(QAD_GPIO_PinState_Off) {

	//Initialize the GPIO Pin
  periphInit();
}


//QAD_GPIO_Output::QAD_GPIO_Output
//QAD_GPIO_Output Constructor
//
//This method will initialize the required GPIO pin using the selected output mode (PP/OD), Pull Up/Pull Down resistors and selected speed
//pGPIO  - The GPIO port for the required pin. A member of GPIO_TypeDef as defined in stm32f769xx.h
//uPin   - The pin number for the required pin. A member of GPIO_pins_define as defined in stm32f7xx_hal_gpio.h
//eMode  - Selects if pin is to be used in push/pull or open drain mode. A member of QAD_GPIO_OutputMode as defined in QAD_GPIO.hpp
//ePull  - Selects if pull up or pull down resistor is to be used. A member of QAD_GPIO_PullMode as defined in QAD_GPIO.hpp
//eSpeed - Selects the speed the pin is to be used in. A member of QAD_GPIO_Speed as defined in QAD_GPIO.hpp
QAD_GPIO_Output::QAD_GPIO_Output(GPIO_TypeDef* pGPIO, uint16_t uPin, QAD_GPIO_OutputMode eMode, QAD_GPIO_PullMode ePull, QAD_GPIO_Speed eSpeed) :
  m_pGPIO(pGPIO),
	m_uPin(uPin),
	m_eOutputMode(eMode),
	m_ePullMode(ePull),
	m_eSpeed(eSpeed),
	m_eState(QAD_GPIO_PinState_Off) {

	//Initialize the GPIO Pin
	periphInit();
}


//QAD_GPIO_Output::~QAD_GPIO_Output
//QAD_GPIO_Output Destructor
//
//Destructor used to deinitialize the GPIO pin when the driver class is destroyed
QAD_GPIO_Output::~QAD_GPIO_Output() {

	//Deinitialize the GPIO Pin
  periphDeinit();
}


//QAD_GPIO_Output::on
//QAD_GPIO_Output Control Method
//
//Used to turn the GPIO pin on
void QAD_GPIO_Output::on(void) {
	HAL_GPIO_WritePin(m_pGPIO, m_uPin, GPIO_PIN_SET);
	m_eState = QAD_GPIO_PinState_On;
}


//QAD_GPIO_Output::off
//QAD_GPIO_Output Control Method
//
//Used to turn the GPIO pin off
void QAD_GPIO_Output::off(void) {
	HAL_GPIO_WritePin(m_pGPIO, m_uPin, GPIO_PIN_RESET);
	m_eState = QAD_GPIO_PinState_Off;
}


//QAD_GPIO_Output::toggle
//QAD_GPIO_Output Control Method
//
//Used to toggle the state of the GPIO pin (will turn off if currently on, or turn on if currently off)
void QAD_GPIO_Output::toggle(void) {

	switch (m_eState) {
	  case (QAD_GPIO_PinState_On):
	  	off();
	    break;
	  case (QAD_GPIO_PinState_Off):
	  	on();
	    break;
	}
}


//QAD_GPIO_Output::getState
//QAD_GPIO_Output Control Method
//
//Returns the current state of the GPIO pin. Returns a member of QAD_GPIO_PinState (defined in QAD_GPIO.hpp)
QAD_GPIO_PinState QAD_GPIO_Output::getState(void) {
  return m_eState;
}


//QAD_GPIO_Output::setOutputMode
//QAD_GPIO_Output Control Method
//
//Sets a new output mode
//eMode - New output mode. Member of QAD_GPIO_OutputMode enum as defined in QAD_GPIO.hpp
void QAD_GPIO_Output::setOutputMode(QAD_GPIO_OutputMode eMode) {

	//Check if new mode is the same as the current mode
	if (m_eOutputMode == eMode)
		return;

	//Store new Output Mode
	m_eOutputMode = eMode;

	//Reinitialize GPIO with new Output Mode setting
	periphInit();
}


//QAD_GPIO_Output::getOutputMode
//QAD_GPIO_Output Control Method
//
//Returns current output mode. Member of QAD_GPIO_OutputMode enum as defined in QAD_GPIO.hpp
QAD_GPIO_OutputMode QAD_GPIO_Output::getOutputMode(void) {

	//Return current Output Mode
	return m_eOutputMode;
}


//QAD_GPIO_Output::setPullMode
//QAD_GPIO_Output Control Method
//
//Sets new pull mode (Pull-up, Pull-down or No-pull)
//eMode - New pull mode. Member of QAD_GPIO_PullMode enum as defined in QAD_GPIO.hpp
void QAD_GPIO_Output::setPullMode(QAD_GPIO_PullMode eMode) {

	//Store new Pull Mode
	m_ePullMode = eMode;

	//Reinitialize GPIO with new Output Mode setting
	periphInit();
}


//QAD_GPIO_Output::getPullMode
//QAD_GPIO_Output Control Method
//
//Returns current pull mode. Member of QAD_GPIO_PullMode enum as defined in QAD_GPIO.hpp
QAD_GPIO_PullMode QAD_GPIO_Output::getPullMode(void) {

	//Return current Pull Mode
	return m_ePullMode;
}


//QAD_GPIO_Output::setSpeed
//QAD_GPIO_Output Control Method
//
//Sets new GPIO speed
//eSpeed - New speed setting. Member of QAD_GPIO_Speed enum as defined in QAD_GPIO.hpp
void QAD_GPIO_Output::setSpeed(QAD_GPIO_Speed eSpeed) {

	//Store new Speed
	m_eSpeed = eSpeed;

	//Reinitialize GPIO with new Speed setting
	periphInit();
}


//QAD_GPIO_Output::getSpeed
//QAD_GPIO_Output Control Method
//
//Returns current speed setting. Member of QAD_GPIO_Speed enum as defined in QAD_GPIO.hpp
QAD_GPIO_Speed QAD_GPIO_Output::getSpeed(void) {

	//Return current Speed
	return m_eSpeed;
}


	//----------------------------------------------
	//QAD_GPIO_Output Private Initialization Methods

//QAD_GPIO_Output::periphInit
//QAD_GPIO_Output Private Initialization Method
//
//Used to initialize the GPIO Pin based on the currently selected settings
//Specifically to be called by device class constructors and methods used for changing settings
void QAD_GPIO_Output::periphInit(void) {
	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Pin    = m_uPin;
	GPIO_Init.Mode   = m_eOutputMode ? GPIO_MODE_OUTPUT_OD : GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull   = m_ePullMode;
	GPIO_Init.Speed  = m_eSpeed;
	HAL_GPIO_Init(m_pGPIO, &GPIO_Init);
}


//QAD_GPIO_Output::periphDeinit
//QAD_GPIO_Output Private Initialization Method
//
//Used to deinitialize the GPIO pin
//Specifically to be called by device class destructor and methods used for changing settings
void QAD_GPIO_Output::periphDeinit(void) {

	HAL_GPIO_DeInit(m_pGPIO, m_uPin);
}




	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //--------------
  //--------------
  //QAD_GPIO_Input


  //-----------------------------------------
  //QAD_GPIO_Input Constructors / Destructors

//QAD_GPIO_Input::QAD_GPIO_Input
//QAD_GPIO_Input Constructor
//
//This method will initialize the required GPIO pin with Pull Up and Pull Down resistors disabled, and in low speed mode
//pGPIO - The GPIO port for the required pin. A member of GPIO_TypeDef as defined in stm32f769xx.h
//uPin  - The pin number for the required pin. A member of GPIO_pins_define as defined in stm32f7xx_hal_gpio.h
QAD_GPIO_Input::QAD_GPIO_Input(GPIO_TypeDef* pGPIO, uint16_t uPin) :
		m_pGPIO(pGPIO),
		m_uPin(uPin),
		m_ePullMode(QAD_GPIO_PullMode_NoPull) {

	//Initialize the GPIO pin
	periphInit();
}


//QAD_GPIO_Input::QAD_GPIO_Input
//QAD_GPIO_Input Constructor
//
//This method will initialize the required GPIO pin using the selection Pull Up/Pull Down setting and selected speed
//pGPIO  - The GPIO port for the required pin. A member of GPIO_TypeDef as defined in stm32f769xx.h
//uPin   - The pin number for the required pin. A member of GPIO_pins_define as defined in stm32f7xx_hal_gpio.h
//ePull  - Selects if pull-up or pull-down resistor is to be used. A member of QAD_GPIO_PullMode as defined in QAD_GPIO.hpp
QAD_GPIO_Input::QAD_GPIO_Input(GPIO_TypeDef* pGPIO, uint16_t uPin, QAD_GPIO_PullMode ePull) :
		m_pGPIO(pGPIO),
		m_uPin(uPin),
		m_ePullMode(ePull) {

	//Initialize the GPIO pin
	periphInit();
}


//QAD_GPIO_Input::~QAD_GPIO_Input
//QAD_GPIO_Input Destructor
QAD_GPIO_Input::~QAD_GPIO_Input() {

	//Deinitialize the GPIO pin
	periphDeinit();
}


  //------------------------------
  //QAD_GPIO_Input Control Methods

//QAD_GPIO_Input::getState
//QAD_GPIO_Input Control Method
//
//Returns the current state of the pin. A member of QAD_GPIO_PinState as defined in QAD_GPIO.hpp
QAD_GPIO_PinState QAD_GPIO_Input::getState(void) {
	return (HAL_GPIO_ReadPin(m_pGPIO, m_uPin) == GPIO_PIN_SET) ? QAD_GPIO_PinState_On : QAD_GPIO_PinState_Off;
}


//QAD_GPIO_Input::setPullMode
//QAD_GPIO_Input Control Method
//
//Sets new pull mode (Pull-up, Pull-down or No-pull)
//ePull - New pull mode. Member of QAD_GPIO_PullMode as defined in QAD_GPIO.hpp
void QAD_GPIO_Input::setPullMode(QAD_GPIO_PullMode ePull) {

	//Store new Pull mode setting
	m_ePullMode = ePull;

	//Reinitialize peripheral with new Pull mode
	periphInit();
}


//QAD_GPIO_Input::getPullMode
//QAD_GPIO_Input Control Method
//
//Returns current pull mode. Member of QAD_GPIO_PullMode as defined in QAD_GPIO.hpp
QAD_GPIO_PullMode QAD_GPIO_Input::getPullMode(void) {

	//Return curent Pull mode
	return m_ePullMode;
}


  //---------------------------------------------
  //QAD_GPIO_Input Private Initialization Methods


//QAD_GPIO_Input::periphInit
//QAD_GPIO_Input Private Initialization Method
//
//Used to initialize the GPIO Pin based on the currently selected settings
//Specifically to be called by device class constructors and methods used for changing settings
void QAD_GPIO_Input::periphInit(void) {

	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Pin    = m_uPin;
	GPIO_Init.Mode   = GPIO_MODE_INPUT;
	GPIO_Init.Pull   = m_ePullMode;
	GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(m_pGPIO, &GPIO_Init);
}


//QAD_GPIO_Input::periphDeinit
//QAD_GPIO_Init Private Initialization Method
//
//Used to deinitialize the GPIO Pin
//Specifically to be called by device class destructor and methods used for changing settings
void QAD_GPIO_Input::periphDeinit(void) {

	HAL_GPIO_DeInit(m_pGPIO, m_uPin);
}


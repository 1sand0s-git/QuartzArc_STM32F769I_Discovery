/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Application Entry Point                                         */
/*   Filename: main.cpp                                                    */
/*   Date: 22nd September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */


//Includes
#include "main.hpp"
#include "boot.hpp"

#include "QAD_GPIO.hpp"
#include "QAD_FMC.hpp"
#include "QAD_RNG.hpp"
#include "QAD_RTC.hpp"
#include "QAD_I2C.hpp"
#include "QAD_FT6206.hpp"
#include "QAD_QuadSPI.hpp"
#include "QAD_SDMMC.hpp"

#include "QAS_Serial_Dev_UART.hpp"
#include "QAS_LCD.hpp"

#include <string.h>
#include <stdio.h>


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//User LED driver classes (driver defined in QAD_GPIO.hpp)
QAD_GPIO_Output* GPIO_UserLED_Red;
QAD_GPIO_Output* GPIO_UserLED_Green;

//User Button driver class (driver defined in QAD_GPIO.hpp)
QAD_GPIO_Input* GPIO_UserButton;

//STLink UART system class (system defined in QAD_Serial_Dev_UART.hpp)
QAS_Serial_Dev_UART* UART_STLink;

//I2C driver class (used for touch controller and audio codec)
QAD_I2C* I2C_System;


//Task Timing
//
//These constants are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop within the main() function.
//

const uint32_t QA_FT_SDCardTickThreshold = 500;

const uint32_t QA_FT_LCDTickThreshold = 33;

const uint32_t QA_FT_HeartbeatTickThreshold = 500;   //Time in milliseconds between heartbeat LED updates
                                                     //The rate of flashing of the heartbeat LED will be double the value defined here


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------------------
//-------------------------------
//System Initialization Functions

QA_Result QA_DriverInit(void);
QA_Result QA_SystemInit(void);


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//main
//Application Entry Point
//
//This is the first C++ function to be called when the microcontroller starts up
//This function is called by the assembly language in startup_stm32f769nihx.s
int main(void) {

	//----------------------------------
	//----------------------------------
  //System Initialization

	//Calls the SystemInitialize method located within boot.hpp, which initializes the
	//CPU caches, as well as the PLLs, system, bus and some peripheral clocks.
	//All GPIOs are also enabled by this method
	if (SystemInitialize()) {
	  while (1) {}
  }

  //Initialize Drivers
	if (QA_DriverInit()) {
		while (1) {}
	}
	UART_STLink->txStringCR("Drivers Initialized OK");
	UART_STLink->txCR();

	//Initialize Systems
	if (QA_SystemInit()) {
		UART_STLink->txStringCR("Systems Initialization Failed");
		while (1) {}
	}
	UART_STLink->txStringCR("Systems Initialized OK");


	//


	//----------------------------------
	//----------------------------------
  //Processing Loop

  //Create processing loop timing variables
  uint32_t uTicks;
  uint32_t uCurTick;
  uint32_t uNewTick = HAL_GetTick();
  uint32_t uOldTick = uNewTick;

  //Create task timing variables
  uint32_t uSDCardTicks = 0;
  uint32_t uLCDTicks = 0;

  uint32_t uHeartbeatTicks = 0;


  //-----------------------------------
  //Infinite loop for device processing
  while (1) {

  	//----------------------------------
  	//Frame Timing
  	//Calculates how many ticks (in milliseconds) have passed since the previous loop, this value is placed into the uTicks variable
  	//uTicks is then used to calculate task timing below
    uCurTick = HAL_GetTick();
    if (uCurTick != uNewTick) { //Handle loop faster than 1ms issue
    	uOldTick = uNewTick;
    	uNewTick = uCurTick;
    	if (uNewTick < uOldTick) { //Handle 32bit overflow issue
    		uTicks = (uNewTick + (0xFFFFFFFF - uOldTick));
    	} else {
    		uTicks = (uNewTick - uOldTick);
    	}

    } else {
    	uTicks = 0;
    }


    //Update SDCard
    uSDCardTicks += uTicks;
    if (uSDCardTicks >= QA_FT_SDCardTickThreshold) {
      QAD_SDMMC::process();
    	uSDCardTicks -= QA_FT_SDCardTickThreshold;
    }


    //Update LCD
    uLCDTicks += uTicks;
    if (uLCDTicks >= QA_FT_LCDTickThreshold) {

    	char strLCD[256];

      QAS_LCD::setDrawBuffer(QAD_LTDC_Layer0);
      QAS_LCD::setDrawColor(0xF333);
      QAS_LCD::clearBuffer();

      QAS_LCD::setDrawColor(0xF000);
      QAS_LCD::drawRectFill(QAT_Vector2_16(40, 40), QAT_Vector2_16(760, 440));

      QAS_LCD::setFontByIndex(1);
      QAS_LCD::setDrawColor(0xFFFF);
      QAS_LCD::drawStrC(QAT_Vector2_16(400, 45), "SD Card Test");

        //Card Presence
      QAS_LCD::drawStrL(QAT_Vector2_16(50, 80), "Card Present:");
      if (QAD_SDMMC::getCardPresent())
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 80), "Card Present"); else
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 80), "No Card");

        //Card State
      QAS_LCD::drawStrL(QAT_Vector2_16(50, 110), "Card State:");
      QAD_SDMMC_CardState eState = QAD_SDMMC::getCardState();
      if (eState == QAD_SDMMC_State_NoCard)
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 110), "No Card");
      else if (eState == QAD_SDMMC_State_PeripheralError)
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 110), "Peripheral Error");
      else if (eState == QAD_SDMMC_State_CardError)
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 110), "Card Error");
      else {

      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 110), "Mounted");

      	//Size
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 150), "Card Size:");
      	sprintf(strLCD, "%lu MB", QAD_SDMMC::getCardSizeMB());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 150), strLCD);

      	//Block Size
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 190), "Block Size:");
      	sprintf(strLCD, "%lu bytes", QAD_SDMMC::getBlockSize());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 190), strLCD);

      	//Block Count
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 220), "Block Count:");
      	sprintf(strLCD, "%lu", QAD_SDMMC::getBlockCount());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 220), strLCD);

      	//Card Type
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 270), "Card Type:");
      	sprintf(strLCD, "%lu", QAD_SDMMC::getCardType());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 270), strLCD);

      	//Card Version
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 300), "Card Version:");
      	sprintf(strLCD, "%lu", QAD_SDMMC::getCardVersion());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 300), strLCD);

      	//Card Class
      	QAS_LCD::drawStrL(QAT_Vector2_16(50, 330), "Card Class:");
      	sprintf(strLCD, "%lu", QAD_SDMMC::getCardClass());
      	QAS_LCD::drawStrL(QAT_Vector2_16(400, 330), strLCD);
      }

      QAS_LCD::setDrawBuffer(QAD_LTDC_Layer1);
      QAS_LCD::setDrawColor(0x0000);
      QAS_LCD::clearBuffer();

      QAS_LCD::flipLayer0();
      QAS_LCD::flipLayer1();

    	uLCDTicks -= QA_FT_LCDTickThreshold;
    }


  	//----------------------------------
    //Update Heartbeat LED
    //The heartbeat LED uses the green User LED to flash at a regular rate to visually show whether the microcontroller has locked up or
    //has become stuck in an exception or interrupt handler
    uHeartbeatTicks += uTicks;
    if (uHeartbeatTicks >= QA_FT_HeartbeatTickThreshold) {
    	GPIO_UserLED_Green->toggle();
    	uHeartbeatTicks -= QA_FT_HeartbeatTickThreshold;
    }

  }

  //This return value is unused, but in included in the source code to prevent compiler warning that main() doesn't return a value
  return 0;
}



	//-------------------------------
	//-------------------------------
	//System Initialization Functions

//QA_DriverInit
//System Initialization Function
QA_Result QA_DriverInit(void) {

	//----------------------------------
	//Initialize the User LEDs using the QAD_GPIO_Output driver class.
	//QAD_USERLED_RED_GPIO_PORT, QAD_USERLED_GREEN_GPIO_PORT, QAD_USER_LED_RED_GPIO_PIN and QAD_USERLED_GREEN_GPIO_PIN are defined in setup.hpp
  GPIO_UserLED_Red   = new QAD_GPIO_Output(QAD_USERLED_RED_GPIO_PORT, QAD_USERLED_RED_GPIO_PIN);
  GPIO_UserLED_Green = new QAD_GPIO_Output(QAD_USERLED_GREEN_GPIO_PORT, QAD_USERLED_GREEN_GPIO_PIN);


	//----------------------------------
  //Initialize the User Button using the QAD_GPIO_Input driver class.
  //QAD_USERBUTTON_GPIO_PORT and QAD_USERBUTTON_GPIO_PIN are defined in setup.hpp
  GPIO_UserButton = new QAD_GPIO_Input(QAD_USERBUTTON_GPIO_PORT, QAD_USERBUTTON_GPIO_PIN);


	//----------------------------------
  //Initialize UART1, which is used to communicate over serial via the STLINK connection
  //Baudrates, IRQ Priorities, GPIO details and FIFO sizes are defined in setup.hpp

  //Fill out initialization structure required to create UART class (defined in QAS_Serial_Dev_UART.hpp)
  QAS_Serial_Dev_UART_InitStruct sSerialInit;
  sSerialInit.sUART_Init.uart        = QAD_UART1;
  sSerialInit.sUART_Init.baudrate    = QAD_UART1_BAUDRATE;
  sSerialInit.sUART_Init.irqpriority = QAD_IRQPRIORITY_UART1;
  sSerialInit.sUART_Init.txgpio      = QAD_UART1_TX_PORT;
  sSerialInit.sUART_Init.txpin       = QAD_UART1_TX_PIN;
  sSerialInit.sUART_Init.txaf        = QAD_UART1_TX_AF;
  sSerialInit.sUART_Init.rxgpio      = QAD_UART1_RX_PORT;
  sSerialInit.sUART_Init.rxpin       = QAD_UART1_RX_PIN;
  sSerialInit.sUART_Init.rxaf        = QAD_UART1_RX_AF;
  sSerialInit.uTXFIFO_Size           = QAD_UART1_TX_FIFOSIZE;
  sSerialInit.uRXFIFO_Size           = QAD_UART1_RX_FIFOSIZE;

  //Create the UART class, passing to it a reference to the initialization structure
  UART_STLink = new QAS_Serial_Dev_UART(sSerialInit);

  //If initialization failed the turn on User LED and enter infinite loop
  if (UART_STLink->init(NULL) == QA_Fail) {
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }

  //If initialization succeeded then output a message via serial
  UART_STLink->txCR();
  UART_STLink->txStringCR("STM32F769I Discovery Booting...");



	//----------------------------------
  //Initialize FMC / SDRAM driver using the QAD_FMC singleton driver class
  if (QAD_FMC::init() == QA_OK) {

  	//If initialization passed then output message via serial
    UART_STLink->txStringCR("SDRAM: Initialized");

    //Test SDRAM to confirm correct operation
  	if (QAD_FMC::test() == QA_OK) {

  		//If SDRAM test passed then output message via serial
  		UART_STLink->txStringCR("SDRAM: Test Passed");

  	} else {

  		//If SDRAM test failed then output message via serial, turn on Red User LED and enter infinite loop
  		UART_STLink->txStringCR("SDRAM: Test Failed");
  		GPIO_UserLED_Red->on();
    	return QA_Fail;
  	}

  } else {

  	//If initialization failed then output message via serial, turn on User LED and enter infinite loop
    UART_STLink->txStringCR("SDRAM: Initialization failed");
    GPIO_UserLED_Red->on();
  	return QA_Fail;
  }


  //---------------
  //Init RNG Driver
  if (QAD_RNG::init()) {
  	UART_STLink->txStringCR("RNG: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  UART_STLink->txStringCR("RNG: Initialized");


  //---------------
  //Init RTC Driver
  if (QAD_RTC::init()) {
  	UART_STLink->txStringCR("RTC: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  UART_STLink->txStringCR("RTC: Initialized");



  //---------------------
  //Initialize System I2C
  QAD_I2C_InitStruct I2C_Init;
  I2C_Init.eI2C                = QAD_I2C4;
  I2C_Init.uIRQPriority_Event  = 0xE;
  I2C_Init.uIRQPriority_Error  = 0xE;
  I2C_Init.uTiming             = 40912732;
  I2C_Init.eAddressingMode     = QAD_I2C_AddressingMode_7Bit;
  I2C_Init.eDualAddressingMode = QAD_I2C_DualAddressingMode_Disable;
  I2C_Init.eGeneralCallMode    = QAD_I2C_GeneralCallMode_Disable;
  I2C_Init.eNoStretchMode      = QAD_I2C_NoStretchMode_Disable;
  I2C_Init.uOwnAddress1        = 0x0;
  I2C_Init.uOwnAddress2        = 0x0;
  I2C_Init.pSCL_GPIO           = GPIOD;
  I2C_Init.uSCL_Pin            = GPIO_PIN_12;
  I2C_Init.uSCL_AF             = GPIO_AF4_I2C4;
  I2C_Init.pSDA_GPIO           = GPIOB;
  I2C_Init.uSDA_Pin            = GPIO_PIN_7;
  I2C_Init.uSDA_AF             = GPIO_AF11_I2C4;
  I2C_System = new QAD_I2C(I2C_Init);

  if (I2C_System->init()) {
  	UART_STLink->txStringCR("System I2C: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  I2C_System->start();
  UART_STLink->txStringCR("System I2C: Initialized and Started");


  //----------------------------------
  //Initialize FT6206 Touch Controller
  if (QAD_FT6206::init(I2C_System)) {
  	UART_STLink->txStringCR("FT6206: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  UART_STLink->txStringCR("FT6206: Initialized");


  //-----------------------------------
  //Initialize QuadSPI / MX25L512 Flash
  if (QAD_QuadSPI::init()) {
  	UART_STLink->txStringCR("QuadSPI: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  UART_STLink->txStringCR("QuadSPI: Initialized");


  //--------------------------------
  //Initialize SDMMC / SDCard Driver
  if (QAD_SDMMC::init()) {
  	UART_STLink->txStringCR("SDMMC: Initialization Failed");
  	GPIO_UserLED_Red->on();
  	return QA_Fail;
  }
  UART_STLink->txStringCR("SDMMC: Initialized");


  //Return
  return QA_OK;
}


//QA_SystemInit
//System Initialization Function
QA_Result QA_SystemInit(void) {

	//----------------------------------
  //Initialize LCD using QAS_LCD singleton class.
  //This will also initialize LTDC and DSI peripherals, and also initialize otm8009a display controller
  if (QAS_LCD::init()) {
  	UART_STLink->txStringCR("LCD: Initialization failed");
  	return QA_Fail;
  }
  UART_STLink->txStringCR("LCD: Initialized");

  //Test rendering methods to confirm LCD and rendering subsystem are working correctly

  QAS_LCD::setDrawBuffer(QAD_LTDC_Layer0);
  QAS_LCD::setDrawColor(0xFFFF);
  QAS_LCD::clearBuffer();

  //Shapes
  QAS_LCD::setDrawColor(0xF003);
  QAS_LCD::drawRectFill(QAT_Vector2_16(40, 40), QAT_Vector2_16(760, 440));

  QAS_LCD::setDrawColor(0xFFFF);
  QAS_LCD::drawRect(QAT_Vector2_16(50, 50), QAT_Vector2_16(750, 430));

  QAS_LCD::setDrawBuffer(QAD_LTDC_Layer1);
  QAS_LCD::setDrawColor(0x0000);
  QAS_LCD::clearBuffer();

  QAS_LCD::setDrawColor(0xFBFB);
  QAS_LCD::setFontByName("SegoeUI12pt");
  //QAS_LCD::setFontByIndex(0);
  QAS_LCD::drawStrC(QAT_Vector2_16(400, 150), "Hello World!");

  QAS_LCD::setDrawColor(0xFBBF);
  //QAS_LCD::setFontByName("SegoeUI20ptSB");
  QAS_LCD::setFontByIndex(1);
  QAS_LCD::drawStrC(QAT_Vector2_16(400, 250), "Welcome to 1s and 0s");

  /*QAS_LCD::setDrawColor(0xFFBB);
  //QAS_LCD::setFontByName("ShowcardGothic18pt");
  QAS_LCD::setFontByIndex(2);
  QAS_LCD::drawStrC(QAT_Vector2_16(400, 350), "Test Font");*/

  QAS_LCD::flipLayer0();
  QAS_LCD::flipLayer1();

  //Return
  return QA_OK;
}



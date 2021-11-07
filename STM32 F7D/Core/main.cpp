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


//Task Timing
//
//These constancts are used to determine the update rate (in milliseconds) of each of the
//tasks that are run in the processing loop withing the main() function.
//
const uint32_t QA_FT_HeartbeatTickThreshold = 500;   //Time in milliseconds between heartbeat LED updates
                                                     //The rate of flashing of the heartbeat LED will be double the value defined here


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
	//----------------------------------
  //System Initialization

	//Calls the SystemInitialize method located within boot.hpp, which initializes the
	//CPU caches, as well as the PLLs, system, bus and some peripheral clocks.
	//All GPIOs are also enabled by this method
	if (SystemInitialize()) {
	  while (1) {}
  }


	//----------------------------------
	//Initialize the Used LEDs using the QAD_GPIO_Output driver class.
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
  sSerialInit.uart.uart        = QAD_UART1;
  sSerialInit.uart.baudrate    = QAD_UART1_BAUDRATE;
  sSerialInit.uart.irqpriority = QAD_IRQPRIORITY_UART1;
  sSerialInit.uart.txgpio      = QAD_UART1_TX_PORT;
  sSerialInit.uart.txpin       = QAD_UART1_TX_PIN;
  sSerialInit.uart.txaf        = QAD_UART1_TX_AF;
  sSerialInit.uart.rxgpio      = QAD_UART1_RX_PORT;
  sSerialInit.uart.rxpin       = QAD_UART1_RX_PIN;
  sSerialInit.uart.rxaf        = QAD_UART1_RX_AF;
  sSerialInit.txfifo_size      = QAD_UART1_TX_FIFOSIZE;
  sSerialInit.rxfifo_size      = QAD_UART1_RX_FIFOSIZE;

  //Create the UART class, passing to it a reference to the initialization structure
  UART_STLink = new QAS_Serial_Dev_UART(sSerialInit);

  //If initialization failed the turn on User LED and enter infinite loop
  if (UART_STLink->init(NULL) == QA_Fail) {
  	GPIO_UserLED_Red->on();
    while (1) {}
  }


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
  		while(1) {}
  	}

  } else {

  	//If initialization failed then output message via serial, turn on User LED and enter infinite loop
    UART_STLink->txStringCR("SDRAM: Initialization failed");
    GPIO_UserLED_Red->on();
    while (1) {}
  }


	//----------------------------------
  //Initialize LCD using QAS_LCD singleton class.
  //This will also initialize LTDC and DSI peripherals, and also initialize otm8009a display controller
  if (QAS_LCD::init()) {
  	UART_STLink->txStringCR("LCD: Initialization failed");
  }
  UART_STLink->txStringCR("LCD: Initialized");

  //Test rendering methods to confirm LCD and rendering subsystem are working correctly
  //QAS_LCD::flipLayer0();
  //QAS_LCD::flipLayer1();

  QAS_LCD::setDrawBuffer(Layer0);
  QAS_LCD::setDrawColor(0xFFFF);
  QAS_LCD::clearBuffer();

  //Shapes
  QAS_LCD::setDrawColor(0xF000);
  QAS_LCD::drawLine(QAT_Vector2(0, 240), QAT_Vector2(799, 240));  //Causes error
  QAS_LCD::drawLine(QAT_Vector2(10, 240), QAT_Vector2(789, 240));  //Causes error
  QAS_LCD::drawLine(QAT_Vector2(400, 0), QAT_Vector2(400, 479));    //Causes error
  QAS_LCD::drawLine(QAT_Vector2(0, 0), QAT_Vector2(799, 479));        //Causes error

  QAS_LCD::drawRect(QAT_Vector2(200, 120), QAT_Vector2(600, 360));      //Doesn't cause errors

  QAS_LCD::setDrawColor(0xF00F);
  QAS_LCD::drawRectFill(QAT_Vector2(300, 180), QAT_Vector2(500, 300));  //Causes error

  //Fonts
  QAS_LCD::setDrawColor(0xF000);
  //QAS_LCD::setFontByName("SegoeUI20ptSB");
  //QAS_LCD::setFontByIndex(0);
  QAS_LCD::setFontByIndex(0);
  QAS_LCD::drawStrC(QAT_Vector2(400, 100), "Hello World!");

  QAS_LCD::setDrawColor(0xF077);
  //QAS_LCD::setFontByName("SegoeUI20pt");
  QAS_LCD::setFontByIndex(1);
  QAS_LCD::drawStrC(QAT_Vector2(400, 150), "Welcome to");

  QAS_LCD::setDrawColor(0xF337);
  //QAS_LCD::setFontByName("SegoeUI20pt");
  QAS_LCD::setFontByIndex(0);
  QAS_LCD::drawStrC(QAT_Vector2(400, 200), "ABCDefghIJKLmnopQRSTuvwxYZ!@#$%^");

  QAS_LCD::setDrawBuffer(Layer1);
  QAS_LCD::setDrawColor(0x0000);
  QAS_LCD::clearBuffer();


  QAS_LCD::flipLayer0();
  QAS_LCD::flipLayer1();


	//----------------------------------
	//----------------------------------
	//----------------------------------
  //Processing Loop

  //Create processing loop timing variables
  uint32_t uTicks;
  uint32_t uCurTick;
  uint32_t uNewTick = HAL_GetTick();
  uint32_t uOldTick = uNewTick;

  //Create task timing variables
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



/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - Serial                                              */
/*   Role: Serial Device UART Class                                        */
/*   Filename: QAS_Serial_Dev_UART.hpp                                     */
/*   Date: 28th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_SERIAL_DEV_UART_HPP_
#define __QAS_SERIAL_DEV_UART_HPP_


//Includes
#include "setup.hpp"

#include <memory>
#include <string.h>

#include "QAT_FIFO.hpp"
#include "QAS_Serial_Dev_Base.hpp"
#include "QAD_UART.hpp"



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//------------------------------
//QAS_Serial_Dev_UART_InitStruct
//
//This structure is used to be able to create the QAS_Serial_Dev_UART system class
typedef struct {

	QAD_UART_InitStruct sUART_Init;     //QAD_UART_InitStruct containing details for setup of UART peripheral (as defined in QAD_UART.hpp)

	uint16_t            uTXFIFO_Size;   //Size in bytes of the circular FIFO buffer to be used for data transmission
	uint16_t            uRXFIFO_Size;   //Size in bytes of the circular FIFO buffer to be used for data reception

} QAS_Serial_Dev_UART_InitStruct;



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------
//QAS_Serial_Dev_UART
//
//This class inherits from the QAS_Serial_Dev_Base system class (defined in QAS_Serial_Dev_Base.hpp)
//This class is used to implement serial functionality using UART peripherals
class QAS_Serial_Dev_UART : public QAS_Serial_Dev_Base {
private:

	QAD_UART_Periph           m_ePeriph;  //UART peripheral to be used (member of QAD_UART_Periph, as defined in QAD_UARTMgr.hpp)

	std::unique_ptr<QAD_UART> m_pUART;    //Pointer to QAD_UART device class

public:

	//--------------------------
	//Constructors / Destructors

	QAS_Serial_Dev_UART() = delete;       //Delete the default class constructor, as we need an initialization structure to be provided on class creation

	//The class constructor to be used, which has a reference to a QAS_Serial_Dev_UART_InitStruct passed to it
  QAS_Serial_Dev_UART(QAS_Serial_Dev_UART_InitStruct& sInit) :
  	QAS_Serial_Dev_Base(sInit.uTXFIFO_Size, sInit.uRXFIFO_Size, DT_UART),
		m_ePeriph(sInit.sUART_Init.uart),
		m_pUART(std::make_unique<QAD_UART>(sInit.sUART_Init)) {}

private:

  //NOTE: The following methods are implementations of the pure virtual functions as defined in QAS_Serial_Dev_Base system class
  //See QAS_Serial_Dev_UART.cpp for details on the following methods

  //----------------------
  //Initialization Methods

  QA_Result imp_init(void* p) override;
  void imp_deinit(void) override;


  //---------------------------------
  //Interrupt Request Handler Methods

  void imp_handler(void* p) override;


  //---------------
  //Control Methods

  void imp_txStart(void) override;
  void imp_txStop(void) override;
  void imp_rxStart(void) override;
  void imp_rxStop(void) override;

};


//Prevent Recursive Inclusion
#endif /* __QAS_SERIAL_DEV_UART_HPP */

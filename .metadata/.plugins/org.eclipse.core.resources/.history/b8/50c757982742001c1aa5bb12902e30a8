/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: Exception and Interrupt Handlers                                */
/*   Filename: handlers.hpp                                                */
/*   Date: 22nd September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __HANDLERS_HPP_
#define __HANDLERS_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

extern "C" {


  //---------------------------
  //Exception Handler Functions

void  NMI_Handler(void);
void  HardFault_Handler(void);
void  MemManage_Handler(void);
void  BusFault_Handler(void);
void  UsageFault_Handler(void);
void  SVC_Handler(void);
void  DebugMon_Handler(void);
void  PendSV_Handler(void);
void  SysTick_Handler(void);


  //-------------------------
  //Interrupt Handler Methods

void USART1_IRQHandler(void);


}

//Prevent Recursive Inclusion
#endif /* __HANDLERS_HPP_ */

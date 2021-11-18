/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: RNG Driver                                                      */
/*   Filename: QAD_RNG.hpp                                                 */
/*   Date: 18th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_RNG_HPP_
#define __QAD_RNG_HPP_


//Includes
#include "setup.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-------
//QAD_RNG
class QAD_RNG {
private:

	QA_InitState      m_eInitState;

	RNG_HandleTypeDef m_sHandle;


	//-----------
	//Constructor
	QAD_RNG() :
	  m_eInitState(QA_NotInitialized),
		m_sHandle({0}) {}

public:

  //Delete copy constructor and assignment operator
  QAD_RNG(const QAD_RNG& other) = delete;
  QAD_RNG& operator=(const QAD_RNG& other) = delete;

  //-----------------
  //Singleton Methods
  static QAD_RNG& get(void) {
  	static QAD_RNG instance;
  	return instance;
  }


  //----------------------
  //Initialization Methods
  static QA_Result init(void) {
  	return get().imp_init();
  }

  static void deinit(void) {
  	get().imp_deinit();
  }


  //------------
  //Data Methods
  static QA_InitState getInitState(void) {
  	return get().m_eInitState;
  }

  static uint32_t getValue(void) {
  	return get().imp_getValue();
  }

private:

  //----------------------
  //Initialization Methods
  QA_Result imp_init(void);
  void imp_deinit(void);


  //------------
  //Data Methods
  uint32_t imp_getValue(void);

};


//Prevent Recursive Inclusion
#endif /* __QAD_RNG_HPP_ */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Tools                                                         */
/*   Role: Vector Tools                                                    */
/*   Filename: QAT_Vector.hpp                                              */
/*   Date: 20th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAT_VECTOR_HPP_
#define __QAT_VECTOR_HPP_


//Includes
#include "setup.hpp"


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------


//----
//----
//QAT_Vector2
class QAT_Vector2 {
public:

	uint16_t x;
	uint16_t y;

public:

  //Constructors
  QAT_Vector2() :
    x(0), y(0) {}

  QAT_Vector2(uint16_t x, uint16_t y) :
    x(x), y(y) {}

  QAT_Vector2(const QAT_Vector2& other) :
    x(other.x),
		y(other.y) {}

  //Operators
  bool operator==(const QAT_Vector2& other) const {
    return ((x == other.x) && (y == other.y));
  }

  QAT_Vector2& operator=(const QAT_Vector2& other) {
    if (*this == other)
      return *this;

    x = other.x;
    y = other.y;
    return *this;
  }

};


//Prevent Recursive Inclusion
#endif /* __QAT_VECTOR_H_ */
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


//-----------
//QAT_Vector2_16
//
//2D Vector structure used for providing X/Y coordinate data for LCD rendering methods
//The X and Y components are both 16bit values which works with the 800 pixel wide and 480 pixel high
//coordinate values required for the STM32F769I Discovery's LCD panel
class QAT_Vector2_16 {
public:

	//A union is used to allow 16bit x and y components to be accessed separately,
	//or accessed as a single 32 bit value.
	//Being able to copy, compare and manipulate the vector as a single 32bit value
	//allows for better performance.
	union {
    struct {
    	uint16_t x;    //X component of vector
    	uint16_t y;    //Y component of vector
    };
    uint32_t val;
	};

public:

	//------------
  //Constructors

	//Default constructor. Sets both X and Y components to zero
  QAT_Vector2_16() :
    val(0) {}

  //Constructor used to set individual X and Y components
  QAT_Vector2_16(uint16_t x, uint16_t y) :
    x(x), y(y) {}

  //Copy Constructor
  //Copies data as single 32bit value for performance reasons
  QAT_Vector2_16(const QAT_Vector2_16& other) :
    val(other.val) {}


  //---------
  //Operators

  //Equality operator
  //Performs equality operation as a single 32bit value for performance reasons
  bool operator==(const QAT_Vector2_16& other) const {
    return (val == other.val);
  }

  //Assinment operator
  //Performs assignment as a single 32bit value for performance reasons
  QAT_Vector2_16& operator=(const QAT_Vector2_16& other) {
    if (*this == other)
      return *this;

    val = other.val;
    return *this;
  }

};


//Prevent Recursive Inclusion
#endif /* __QAT_VECTOR_H_ */

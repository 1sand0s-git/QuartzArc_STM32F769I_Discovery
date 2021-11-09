/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Tools                                                         */
/*   Role: Pixel Tools                                                     */
/*   Filename: QAT_Pixel.hpp                                               */
/*   Date: 20th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAT_PIXEL_HPP_
#define __QAT_PIXEL_HPP_


//Includes
#include "setup.hpp"


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------


//-----------------
//QAT_Pixel_ARGB4444
//
//Class used to assist with the storage/manipulation of 16bit pixel data,
//with 4 bits for each for the Alpha, Red, Green and Blue color components
//
//The class is designed to only take up 16bits (2 bytes) of space to be
//usable for referencing into frame-buffer arrays, while adding functionality
//to aid in manipulation of individual A, R, G & B components
class QAT_Pixel_ARGB4444 {
private:

	uint16_t m_uPxl; //The actual pixel data

	//Used to compact alpha, red, green and blue components into a 16bit value
	//a, r, g & b should be values in between 0 and 15
	//Returns the 16bit pixel value
	uint16_t makePxl(uint8_t& a, uint8_t& r, uint8_t& g, uint8_t& b) {
		return ((a & 0x0F) << 12) | ((r & 0x0F) << 8) | ((g & 0x0F) << 4) | (b & 0x0F);
	}

public:

	//------------
	//Constructors

	//Default constructor
	//Creates a transparent, black pixel
	QAT_Pixel_ARGB4444() :
		m_uPxl(0) {}

	//Constructor to accept a 16bit pixel value
	QAT_Pixel_ARGB4444(uint16_t pxl) :
		m_uPxl(pxl) {}

	//Constructor to accept individual Alpha, Red, Green and Blue components
	QAT_Pixel_ARGB4444(uint8_t a, uint8_t r, uint8_t g, uint8_t b) :
		m_uPxl(makePxl(a, r, g, b)) {}

	//Copy Constructor
	QAT_Pixel_ARGB4444(const QAT_Pixel_ARGB4444& other) :
		m_uPxl(other.pxl()) {}


	//---------
	//Operators

	//Equality operator
	bool operator==(const QAT_Pixel_ARGB4444& other) const {
		return (m_uPxl == other.pxl());
	}

	//Assignment operator to accept another QAD_Pixel_ARGB4444 class
	QAT_Pixel_ARGB4444& operator=(const QAT_Pixel_ARGB4444& other) {
		if (*this == other)
			return *this;

		m_uPxl = other.pxl();
		return *this;
	}

	//Assignment operator to accept a 16bit pixel value
	QAT_Pixel_ARGB4444& operator=(const uint16_t& other) {
		m_uPxl = other;
		return *this;
	}


	//------------
	//Data Methods

	//Returns the current 16bit pixel value
	uint16_t pxl(void) const {
		return m_uPxl;
	}

	//Sets the current 16bit pixel value
	void pxl(uint16_t pxl) {
		m_uPxl = pxl;
	}

	//Sets the current pixel using individual Alpha, Red, Green and Blue components
	void pxl(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
		m_uPxl = makePxl(a, r, g, b);
	}


	//Returns the current 4 bit Alpha component of the pixel
	uint8_t a(void) const {
		return (m_uPxl & 0xF000) >> 12;
	}

	//Sets the 4 bit Alpha component of the pixel
	void a(uint8_t a) {
		m_uPxl = (m_uPxl & 0x0FFF) | ((a & 0x0F) << 12);
	}


	//Returns the current 4 bit Red component of the pixel
	uint8_t r(void) const {
		return (m_uPxl & 0x0F00) >> 8;
	}

	//Sets the 4 bit Red component of the pixel
	void r(uint8_t r) {
		m_uPxl = (m_uPxl & 0xF0FF) | ((r & 0x0F) << 8);
	}


	//Returns the current 4 bit Green component of the pixel
	uint8_t g(void) const {
		return (m_uPxl & 0x00F0) >> 4;
	}

	//Sets the 4 bit Green component of the pixel
	void g(uint8_t g) {
		m_uPxl = (m_uPxl & 0xFF0F) | ((g & 0x0F) << 4);
	}


	//Returns the current 4 bit Blue component of the pixel
	uint8_t b(void) const {
		return (m_uPxl & 0x000F);
	}

	//Sets the 4 bit Blue component of the pixel
	void b(uint8_t b) {
		m_uPxl = (m_uPxl & 0xFFF0) | (b & 0x0F);
	}

};


//Prevent Recursive Inclusion
#endif /* __QAT_PIXEL_HPP_ */

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


//QAT_PixelARGB4444
class QAT_PixelARGB4444 {
private:

	uint16_t m_uPxl;

	uint16_t makePxl(uint8_t& a, uint8_t& r, uint8_t& g, uint8_t& b) {
		return ((a & 0x0F) << 12) | ((r & 0x0F) << 8) | ((g & 0x0F) << 4) | (b & 0x0F);
	}

public:

	//Constructors
	QAT_PixelARGB4444() :
		m_uPxl(0) {}

	QAT_PixelARGB4444(uint16_t pxl) :
		m_uPxl(pxl) {}

	QAT_PixelARGB4444(uint8_t a, uint8_t r, uint8_t g, uint8_t b) :
		m_uPxl(makePxl(a, r, g, b)) {}

	QAT_PixelARGB4444(const QAT_PixelARGB4444& other) :
		m_uPxl(other.pxl()) {}

	//Operator
	bool operator==(const QAT_PixelARGB4444& other) const {
		return (m_uPxl == other.pxl());
	}

	QAT_PixelARGB4444& operator=(const QAT_PixelARGB4444& other) {
		if (*this == other)
			return *this;

		m_uPxl = other.pxl();
		return *this;
	}

	QAT_PixelARGB4444& operator=(const uint16_t& other) {
		m_uPxl = other;
		return *this;
	}

	//Pxl
	uint16_t pxl(void) const {
		return m_uPxl;
	}

	void pxl(uint16_t& pxl) {
		m_uPxl = pxl;
	}

	//A
	uint8_t a(void) const {
		return (m_uPxl & 0xF000) >> 12;
	}

	void a(uint8_t a) {
		m_uPxl = (m_uPxl & 0x0FFF) | ((a & 0x0F) << 12);
	}

	//R
	uint8_t r(void) const {
		return (m_uPxl & 0x0F00) >> 8;
	}

	void r(uint8_t r) {
		m_uPxl = (m_uPxl & 0xF0FF) | ((r & 0x0F) << 8);
	}

	//G
	uint8_t g(void) const {
		return (m_uPxl & 0x00F0) >> 4;
	}

	void g(uint8_t g) {
		m_uPxl = (m_uPxl & 0xFF0F) | ((g & 0x0F) << 4);
	}

	//B
	uint8_t b(void) const {
		return (m_uPxl & 0x000F);
	}

	void b(uint8_t b) {
		m_uPxl = (m_uPxl & 0xFFF0) | (b & 0x0F);
	}

};


//Prevent Recursive Inclusion
#endif /* __QAT_PIXEL_HPP_ */

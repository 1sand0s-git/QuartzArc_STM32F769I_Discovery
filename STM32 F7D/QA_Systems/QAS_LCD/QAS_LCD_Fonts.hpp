/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - LCD Rendering                                       */
/*   Role: LCD Rendering System - Fonts                                    */
/*   Filename: QAS_LCD_Fonts.hpp                                           */
/*   Date: 5th November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_LCD_FONTS_HPP_
#define __QAS_LCD_FONTS_HPP_


//Includes
#include "setup.hpp"

#include "QAD_LTDC.hpp"

#include "QAT_Vector.hpp"

#include <string.h>
#include <vector>


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------


#define QAS_LCD_FONTNAME_LENGTH  ((uint8_t)32)


//----------------
//QAS_LCD_FontDesc
typedef struct {
	uint16_t uWidth;
	uint16_t uOffset;
} QAS_LCD_FontDesc;


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//------------
//QAS_LCD_Font
class QAS_LCD_Font {
public:

	char m_cName[QAS_LCD_FONTNAME_LENGTH];

	QAS_LCD_FontDesc* m_pDesc;
	uint8_t*          m_pData;

	uint16_t          m_uHeight;
	uint16_t          m_uSpaceWidth;
	uint16_t          m_uCharGap;

public:

	//------------
  //Constructors

	QAS_LCD_Font() = delete;  //Delete default constructor

	QAS_LCD_Font(const char* strName, const QAS_LCD_FontDesc* pDesc, const uint8_t* pData, uint16_t& uHeight, uint16_t& uSpaceWidth, uint16_t& uCharGap) :
		m_uHeight(uHeight),
		m_uSpaceWidth(uSpaceWidth),
		m_uCharGap(uCharGap) {

		m_pDesc = (QAS_LCD_FontDesc*)pDesc;
		m_pData = (uint8_t*)pData;

		for (uint8_t i=0; i<QAS_LCD_FONTNAME_LENGTH; i++) {
			m_cName[i] = 0;
		}
		strcpy(m_cName, strName);
	}

	bool operator==(QAS_LCD_Font& other) {
		bool bResult = true;
		for (uint8_t i=0; i<QAS_LCD_FONTNAME_LENGTH; i++) {
			if (m_cName[i] != other.m_cName[i])
				bResult = false;
		}
		return bResult;
	}

	bool operator==(const char* strName) {
		bool bResult = true;
		uint8_t uLen = strlen(strName);
		uint8_t uIdxLen = (uLen < QAS_LCD_FONTNAME_LENGTH) ? uLen : QAS_LCD_FONTNAME_LENGTH;
		for (uint8_t i=0; i<uIdxLen; i++) {
			if (m_cName[i] != strName[i])
				bResult = false;
		}
		return bResult;
	}

	QAS_LCD_Font& operator=(QAS_LCD_Font& other) {
		for (uint8_t i=0; i<QAS_LCD_FONTNAME_LENGTH; i++)
			m_cName[i] = other.m_cName[i];

		m_pDesc       = other.m_pDesc;
		m_pData       = other.m_pData;
		m_uHeight     = other.m_uHeight;
		m_uSpaceWidth = other.m_uSpaceWidth;
		m_uCharGap    = other.m_uCharGap;
		return *this;
	}

};


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------


//---------------
//QAS_LCD_FontMgr
class QAS_LCD_FontMgr {
private:

	std::vector<QAS_LCD_Font*> m_vFonts;

	int8_t                     m_iCurrentIdx;
	QAS_LCD_Font*              m_pCurrent;

	QAD_LTDC_Buffer*           m_pBuffer;
	uint16_t                   m_uColor;

public:

	//--------------------------
	//Constructors / Destructors

	QAS_LCD_FontMgr() :
		m_iCurrentIdx(-1),
		m_pCurrent(NULL),
		m_pBuffer(NULL),
		m_uColor(0x0000) {
		m_vFonts.clear();
	}

	~QAS_LCD_FontMgr() {
		for (uint8_t i=0; i<m_vFonts.size(); i++) {
			delete m_vFonts[i];
		}
	}

	//------------------
	//Management Methods

	void add(const char* strName, const QAS_LCD_FontDesc* pDesc, const uint8_t* pData, uint16_t uHeight, uint16_t uSpaceWidth, uint16_t uCharGap);
	void remove(const char* strName);
	void clear(void);
	int8_t find(const char* strName);
	void setFontByName(const char* strName);
	void setFontByIndex(uint8_t uIdx);


private:

	//--------------------
	//Private Data Methods

	QAS_LCD_FontDesc* getDesc(void) const;
	uint8_t* getData(void) const;

public:

	//------------
	//Data Methods

	uint8_t getHeight(void) const;
	uint8_t getSpaceWidth(void) const;
	uint8_t getCharGap(void) const;
	uint16_t getCharWidth(char ch);
	uint16_t getStringWidth(const char* str);

	//-----------------
	//Rendering Methods

	void setDrawBuffer(QAD_LTDC_Buffer* pBuffer);
	void setDrawColor(uint16_t uColor);

	void drawChar(QAT_Vector2_16 cPos, char& ch);
	void drawStrL(QAT_Vector2_16 cPos, const char* str);
	void drawStrC(QAT_Vector2_16 cPos, const char* str);
	void drawStrR(QAT_Vector2_16 cPos, const char* str);

private:

	//-----------------
	//Rendering Methods

	void drawCharP(QAT_Vector2_16 cPos, char ch);

};


//Prevent Recursive Inclusion
#endif /* __QAS_LCD_FONTS_HPP_ */















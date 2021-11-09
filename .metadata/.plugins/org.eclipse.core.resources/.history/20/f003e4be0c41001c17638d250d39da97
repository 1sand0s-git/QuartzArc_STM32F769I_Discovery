/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - LCD Rendering                                       */
/*   Role: LCD Rendering System - Fonts                                    */
/*   Filename: QAS_LCD_Fonts.cpp                                           */
/*   Date: 5th November 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAS_LCD_Fonts.hpp"


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------


	//----------------------------------
	//----------------------------------
	//QAS_LCD_FontMgr Management Methods

//QAS_LCD_FontMgr::add
//QAS_LCD_FontMgr Management Method
void QAS_LCD_FontMgr::add(const char* strName, const QAS_LCD_FontDesc* pDesc, const uint8_t* pData, uint16_t uHeight, uint16_t uSpaceWidth, uint16_t uCharGap) {
  QAS_LCD_Font* cFont = new QAS_LCD_Font(strName, pDesc, pData, uHeight, uSpaceWidth, uCharGap);
  m_vFonts.push_back(cFont);
}


//QAS_LCD_FontMgr::remove
//QAS_LCD_FontMgr Management Method
void QAS_LCD_FontMgr::remove(const char* strName) {
  int8_t iIdx = find(strName);
  if (iIdx >= 0)
  	m_vFonts.erase(m_vFonts.begin()+iIdx);
}


//QAS_LCD_FontMgr::clear
//QAS_LCD_FontMgr Management Method
void QAS_LCD_FontMgr::clear(void) {
  m_vFonts.clear();
}


//QAS_LCD_FontMgr::find
//QAS_LCD_FontMgr Management Method
int8_t QAS_LCD_FontMgr::find(const char* strName) {
  int8_t iIdx = -1;
  for (uint8_t i=0; i<m_vFonts.size(); i++)
  	if (*m_vFonts[i] == strName) {
  		iIdx = i;
  	}
  return iIdx;
}


//QAS_LCD_FontMgr::setFontByName
//QAS_LCD_FontMgr Management Method
void QAS_LCD_FontMgr::setFontByName(const char* strName) {
  int8_t iIdx = find(strName);
  if (iIdx < 0) {
  	m_iCurrentIdx = -1;
  	m_pCurrent    = NULL;
  	return;
  }

  m_iCurrentIdx = iIdx;
  m_pCurrent    = m_vFonts[iIdx];
}


//QAS_LCD_FontMgr::setFontByIndex
//QAS_LCD_FontMgr Management Method
void QAS_LCD_FontMgr::setFontByIndex(uint8_t uIdx) {
  if (uIdx >= m_vFonts.size()) {
  	m_iCurrentIdx = -1;
  	m_pCurrent    = NULL;
  	return;
  }

  m_iCurrentIdx = uIdx;
  m_pCurrent    = m_vFonts[uIdx];
}


	//------------------------------------
	//QAS_LCD_FontMgr Private Data Methods

//QAS_LCD_FontMgr::getDesc
//QAS_LCD_FontMgr Private Data Methods
QAS_LCD_FontDesc* QAS_LCD_FontMgr::getDesc(void) const {
	if (m_iCurrentIdx < 0)
		return NULL;
	return m_pCurrent->m_pDesc;
}


//QAS_LCD_FontMgr::getData
//QAS_LCD_FontMgr Private Data Methods
uint8_t* QAS_LCD_FontMgr::getData(void) const {
	if (m_iCurrentIdx < 0)
		return NULL;
	return m_pCurrent->m_pData;
}


	//----------------------------
	//QAS_LCD_FontMgr Data Methods

//QAS_LCD_FontMgr::getHeight
//QAS_LCD_FontMgr Data Methods
uint8_t QAS_LCD_FontMgr::getHeight(void) const {
	if (m_iCurrentIdx < 0)
		return 0;
	return m_pCurrent->m_uHeight;
}


//QAS_LCD_FontMgr::getSpaceWidth
//QAS_LCD_FontMgr Data Methods
uint8_t QAS_LCD_FontMgr::getSpaceWidth(void) const {
	if (m_iCurrentIdx < 0)
		return 0;
	return m_pCurrent->m_uSpaceWidth;
}


//QAS_LCD_FontMgr::getCharGap
//QAS_LCD_FontMgr Data Methods
uint8_t QAS_LCD_FontMgr::getCharGap(void) const {
	if (m_iCurrentIdx < 0)
		return 0;
	return m_pCurrent->m_uCharGap;
}


//QAS_LCD_FontMgr::getCharWidth
//QAS_LCD_FontMgr Data Methods
uint16_t QAS_LCD_FontMgr::getCharWidth(char ch) {
  if (m_iCurrentIdx < 0)
    return 0;
  uint16_t uWidth = 0;
  if (ch == 32) {
    uWidth = m_pCurrent->m_uSpaceWidth;
  } else if ((ch >= 33) && (ch <= 126)) {
    uWidth = m_pCurrent->m_pDesc[ch-33].uWidth;
  }
  return uWidth;
}


//QAS_LCD_FontMgr::getStringWidth
//QAS_LCD_FontMgr Data Methods
uint16_t QAS_LCD_FontMgr::getStringWidth(const char* str) {
  uint16_t uWidth = 0;
  uint16_t uLength = strlen(str);
  uint8_t  uChar;

  if ((uLength == 0) || (m_iCurrentIdx < 0))
    return uWidth;

  for (uint8_t i=0; i<uLength; i++) {
    uChar = (uint8_t)str[i];
    if (uChar == 32) {
      uWidth += (m_pCurrent->m_uSpaceWidth + m_pCurrent->m_uCharGap);
    } else if ((uChar >= 33) && (uChar <= 126)) {
      uWidth += (m_pCurrent->m_pDesc[uChar-33].uWidth + m_pCurrent->m_uCharGap);
    }
  }
  uWidth -= m_pCurrent->m_uCharGap;
  return uWidth;
}


	//---------------------------------
	//QAS_LCD_FontMgr Rendering Methods

//QAS_LCD_FontMgr::setDrawBuffer
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::setDrawBuffer(QAD_LTDC_Buffer* pBuffer) {
  m_pBuffer = pBuffer;
}


//QAS_LCD_FontMgr::setDrawColor
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::setDrawColor(uint16_t uColor) {
  m_uColor = uColor;
}

//QAS_LCD_FontMgr::drawChar
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::drawChar(QAT_Vector2 cPos, char& ch) {
  if ((m_pBuffer == NULL) || (m_iCurrentIdx < 0))
    return;

  if ((ch >= 33) && (ch <= 126))
    drawCharP(cPos, ch);
}


//QAS_LCD_FontMgr::drawStrL
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::drawStrL(QAT_Vector2 cPos, const char* str) {
  if ((m_pBuffer == NULL) || (m_iCurrentIdx < 0))
    return;

  uint8_t uLen = strlen(str);
  if (uLen < 0) return;

  QAT_Vector2 cDrawPos = cPos;
  for (uint8_t i=0; i<uLen; i++) {
  	if ((str[i] >= 33) && (str[i] <= 126)) {
      drawCharP(cDrawPos, str[i]);
      cDrawPos.x += getCharWidth(str[i]) + getCharGap();
  	} else {
  		cDrawPos.x += getSpaceWidth() + getCharGap();
  	}
  }
}


//QAS_LCD_FontMgr::drawStrC
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::drawStrC(QAT_Vector2 cPos, const char* str) {
  if ((m_pBuffer == NULL) || (m_iCurrentIdx < 0))
    return;

  uint8_t uLen = strlen(str);
  if (uLen < 0) return;

  QAT_Vector2 cDrawPos = cPos;
  cDrawPos.x -= (getStringWidth(str) / 2);
  for (uint8_t i=0; i<uLen; i++) {
  	if ((str[i] >= 33) && (str[i] <= 126)) {
      drawCharP(cDrawPos, str[i]);
      cDrawPos.x += getCharWidth(str[i]) + getCharGap();
  	} else {
  		cDrawPos.x += getSpaceWidth() + getCharGap();
  	}
  }
}


//QAS_LCD_FontMgr::drawStrR
//QAS_LCD_FontMgr Data Methods
void QAS_LCD_FontMgr::drawStrR(QAT_Vector2 cPos, const char* str) {
  if ((m_pBuffer == NULL) || (m_iCurrentIdx < 0))
    return;

  uint8_t uLen = strlen(str);
  if (uLen < 0) return;

  QAT_Vector2 cDrawPos = cPos;
  cDrawPos.x -= getStringWidth(str);
  for (uint8_t i=0; i<uLen; i++) {
  	if ((str[i] >= 33) && (str[i] <= 126)) {
      drawCharP(cDrawPos, str[i]);
      cDrawPos.x += getCharWidth(str[i]) + getCharGap();
  	} else {
  		cDrawPos.x += getSpaceWidth() + getCharGap();
  	}
  }
}


  //-----------------------------------------
  //QAS_LCD_FontMgr Private Rendering Methods

//QAS_LCD_FontMgr::drawCharP
//QAS_LCD_FontMgr Private Rendering Method
void QAS_LCD_FontMgr::drawCharP(QAT_Vector2 cPos, char ch) {
  uint16_t uLetter = (uint8_t)ch - 33;
  uint16_t uWidth  = m_pCurrent->m_pDesc[uLetter].uWidth;
  uint32_t uOffset = m_pCurrent->m_pDesc[uLetter].uOffset;

  uint8_t uLine;
  uint8_t uLineInc;
  uint32_t uCur;

  for (uint8_t uHeight=0; uHeight<m_pCurrent->m_uHeight; uHeight++) {
    uLineInc = 0;
    uCur = cPos.x+((cPos.y+uHeight)*QAD_LTDC_WIDTH);
    for (uint16_t i=0; i<uWidth; i++) {
      if (uLineInc == 0) {
        uLine = m_pCurrent->m_pData[uOffset];
        uOffset++;
      }
      if (uLine & 0x01)
        m_pBuffer->pixel[uCur+i] = m_uColor;
      uLine = uLine >> 1;
      uLineInc++;
      if (uLineInc > 7)
        uLineInc = 0;
    }
  }
}
















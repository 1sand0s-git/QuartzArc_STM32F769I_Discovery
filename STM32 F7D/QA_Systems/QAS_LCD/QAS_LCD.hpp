/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - LCD Rendering                                       */
/*   Role: LCD Rendering System                                            */
/*   Filename: QAS_LCD.hpp                                                 */
/*   Date: 16th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAS_LCD_HPP_
#define __QAS_LCD_HPP_


//Includes
#include "setup.hpp"

#include "QAD_LTDC.hpp"

#include "QAT_Vector.hpp"

#include "QAS_LCD_Fonts.hpp"
#include "QAS_LCD_Fonts_SegoeUI12pt.hpp"
#include "QAS_LCD_Fonts_SegoeUI20ptSB.hpp"


#define QAS_LCD_ABS(X)    ((X) > 0 ? (X) : -(X))
#define QAS_LCD_X_MAX(X)  ((X) >= CE2XD_LTDC_WIDTH ? CE2XD_LTDC_WIDTH-1 : (X))
#define QAS_LCD_Y_MAX(X)  ((X) >= CE2XD_LTDC_HEIGHT ? CE2XD_LTDC_HEIGHT-1 : (X))

  //------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------
//QAS_LCD
class QAS_LCD {
private:

	QA_InitState      m_eInitState;

	QAD_LTDC_Buffer*  m_pDrawBuffer;
	uint16_t          m_uDrawColor;

  QAS_LCD_FontMgr   m_cFontMgr;

	QAS_LCD() :
	  m_eInitState(QA_NotInitialized),
		m_pDrawBuffer(NULL),
		m_uDrawColor(0x0000) {

	}

public:

  QAS_LCD(const QAS_LCD&) = delete;
  QAS_LCD& operator=(const QAS_LCD&) = delete;

  static QAS_LCD& get() {
  	static QAS_LCD instance;
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

    //-----------------------
    //Rendering Setup Methods

  static void flipLayer0(void) {
  	get().imp_flipLayer0();
  }

  static void flipLayer1(void) {
  	get().imp_flipLayer1();
  }

  static void setDrawBuffer(QAD_LTDC_LayerIdx eLayer) {
  	get().imp_setDrawBuffer(eLayer);
  }

  static void setDrawColor(uint16_t uColor) {
  	get().imp_setDrawColor(uColor);
  }

    //-----------------
    //Rendering Methods

  static void clearBuffer(void) {
  	get().imp_clearBuffer();
  }

  static void drawPixel(QAT_Vector2 cPos) {
  	get().imp_drawPixel(cPos);
  }

  static void drawLine(QAT_Vector2 cStart, QAT_Vector2 cEnd) {
  	get().imp_drawLine(cStart, cEnd);
  }

  static void drawRect(QAT_Vector2 cStart, QAT_Vector2 cEnd) {
  	get().imp_drawRect(cStart, cEnd);
  }

  static void drawRectFill(QAT_Vector2 cStart, QAT_Vector2 cEnd) {
  	get().imp_drawRectFill(cStart, cEnd);
  }

    //----------------------
    //Font Rendering Methods

  static void setFontByName(const char* strName) {
  	get().m_cFontMgr.setFontByName(strName);
  }

  static void setFontByIndex(uint8_t uIdx) {
  	get().m_cFontMgr.setFontByIndex(uIdx);
  }

  static void drawChar(QAT_Vector2 cPos, char ch) {
  	get().m_cFontMgr.drawChar(cPos, ch);
  }

  static void drawStrL(QAT_Vector2 cPos, const char* str) {
  	get().m_cFontMgr.drawStrL(cPos, str);
  }

  static void drawStrC(QAT_Vector2 cPos, const char* str) {
  	get().m_cFontMgr.drawStrC(cPos, str);
  }

  static void drawStrR(QAT_Vector2 cPos, const char* str) {
  	get().m_cFontMgr.drawStrR(cPos, str);
  }

private:

  //Initialization Methods
  QA_Result imp_init(void);
  void imp_deinit(void);

  //Rendering Setup Methods
  void imp_flipLayer0(void);
  void imp_flipLayer1(void);

  void imp_setDrawBuffer(QAD_LTDC_LayerIdx eLayer);
  void imp_setDrawColor(uint16_t uColor);

  //Rendering Methods
  void imp_clearBuffer(void);

  void imp_drawPixel(QAT_Vector2& cPos);
  void imp_drawLine(QAT_Vector2& cStart, QAT_Vector2& cEnd);
  void imp_drawHLine(QAT_Vector2& cStart, QAT_Vector2& cEnd);
  void imp_drawVLine(QAT_Vector2& cStart, QAT_Vector2& cEnd);
  void imp_drawALine(QAT_Vector2& cStart, QAT_Vector2& cEnd);

  void imp_drawRect(QAT_Vector2& cStart, QAT_Vector2& cEnd);
  void imp_drawRectFill(QAT_Vector2& cStart, QAT_Vector2& cEnd);

};


//Prevent Recursive Inclusion
#endif /* __QAD_LCD_HPP_ */
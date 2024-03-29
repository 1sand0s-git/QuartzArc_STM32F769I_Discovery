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

  //Font System Includes
#include "QAS_LCD_Fonts.hpp"


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------------------------
//QAS_LCD Macro Definitions
#define QAS_LCD_ABS(X)    ((X) > 0 ? (X) : -(X))                                   //Macro definition used to return absolute value
#define QAS_LCD_X_MAX(X)  ((X) >= CE2XD_LTDC_WIDTH ? CE2XD_LTDC_WIDTH-1 : (X))     //Macro definition used to limit value to LCD width - 1
#define QAS_LCD_Y_MAX(X)  ((X) >= CE2XD_LTDC_HEIGHT ? CE2XD_LTDC_HEIGHT-1 : (X))   //Macro definition used to limit value to LCD height - 1


  //------------------------------------------
	//------------------------------------------
	//------------------------------------------

//-------
//QAS_LCD
//
//Singleton class
//System class for LCD display rendering system
//This is setup as a singleton class due to making use of the LTDC peripheral driver, which is also a singleton class.
//As a result there is a need to avoid potential conflicts by not allowing more than one instance of the class to exist.
class QAS_LCD {
private:

	QA_InitState      m_eInitState;    //Stores whether the system is currently initialized. Member of QA_InitState enum defined in setup.hpp

	QAD_LTDC_Buffer*  m_pDrawBuffer;   //Stores a pointer to the current frame buffer that is being rendered to
	                                   //This will be either the layer 0 or layer 1 back-buffer (see QAD_LTDC.hpp for more details)

	uint16_t          m_uDrawColor;    //Stores the current colour to be used by rendering methods

  QAS_LCD_FontMgr   m_cFontMgr;      //A instance of the QAS_LCD_FontMgr class used for managing fonts and rendering of text


  //------------
  //Constructors

  //As this is a private method in a singleton class, this method will be called the first time the class's get() method is called
	QAS_LCD() :
	  m_eInitState(QA_NotInitialized),
		m_pDrawBuffer(NULL),
		m_uDrawColor(0x0000) {}

public:

	//----------------------------------------------------------------------------------
	//Delete the copy constructor and assignment operator due to being a singleton class
  QAS_LCD(const QAS_LCD&) = delete;
  QAS_LCD& operator=(const QAS_LCD&) = delete;


  //-----------------
  //Singleton Methods
  //
  //Used to retrieve a reference to the singleton class
  static QAS_LCD& get() {
  	static QAS_LCD instance;
  	return instance;
  }


	//----------------------
	//Initialization Methods

  //Used to initialize the LTDC peripheral and setup Font Manager class
  //Returns QA_OK if initialization successful, or QA_Fail if initialization fails
  static QA_Result init(void) {
  	return get().imp_init();
  }

  //Used to deinitialize the LTDC peripheral and flush Font Manager class
  static void deinit(void) {
  	get().imp_deinit();
  }


	//-----------------------
	//Rendering Setup Methods

  //Used to flip the front and back buffer for layer 0
  //See QAD_LTDC.hpp for more details
  static void flipLayer0(void) {
  	get().imp_flipLayer0();
  }

  //Used to flip the front and back buffer for layer 1
  //See QAD_LTDC.hpp for more details
  static void flipLayer1(void) {
  	get().imp_flipLayer1();
  }

  //Used to set the current draw buffer to either layer 0 or layer 1 back-buffer
  //eLayer - Sets whether the current draw buffer is layer 0 or layer 1. A member of QAD_LTDC_LayerIdx as defined in QAD_LTDC.hpp)
  static void setDrawBuffer(QAD_LTDC_LayerIdx eLayer) {
  	get().imp_setDrawBuffer(eLayer);
  }

  //Used to set the current draw color
  //uColor - A 16bit ARGB4444 color value
  static void setDrawColor(uint16_t uColor) {
  	get().imp_setDrawColor(uColor);
  }

  //Used to set the current draw color
  //cColor - A QAT_Pixel_ARGB4444 class containing the required color value
  static void setDrawColor(QAT_Pixel_ARGB4444 cColor) {
  	get().imp_setDrawColor(cColor.pxl());
  }


	//-----------------
	//Rendering Methods

  //Used to clear the currently selected draw buffer to the currently selected draw color
  static void clearBuffer(void) {
  	get().imp_clearBuffer();
  }

  //Used to draw an individual pixel located at a specific position
  //Pixel will be drawn to the currently selected draw buffer using the currently selected draw color
  //cPos - A QAT_Vector2_16 class containing the X and Y coordinates for the pixel to be drawn
  static void drawPixel(QAT_Vector2_16 cPos) {
  	get().imp_drawPixel(cPos);
  }

  //Used to draw an single pixel width line between a start and end point
  //Line will be drawn to the currently selected draw buffer using the currently selected draw color
  //cStart - A QAT_Vector2_16 class containing the X and Y coordinates for the line's start location
  //cEnd   - A QAT_Vector2_16 class containing the X and Y coordinates for the line's end location
  static void drawLine(QAT_Vector2_16 cStart, QAT_Vector2_16 cEnd) {
  	get().imp_drawLine(cStart, cEnd);
  }

  //Used to draw a non-filled rectangle
  //Rectangle will be drawn to the currently selected draw buffer using the currently selected draw color
  //cStart & cEnd - QAT_Vector2_16 classes that define X and Y coordinates of the diagonally opposing corners for the rectangle
  static void drawRect(QAT_Vector2_16 cStart, QAT_Vector2_16 cEnd) {
  	get().imp_drawRect(cStart, cEnd);
  }

  //Used to draw a filled rectangle
  //Rectangle will be drawn to the currently selected draw buffer using the currently selected draw color
  //cStart & cEnd - QAT_Vector2_16 classes that define X and Y coordinates of the diagonally opposing corder for the rectangle
  static void drawRectFill(QAT_Vector2_16 cStart, QAT_Vector2_16 cEnd) {
  	get().imp_drawRectFill(cStart, cEnd);
  }


	//----------------------
	//Font Rendering Methods

  //Used to set the selected font/typeface to be used for text rendering
  //strName - A C-style string containing the name of the font to be selected. This must match one of the
  //          fonts stored in the font manager, otherwise no font will be selected
  static void setFontByName(const char* strName) {
  	get().m_cFontMgr.setFontByName(strName);
  }

  //Used to set the selected font/typeface to be used for text rendering
  //uIdx - The index of the fond to be selected, based on the array of fonts stored in the font manager
  static void setFontByIndex(uint8_t uIdx) {
  	get().m_cFontMgr.setFontByIndex(uIdx);
  }

  //Used to draw an individual character using the currently selected font/typeface
  //Character will be drawn to currently selected draw buffer with currently selected draw color
  //cPos - A QAD_Vector2_16 class that defines the X and Y coordinates that define the position of upper-left location of the character to be drawn
  //ch   - The specific character to be rendered
  static void drawChar(QAT_Vector2_16 cPos, char ch) {
  	get().m_cFontMgr.drawChar(cPos, ch);
  }

  //Used to draw a left-aligned string of characters using the currently selected font/typeface
  //String will be drawn to currently selected draw buffer with currently selected draw color
  //cPos - A QAD_Vector2_16 class that defines the X and Y coordinates that define the position of the upper-left location of the string to be drawn
  //str  - The C-style string to be drawn
  static void drawStrL(QAT_Vector2_16 cPos, const char* str) {
  	get().m_cFontMgr.drawStrL(cPos, str);
  }

  //Used to draw a center-aligned string of characters using the currently selected font/typeface
  //String will be drawn to currently selected draw buffer with currently selected draw color
  //cPos - A QAD_Vector2_16 class that defines the X and Y coordinates that define the position of the upper-center location of the string to be drawn
  //str  - The C-style string to be drawn
  static void drawStrC(QAT_Vector2_16 cPos, const char* str) {
  	get().m_cFontMgr.drawStrC(cPos, str);
  }

  //Used to draw a right-aligned string of characters using the currently selected font/typeface
  //String will be drawn to currently selected draw buffer with currently selected draw color
  //cPos - A QAD_Vector2_16 class that defines the X and Y coordinates that define the position of the upper-right location of the string to be drawn
  //str  - The C-style string to be drawn
  static void drawStrR(QAT_Vector2_16 cPos, const char* str) {
  	get().m_cFontMgr.drawStrR(cPos, str);
  }

private:

  //NOTE: See QAS_LCD.cpp for details on the following methods

  //----------------------
  //Initialization Methods

  QA_Result imp_init(void);
  void imp_deinit(void);


  //-----------------------
  //Rendering Setup Methods

  void imp_flipLayer0(void);
  void imp_flipLayer1(void);

  void imp_setDrawBuffer(QAD_LTDC_LayerIdx eLayer);
  void imp_setDrawColor(uint16_t uColor);


  //-----------------
  //Rendering Methods

  void imp_clearBuffer(void);

  void imp_drawPixel(QAT_Vector2_16& cPos);
  void imp_drawLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);
  void imp_drawHLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);
  void imp_drawVLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);
  void imp_drawALine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);

  void imp_drawRect(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);
  void imp_drawRectFill(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd);

};


//Prevent Recursive Inclusion
#endif /* __QAD_LCD_HPP_ */

/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Systems - LCD Rendering                                       */
/*   Role: LCD Rendering System                                            */
/*   Filename: QAS_LCD.cpp                                                 */
/*   Date: 16th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAS_LCD.hpp"

  //Include font data header files
#include "QAS_LCD_Fonts_SegoeUI12pt.hpp"
#include "QAS_LCD_Fonts_SegoeUI20ptSB.hpp"
#include "QAS_LCD_Fonts_ShowcardGothic18pt.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //------------------------------
  //------------------------------
  //QAS_LCD Initialization Methods

//QAS_LCD::imp_init
//QAS_LCD Initialization Method
//
//To be called from static method init()
//To be used to initialize the LTDC peripheral and setup the Font Manager class
//Returns QA_OK if initialization successful, or QA_Fail if initialization fails
QA_Result QAS_LCD::imp_init(void) {

	//Return if system is already initialized
	if (m_eInitState) {
  	return QA_OK;
  }

	//Initialize LTDC driver class, returning QA_Fail if LTDC doesn't initialize successfully
	if (QAD_LTDC::init())
		return QA_Fail;


	//Setup font manager
	m_cFontMgr.clear();  //Clear font manager

	  //Add SegoeUI 12pt font (defined in QAS_LCD_Fonts_SegoeUI12pt.hpp)
	m_cFontMgr.add("SegoeUI12pt",
			           QAS_LCD_Fonts_SegoeUI12pt_Desc,
								 QAS_LCD_Fonts_SegoeUI12pt_Data,
								 QAS_LCD_Fonts_SegoeUI12pt_Height,
								 QAS_LCD_Fonts_SegoeUI12pt_SpaceWidth,
								 QAS_LCD_Fonts_SegoeUI12pt_CharGap);

	  //Add SegoeUI 20pt Semi-bold font (defined in QAS_LCD_Fonts_SegoeUI20ptSB.hpp)
	m_cFontMgr.add("SegoeUI20ptSB",
			           QAS_LCD_Fonts_SegoeUI20ptSB_Desc,
								 QAS_LCD_Fonts_SegoeUI20ptSB_Data,
								 QAS_LCD_Fonts_SegoeUI20ptSB_Height,
								 QAS_LCD_Fonts_SegoeUI20ptSB_SpaceWidth,
								 QAS_LCD_Fonts_SegoeUI20ptSB_CharGap);

	  //Add Showcard Gothic 18pt font (defined in QAS_LCD_Fonts_ShowcardGothic18pt.hpp)
	m_cFontMgr.add("ShowcardGothic18pt",
								 QAS_LCD_Fonts_ShowcardGothic18pt_Desc,
								 QAS_LCD_Fonts_ShowcardGothic18pt_Data,
								 QAS_LCD_Fonts_ShowcardGothic18pt_Height,
								 QAS_LCD_Fonts_ShowcardGothic18pt_SpaceWidth,
								 QAS_LCD_Fonts_ShowcardGothic18pt_CharGap);


	//Set System State
	m_eInitState = QA_Initialized; //Set system as now being initialized

	//Set initial data
	m_pDrawBuffer = NULL;   //Set current draw buffer to NULL
	m_uDrawColor  = 0x0000; //Set current draw color to transparent black

	//Return
	return QA_OK;
}


//QAS_LCD::imp_deinit
//QAS_LCD Initialization Method
//
//To be called from static method deinit()
//Used clear Font Manager and Deinitialize LTDC driver class
void QAS_LCD::imp_deinit(void) {

	//Clear Font Manager Font List
	m_cFontMgr.clear();

	//Deinitialize LTDC driver class
	QAD_LTDC::deinit();

	//Set System State
	m_eInitState = QA_NotInitialized;  //Set system as now being not initialized
}


  //-------------------------------
  //-------------------------------
  //QAS_LCD Rendering Setup Methods

//QAS_LCD::imp_flipLayer0
//QAS_LCD Rendering Setup Method
//
//To be called from static method flipLayer0()
//Used to flip the front and back buffer for layer 0
//See QAD_LTDC.hpp for more details
void QAS_LCD::imp_flipLayer0(void) {
  QAD_LTDC::flipLayer0Buffers();
}


//QAS_LCD::imp_flipLayer1
//QAS_LCD Rendering Setup Method
//
//To be called from static method flipLayer1()
//Used to flip the front and back buffer for layer 1
//See QAD_LTDC.hpp for more details
void QAS_LCD::imp_flipLayer1(void) {
  QAD_LTDC::flipLayer1Buffers();
}


//QAS_LCD::imp_setDrawBuffer
//QAS_LCD Rendering Setup Method
//
//To be called from static method setDrawBuffer()
//Used to set the current draw buffer to either layer 0 or layer 1 back-buffer
//eLayer - Sets whether the current draw buffer is layer 0 or layer1. A member of QAD_LTDC_LayerIdx as defined in QAD_LTDC.hpp)
void QAS_LCD::imp_setDrawBuffer(QAD_LTDC_LayerIdx eLayer) {

	//Retrieve required Back Buffer pointer based on selected layer and store to m_pDrawBuffer
	switch (eLayer) {
	  case (QAD_LTDC_Layer0):
	  	m_pDrawBuffer = QAD_LTDC::getLayer0BackBuffer();
	  	break;
	  case (QAD_LTDC_Layer1):
	  	m_pDrawBuffer = QAD_LTDC::getLayer1BackBuffer();
	  	break;
	}

	//Set currently selected draw buffer to Font Manager as well, to be used for text rendering methods
	m_cFontMgr.setDrawBuffer(m_pDrawBuffer);
}


//QAS_LCD::imp_setDrawColor
//QAS_LCD Rendering Setup Method
//
//To be called from either of the two static setDrawColor() methods
//Sets the current color to be used for rendering methods
void QAS_LCD::imp_setDrawColor(uint16_t uColor) {

	//Store new draw color
	m_uDrawColor = uColor;

	//Set currently selected draw color to Font Manager as well, to be used for text rendering methods
  m_cFontMgr.setDrawColor(m_uDrawColor);
}


  //-------------------------
  //-------------------------
  //QAS_LCD Rendering Methods

//QAS_LCD::imp_clearBuffer
//QAS_LCD Rendering Method
//
//To be called from static clearBuffer() method
//Used to clear the currently selected buffer to the currently selected draw color
void QAS_LCD::imp_clearBuffer(void) {
  for (uint32_t i=0; i<QAD_LTDC_PIXELCOUNT; i++)
  	m_pDrawBuffer->pixel[i] = m_uDrawColor;
}


//QAS_LCD::imp_drawPixel
//QAS_LCD Rendering Method
//
//To be called from static drawPixel() method
//Used to draw an individual pixel located at a specific position
//cPos - A QAT_Vector2_16 class containing the X and Y coordinates for the pixel to be drawn
void QAS_LCD::imp_drawPixel(QAT_Vector2_16& cPos) {
  m_pDrawBuffer->pixel[cPos.x + (cPos.y & QAD_LTDC_WIDTH)].pxl(m_uDrawColor);
}


//QAS_LCD::imp_drawLine
//QAS_LCD Rendering Method
//
//To be called from static drawLine() method
//Used to draw an single pixel width line between a start and end point
//Line will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart - A QAT_Vector2_16 class containing the X and Y coordinates for the line's start location
//cEnd   - A QAT_Vector2_16 class containing the X and Y coordinates for the line's end location
//
//Based on whether the line to be draw is horizontal, vertical or arbitrary, this method with then
//call the most suitable method to draw the line in the most optimized manner.
//Horizontal lines are more efficient to draw than vertical lines, and arbitrary (diagonal) lines
//are the least efficient.
void QAS_LCD::imp_drawLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {

	//If line is vertical then call imp_drawVLine
  if (cStart.x == cEnd.x)
  	imp_drawVLine(cStart, cEnd); else

  //If line is horizontal then call imp_drawHLine
  if (cStart.y == cEnd.y)
  	imp_drawHLine(cStart, cEnd); else

  //If line is neither horizontal or vertical the call imp_drawALine
    imp_drawALine(cStart, cEnd);
}


//QAS_LCD::imp_drawHLine
//QAS_LCD Rendering Method
//
//To be called from imp_drawLine method
//Used to draw a single pixel width horizontal line between a start and end point
//Line will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's start location
//cEnd   - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's end location
void QAS_LCD::imp_drawHLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {
  uint32_t xs;
  uint32_t xe;
  if (cStart.x < cEnd.x) {
  	xs = cStart.x;
    xe = cEnd.x;
  } else {
  	xs = cEnd.x;
  	xe = cStart.x;
  }

  uint32_t yofs = cStart.y * QAD_LTDC_WIDTH;
  for (uint32_t i=xs; i<(xe+1); i++) {
  	m_pDrawBuffer->pixel[i + yofs].pxl(m_uDrawColor);
  }
}


//QAS_LCD::imp_drawVLine
//QAS_LCD Rendering Method
//
//To be called from imp_drawLine method
//Used to draw a single pixel width vertical line between a start and end point
//Line will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's start location
//cEnd   - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's end location
void QAS_LCD::imp_drawVLine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {
  uint32_t ys;
  uint32_t ye;
  if (cStart.y < cEnd.y) {
  	ys = cStart.y;
  	ye = cEnd.y;
  } else {
  	ys = cEnd.y;
  	ye = cStart.y;
  }

  uint32_t xofs = cStart.x;
  for (uint32_t i=ys; i<(ye+1); i++) {
  	m_pDrawBuffer->pixel[(i * QAD_LTDC_WIDTH) + xofs].pxl(m_uDrawColor);
  }
}


//QAS_LCD::imp_drawALine
//QAS_LCD Rendering Method
//
//To be called from imp_drawLine method
//Used to draw a single pixel width line between a start and end point
//Line will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's start location
//cEnd   - A reference to a QAT_Vector2_16 class containing the X and Y coordinates for the line's end location
//
//This method implements a variation of Bresenham's line algorithm
//For more information on this it is worth checking out the following Wikipedia article:
//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void QAS_LCD::imp_drawALine(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {
  int16_t uDeltaX = QAS_LCD_ABS(cEnd.x-cStart.x);
  int16_t uDeltaY = QAS_LCD_ABS(cEnd.y-cStart.y);
  int16_t uX = cStart.x;
  int16_t uY = cStart.y;

  int16_t uXInc1;
  int16_t uXInc2;
  int16_t uYInc1;
  int16_t uYInc2;

  if (cEnd.x >= cStart.x) {
    uXInc1 = 1;
    uXInc2 = 1;
  } else {
    uXInc1 = -1;
    uXInc2 = -1;
  }

  if (cEnd.y >= cStart.y) {
    uYInc1 = 1;
    uYInc2 = 1;
  } else {
    uYInc1 = -1;
    uYInc2 = -1;
  }

  int16_t uDenominator;
  int16_t uNumerator;
  int16_t uNumAdd;
  int16_t uNumPixels;

  if (uDeltaX >= uDeltaY) {
    uXInc1       = 0;
    uYInc2       = 0;
    uDenominator = uDeltaX;
    uNumerator   = uDeltaX / 2;
    uNumAdd      = uDeltaY;
    uNumPixels   = uDeltaX;
  } else {
    uXInc2       = 0;
    uYInc1       = 0;
    uDenominator = uDeltaY;
    uNumerator   = uDeltaY / 2;
    uNumAdd      = uDeltaX;
    uNumPixels   = uDeltaY;
  }

  for (uint16_t i=0; i<uNumPixels; i++) {
    m_pDrawBuffer->pixel[uX+(uY*QAD_LTDC_WIDTH)].pxl(m_uDrawColor);

    uNumerator += uNumAdd;
    if (uNumerator >= uDenominator) {
      uNumerator -= uDenominator;
      uX += uXInc1;
      uY += uYInc1;
    }
    uX += uXInc2;
    uY += uYInc2;
  }
}


//QAS_LCD::imp_drawRect
//QAS_LCD Rendering Method
//
//To be called by static drawRect() method
//Used to draw a non-filled rectangle
//Rectangle will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart & cEnd - QAT_Vector2_16 classes that define X and Y coordinates of the diagonally opposing corners for the rectangle
void QAS_LCD::imp_drawRect(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {
  uint32_t xs;
  uint32_t xe;
  if (cStart.x < cEnd.x) {
    xs = cStart.x;
    xe = cEnd.x;
  } else {
    xs = cEnd.x;
    xe = cStart.x;
  }

  uint32_t ys;
  uint32_t ye;
  if (cStart.y < cEnd.y) {
    ys = cStart.y;
    ye = cEnd.y;
  } else {
    ys = cEnd.y;
    ye = cStart.y;
  }

  //Top & Bottom
  uint32_t yt = ys*QAD_LTDC_WIDTH;
  uint32_t yb = ye*QAD_LTDC_WIDTH;
  for (uint32_t x=xs; x<(xe+1); x++) {
    m_pDrawBuffer->pixel[x+yt].pxl(m_uDrawColor);
    m_pDrawBuffer->pixel[x+yb].pxl(m_uDrawColor);
  }

  //Left & Right
  for (uint32_t y=(ys+1); y<ye; y++) {
    m_pDrawBuffer->pixel[xs+y*QAD_LTDC_WIDTH].pxl(m_uDrawColor);
    m_pDrawBuffer->pixel[xe+y*QAD_LTDC_WIDTH].pxl(m_uDrawColor);
  }
}


//QAS_LCD::imp_drawRectFill
//QAS_LCD Rendering Method
//
//To be called by static drawRectFill() method
//Used to draw a filled rectangle
//Rectangle will be drawn to the currently selected draw buffer using the currently selected draw color
//cStart & cEnd - QAT_Vector2_16 classes that define X and Y coordinates of the diagonally opposing corder for the rectangle
void QAS_LCD::imp_drawRectFill(QAT_Vector2_16& cStart, QAT_Vector2_16& cEnd) {
  uint32_t xs;
  uint32_t xe;
  if (cStart.x < cEnd.x) {
    xs = cStart.x;
    xe = cEnd.x;
  } else {
    xs = cEnd.x;
    xe = cStart.x;
  }

  uint32_t ys;
  uint32_t ye;
  if (cStart.y < cEnd.y) {
    ys = cStart.y;
    ye = cEnd.y;
  } else {
    ys = cEnd.y;
    ye = cStart.y;
  }

  uint32_t yofs;
  for (uint32_t y=ys; y<(ye+1); y++) {
    yofs = y*QAD_LTDC_WIDTH;
    for (uint32_t x=xs; x<(xe+1); x++) {
      m_pDrawBuffer->pixel[x+yofs].pxl(m_uDrawColor);
    }
  }
}










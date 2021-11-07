/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: LTDC Driver                                                     */
/*   Filename: QAD_LTDC.hpp                                                */
/*   Date: 3rd October 2021                                                */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Prevent Recursive Inclusion
#ifndef __QAD_LTDC_HPP_
#define __QAD_LTDC_HPP_


//Includes
#include "setup.hpp"

#include "otm8009a.h"

#include "QAT_Pixel.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

#define QAD_LTDC_WIDTH       800
#define QAD_LTDC_HEIGHT      480
#define QAD_LTDC_PIXELCOUNT  (QAD_LTDC_WIDTH * QAD_LTDC_HEIGHT)

#define QAD_LTDC_BUFFERSIZE  (QAD_LTDC_PIXELCOUNT * sizeof(QAT_PixelARGB4444))


  //-----------------------
  //Frame Buffer Structures

enum QAD_LTDC_LayerIdx  : uint8_t {Layer0 = 0, Layer1};
enum QAD_LTDC_BufferIdx : uint8_t {Buffer0 = 0, Buffer1};


typedef struct {
	QAT_PixelARGB4444 pixel[QAD_LTDC_PIXELCOUNT];
} QAD_LTDC_Buffer;


typedef struct {
	QAD_LTDC_Buffer buffers[2];
} QAD_LTDC_DoubleBuffer;



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

//--------
//QAD_LTDC
class QAD_LTDC {
private:

	QA_InitState m_eInitState;

	const uint32_t m_uLCDClockRate    = 27429;
	const uint32_t m_uLCDLaneByteClk  = 62500; //kHz
	const uint32_t m_uLCDClkDivisor   = 15625;

	DSI_HandleTypeDef  m_sDSIHandle;
	DSI_VidCfgTypeDef  m_sDSICfgHandle;
	LTDC_HandleTypeDef m_sLTDCHandle;

	void*              m_pLayer0Ptr;
	void*              m_pLayer1Ptr;

  QAD_LTDC_BufferIdx m_eLayer0Back;
  QAD_LTDC_BufferIdx m_eLayer0Front;
  QAD_LTDC_BufferIdx m_eLayer1Back;
  QAD_LTDC_BufferIdx m_eLayer1Front;

	QAD_LTDC() :
	  m_eInitState(QA_NotInitialized) {}

public:

	QAD_LTDC(const QAD_LTDC& other) = delete;
	QAD_LTDC& operator=(const QAD_LTDC& other) = delete;

	static QAD_LTDC& get(void) {
		static QAD_LTDC instance;
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

	static void dsi_io_writecmd(uint32_t uNumParams, uint8_t* pParams) {
		get().imp_dsi_io_writecmd(uNumParams, pParams);
	}

	  //---------------
	  //Control Methods

	static void lcdOn(void) {
		get().imp_lcdOn();
	}

	static void lcdOff(void) {
		get().imp_lcdOff();
	}

	static void setBrightness(uint8_t uBrightness) {
		get().imp_setBrightness(uBrightness);
	}

    //---------------------
	  //Layer Control Methods

	static void flipLayer0Buffers(void) {
		get().imp_flipLayer0Buffers();
	}

	static void flipLayer1Buffers(void) {
		get().imp_flipLayer1Buffers();
	}

	  //------------------
	  //Layer Data Methods

	static void* getLayer0BackBufferPtr(void) {
		return get().imp_getLayer0BackBufferPtr();
	}

	static QAD_LTDC_Buffer* getLayer0BackBuffer(void) {
		return get().imp_getLayer0BackBuffer();
	}

	static void* getLayer1BackBufferPtr(void) {
		return get().imp_getLayer1BackBufferPtr();
	}

	static QAD_LTDC_Buffer* getLayer1BackBuffer(void) {
		return get().imp_getLayer1BackBuffer();
	}


private:

	//Initialization Methods
	QA_Result imp_init(void);
	void imp_deinit(void);

	//DSI Command Methods
	void imp_dsi_io_writecmd(uint32_t uNumParams, uint8_t* pParams);

	//Control Methods
	void imp_lcdOn(void);
	void imp_lcdOff(void);
	void imp_setBrightness(uint8_t uBrightness);

	//Layer Control Methods
	void imp_flipLayer0Buffers(void);
	void imp_flipLayer1Buffers(void);

	//Layer Data Methods
	void* imp_getLayer0BackBufferPtr(void);
	QAD_LTDC_Buffer* imp_getLayer0BackBuffer(void);
	void* imp_getLayer1BackBufferPtr(void);
	QAD_LTDC_Buffer* imp_getLayer1BackBuffer(void);

};



//DSI Command Method
void DSI_IO_WriteCmd(uint32_t NbrParam, uint8_t* pParams);


//Prevent Recursive Inclusion
#endif /* __QAD_LTDC_HPP */






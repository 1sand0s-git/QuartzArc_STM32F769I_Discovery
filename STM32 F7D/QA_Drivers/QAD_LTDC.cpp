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

//Includes
#include "QAD_LTDC.hpp"



	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //-------------------------------
  //QAD_LTDC Initialization Methods


//QAD_LTDC::imp_init
//QAD_LTDC Initialization Method
QA_Result QAD_LTDC::imp_init(void) {

  //Init GPIOs
	GPIO_InitTypeDef GPIO_Init = {0};

	  //Backlight Control Pin
	GPIO_Init.Pin    = QAD_LTDC_BACKLIGHT_PIN;
	GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull   = GPIO_PULLDOWN;
	GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(QAD_LTDC_BACKLIGHT_PORT, &GPIO_Init);

		//Reset Pin
	GPIO_Init.Pin    = QAD_LTDC_RESET_PIN;
	GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull   = GPIO_PULLUP;
	GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(QAD_LTDC_RESET_PORT, &GPIO_Init);


	//Reset LCD
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_SET);
	HAL_Delay(10);


	//Initialize Peripheral Clocks
	__HAL_RCC_LTDC_CLK_ENABLE();
	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_LTDC_RELEASE_RESET();

	__HAL_RCC_DSI_CLK_ENABLE();
	__HAL_RCC_DSI_FORCE_RESET();
	__HAL_RCC_DSI_RELEASE_RESET();


  //--------
  //Init DSI

  m_sDSIHandle.Instance = DSI;
  HAL_DSI_DeInit(&m_sDSIHandle);

    //DSI PLL
  DSI_PLLInitTypeDef DSI_PLLInit = {0};
  DSI_PLLInit.PLLNDIV = 100;
  DSI_PLLInit.PLLIDF  = DSI_PLL_IN_DIV5;
  DSI_PLLInit.PLLODF  = DSI_PLL_OUT_DIV1;

  m_sDSIHandle.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  m_sDSIHandle.Init.TXEscapeCkdiv = m_uLCDLaneByteClk / m_uLCDClkDivisor;
  HAL_DSI_Init(&m_sDSIHandle, &DSI_PLLInit);

    //DSI Vid Config
  uint32_t HACT = QAD_LTDC_WIDTH;    //Horizontal Active
  uint32_t VACT = QAD_LTDC_HEIGHT;   //Vertical Active
  uint32_t VSA  = 12;                //Vertical Sync Active
  uint32_t VBP  = 12;                //Vertical Back Porch
  uint32_t VFP  = 12;                //Vertical Front Porch
  uint32_t HSA  = 63;                //Horizontal Sync Active
  uint32_t HBP  = 120;               //Horizontal Back Porch
  uint32_t HFP  = 120;               //Horizontal Front Porch

  m_sDSICfgHandle.VirtualChannelID             = 0;
  m_sDSICfgHandle.ColorCoding                  = DSI_RGB888;
  m_sDSICfgHandle.VSPolarity                   = DSI_VSYNC_ACTIVE_HIGH;
  m_sDSICfgHandle.HSPolarity                   = DSI_HSYNC_ACTIVE_HIGH;
  m_sDSICfgHandle.DEPolarity                   = DSI_DATA_ENABLE_ACTIVE_HIGH;
  m_sDSICfgHandle.Mode                         = DSI_VID_MODE_BURST;
  m_sDSICfgHandle.NullPacketSize               = 0xFFF;
  m_sDSICfgHandle.NumberOfChunks               = 0;
  m_sDSICfgHandle.PacketSize                   = HACT;
  m_sDSICfgHandle.HorizontalSyncActive         = (HSA * m_uLCDLaneByteClk) / m_uLCDClockRate;
  m_sDSICfgHandle.HorizontalBackPorch          = (HSA * m_uLCDLaneByteClk) / m_uLCDClockRate;
  m_sDSICfgHandle.HorizontalLine               = ((HACT + HSA + HBP + HFP) * m_uLCDLaneByteClk) / m_uLCDClockRate;
  m_sDSICfgHandle.VerticalSyncActive           = VSA;
  m_sDSICfgHandle.VerticalBackPorch            = VBP;
  m_sDSICfgHandle.VerticalFrontPorch           = VFP;
  m_sDSICfgHandle.VerticalActive               = VACT;

  m_sDSICfgHandle.LPCommandEnable              = DSI_LP_COMMAND_ENABLE;
  m_sDSICfgHandle.LPLargestPacketSize          = 16;
  m_sDSICfgHandle.LPVACTLargestPacketSize      = 0;
  m_sDSICfgHandle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;
  m_sDSICfgHandle.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;
  m_sDSICfgHandle.LPVerticalActiveEnable       = DSI_LP_VACT_ENABLE;
  m_sDSICfgHandle.LPVerticalFrontPorchEnable   = DSI_LP_VFP_ENABLE;
  m_sDSICfgHandle.LPVerticalBackPorchEnable    = DSI_LP_VBP_ENABLE;
  m_sDSICfgHandle.LPVerticalSyncActiveEnable   = DSI_LP_VSYNC_ENABLE;
  HAL_DSI_ConfigVideoMode(&m_sDSIHandle, &m_sDSICfgHandle);

  //---------
  //Init LTDC
  LTDC_LayerCfgTypeDef LTDC_Layer = {0};

    //Peripheral
  m_sLTDCHandle.Instance                = LTDC;
  m_sLTDCHandle.Init.HorizontalSync     = (HSA - 1);
  m_sLTDCHandle.Init.AccumulatedHBP     = (HSA + HBP - 1);
  m_sLTDCHandle.Init.AccumulatedActiveW = (QAD_LTDC_WIDTH + HSA + HBP - 1);
  m_sLTDCHandle.Init.TotalWidth         = (QAD_LTDC_WIDTH + HSA + HBP + HFP - 1);
  m_sLTDCHandle.LayerCfg->ImageWidth    = QAD_LTDC_WIDTH;
  m_sLTDCHandle.LayerCfg->ImageHeight   = QAD_LTDC_HEIGHT;
  m_sLTDCHandle.Init.Backcolor.Red      = 0;
  m_sLTDCHandle.Init.Backcolor.Green    = 0;
  m_sLTDCHandle.Init.Backcolor.Blue     = 255;
  m_sLTDCHandle.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;
  HAL_LTDC_StructInitFromVideoConfig(&m_sLTDCHandle, &m_sDSICfgHandle);
  HAL_LTDC_Init(&m_sLTDCHandle);

    //Init Layer 0
  m_pLayer0Ptr = (void*)0xC0000000;

	LTDC_Layer.WindowX0        = 0;
	LTDC_Layer.WindowX1        = QAD_LTDC_WIDTH;
	LTDC_Layer.WindowY0        = 0;
	LTDC_Layer.WindowY1        = QAD_LTDC_HEIGHT;
	LTDC_Layer.PixelFormat     = LTDC_PIXEL_FORMAT_ARGB4444;
	LTDC_Layer.FBStartAdress   = (uint32_t)m_pLayer0Ptr;
	LTDC_Layer.Alpha           = 255;
	LTDC_Layer.Alpha0          = 0;
	LTDC_Layer.Backcolor.Red   = 0;
	LTDC_Layer.Backcolor.Green = 0;
	LTDC_Layer.Backcolor.Blue  = 0;
	LTDC_Layer.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	LTDC_Layer.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	LTDC_Layer.ImageWidth      = QAD_LTDC_WIDTH;
	LTDC_Layer.ImageHeight     = QAD_LTDC_HEIGHT;
	HAL_LTDC_ConfigLayer(&m_sLTDCHandle, &LTDC_Layer, 0);
	__HAL_LTDC_LAYER_ENABLE(&m_sLTDCHandle, 0);

  //Init Layer 1
	m_pLayer1Ptr = (void*)((uint32_t)m_pLayer0Ptr + (QAD_LTDC_BUFFERSIZE*2));

	LTDC_Layer.WindowX0        = 0;
	LTDC_Layer.WindowX1        = QAD_LTDC_WIDTH;
	LTDC_Layer.WindowY0        = 0;
	LTDC_Layer.WindowY1        = QAD_LTDC_HEIGHT;
	LTDC_Layer.PixelFormat     = LTDC_PIXEL_FORMAT_ARGB4444;
	LTDC_Layer.FBStartAdress   = (uint32_t)m_pLayer1Ptr;
	LTDC_Layer.Alpha           = 255;
	LTDC_Layer.Alpha0          = 0;
	LTDC_Layer.Backcolor.Red   = 0;
	LTDC_Layer.Backcolor.Green = 0;
	LTDC_Layer.Backcolor.Blue  = 0;
	LTDC_Layer.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	LTDC_Layer.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	LTDC_Layer.ImageWidth      = QAD_LTDC_WIDTH;
	LTDC_Layer.ImageHeight     = QAD_LTDC_HEIGHT;
	HAL_LTDC_ConfigLayer(&m_sLTDCHandle, &LTDC_Layer, 1);
	__HAL_LTDC_LAYER_ENABLE(&m_sLTDCHandle, 1);


	//----------
	//Enable DSI
	HAL_DSI_Start(&m_sDSIHandle);


	//------------------------------
	//Initialize OTM8009A Controller
	OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);


	//Set Buffer States
	m_eLayer0Back  = Buffer1;
	m_eLayer0Front = Buffer0;
	m_eLayer1Back  = Buffer1;
	m_eLayer1Front = Buffer0;

	//Set State
	m_eInitState = QA_Initialized;

	//Return
	return QA_OK;
}


//QAD_LTDC::imp_deinit
//QAD_LTDC Initialization Method
void QAD_LTDC::imp_deinit(void) {

	//Disable DSI
	HAL_DSI_Stop(&m_sDSIHandle);

	//Disable LTDC Layers
	__HAL_LTDC_LAYER_DISABLE(&m_sLTDCHandle, 1);
	__HAL_LTDC_LAYER_DISABLE(&m_sLTDCHandle, 0);

	//Deinit LTDC
	HAL_LTDC_DeInit(&m_sLTDCHandle);

	//Deinit DSI
	HAL_DSI_DeInit(&m_sDSIHandle);

	//Disable Clocks
	__HAL_RCC_DSI_CLK_DISABLE();
	__HAL_RCC_LTDC_CLK_DISABLE();

	//Reset LCD
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_SET);
	HAL_Delay(10);

}


  //-------------------
  //DSI Command Methods


//QAD_LTDC::imp_dsi_io_writecmd
//DSI Command Methods
void QAD_LTDC::imp_dsi_io_writecmd(uint32_t uNumParams, uint8_t* pParams) {
	if (uNumParams <= 1) {
		HAL_DSI_ShortWrite(&m_sDSIHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
	} else {
		HAL_DSI_LongWrite(&m_sDSIHandle, 0, DSI_DCS_LONG_PKT_WRITE, uNumParams, pParams[uNumParams], pParams);
	}
}


//DSI_IO_WriteCmd
//DSI Command Method
void DSI_IO_WriteCmd(uint32_t NbrParam, uint8_t* pParams) {
  QAD_LTDC::dsi_io_writecmd(NbrParam, pParams);
}


  //------------------------
  //QAD_LTDC Control Methods

//QAD_LTDC::imp_lcdOn
//QAD_LTDC Control Method
void QAD_LTDC::imp_lcdOn(void) {
	HAL_DSI_ShortWrite(&m_sDSIHandle, m_sDSICfgHandle.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON, 0x00);
	HAL_GPIO_WritePin(QAD_LTDC_BACKLIGHT_PORT, QAD_LTDC_BACKLIGHT_PIN, GPIO_PIN_SET);
}


//QAD_LTDC::imp_lcdOff
//QAD_LTDC Control Method
void QAD_LTDC::imp_lcdOff(void) {
	HAL_DSI_ShortWrite(&m_sDSIHandle, m_sDSICfgHandle.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF, 0x00);
	HAL_GPIO_WritePin(QAD_LTDC_BACKLIGHT_PORT, QAD_LTDC_BACKLIGHT_PIN, GPIO_PIN_RESET);
}


//QAD_LTDC::imp_setBrightness
//QAD_LTDC Control Method
void QAD_LTDC::imp_setBrightness(uint8_t uBrightness) {
	HAL_DSI_ShortWrite(&m_sDSIHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_WRDISBV, (uint16_t)(uBrightness * 255)/100);
}


  //------------------------------
  //QAD_LTDC Layer Control Methods

//QAD_LTDC::imp_flipLayer0Buffers
//QAD_LTDC Layer Control Method
void QAD_LTDC::imp_flipLayer0Buffers(void) {
  if (!m_eInitState)
  	return;

  switch (m_eLayer0Front) {
    case (Buffer0): {
    	m_eLayer0Back  = Buffer0;
    	m_eLayer0Front = Buffer1;
    	uint32_t uAddr = ((uint32_t)m_pLayer0Ptr + QAD_LTDC_BUFFERSIZE);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 0);
    	break;
    }
    case (Buffer1): {
    	m_eLayer0Back  = Buffer1;
    	m_eLayer0Front = Buffer0;
    	uint32_t uAddr = ((uint32_t)m_pLayer0Ptr);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 0);
    	break;
    }
  }
}


//QAD_LTDC::imp_flipLayer1Buffers
//QAD_LTDC Layer Control Method
void QAD_LTDC::imp_flipLayer1Buffers(void) {
  if (!m_eInitState)
  	return;

  switch (m_eLayer1Front) {
    case (Buffer0): {
    	m_eLayer1Back  = Buffer0;
    	m_eLayer1Front = Buffer1;
    	uint32_t uAddr = ((uint32_t)m_pLayer1Ptr + QAD_LTDC_BUFFERSIZE);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 1);
    	break;
    }
    case (Buffer1): {
    	m_eLayer1Back  = Buffer1;
    	m_eLayer1Front = Buffer0;
    	uint32_t uAddr = ((uint32_t)m_pLayer1Ptr);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 1);
    	break;
    }
  }
}


  //---------------------------
  //QAD_LTDC Layer Data Methods

//QAD_LTDC::imp_getLayer0BackBufferPtr
//QAD_LTDC Layer Data Method
void* QAD_LTDC::imp_getLayer0BackBufferPtr(void) {
	if (!m_eInitState)
		return NULL;

	switch (m_eLayer0Back) {
	  case (Buffer0):
	  	return m_pLayer0Ptr;
	  	break;
	  case (Buffer1):
	  	return (void*)((uint32_t)m_pLayer0Ptr + QAD_LTDC_BUFFERSIZE);
	  	break;
	}

	return NULL;
}


//QAD_LTDC::imp_getLayer0BackBuffer
//QAD_LTDC Layer Data Method
QAD_LTDC_Buffer* QAD_LTDC::imp_getLayer0BackBuffer(void) {
	if (!m_eInitState)
		return NULL;

	switch (m_eLayer0Back) {
	  case (Buffer0):
	  	return (QAD_LTDC_Buffer*)m_pLayer0Ptr;
	  	break;
	  case (Buffer1):
	  	return (QAD_LTDC_Buffer*)((uint32_t)m_pLayer0Ptr + QAD_LTDC_BUFFERSIZE);
	  	break;
	}

	return NULL;
}


//QAD_LTDC::imp_getLayer1BackBufferPtr
//QAD_LTDC Layer Data Method
void* QAD_LTDC::imp_getLayer1BackBufferPtr(void) {
	if (!m_eInitState)
		return NULL;

	switch (m_eLayer1Back) {
	  case (Buffer0):
	  	return m_pLayer1Ptr;
	  	break;
	  case (Buffer1):
	  	return (void*)((uint32_t)m_pLayer1Ptr + QAD_LTDC_BUFFERSIZE);
	  	break;
	}

	return NULL;
}


//QAD_LTDC::imp_getLayer1BackBuffer
//QAD_LTDC Layer Data Method
QAD_LTDC_Buffer* QAD_LTDC::imp_getLayer1BackBuffer(void) {
	if (!m_eInitState)
		return NULL;

	switch (m_eLayer1Back) {
	  case (Buffer0):
	  	return (QAD_LTDC_Buffer*)m_pLayer1Ptr;
	  	break;
	  case (Buffer1):
	  	return (QAD_LTDC_Buffer*)((uint32_t)m_pLayer1Ptr + QAD_LTDC_BUFFERSIZE);
	  	break;
	}

	return NULL;
}













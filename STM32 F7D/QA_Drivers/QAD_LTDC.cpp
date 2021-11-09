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
//
//To be called from static method init()
//Used to initialize the DSI and LTDC peripherals, LTDC layers, the OTM8009A display controller and the required reset and backlight control GPIO pins
//Returns QA_OK if initialization successful, or QA_Fail if initialization fails.
QA_Result QAD_LTDC::imp_init(void) {

  //Initialize GPIOs
	GPIO_InitTypeDef GPIO_Init = {0};

	  //Initialize Backlight Control Pin
	GPIO_Init.Pin    = QAD_LTDC_BACKLIGHT_PIN;          //Set pin number. (QAD_LTDC_BACKLIGHT_PIN is defined in setup.hpp)
	GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;             //Set pin mode to output in push/pull mode
	GPIO_Init.Pull   = GPIO_PULLDOWN;                   //Enable pull-down resistor so backlight defaults to off
	GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;             //Set pin's output speed to low
	HAL_GPIO_Init(QAD_LTDC_BACKLIGHT_PORT, &GPIO_Init); //Initialize pin. (QAD_LTDC_BACKLIGHT_PORT is defined in setup.hpp)

		//Initialize OTM8009A Controller Reset Pin
	GPIO_Init.Pin    = QAD_LTDC_RESET_PIN;              //Set pin number. (QAD_LTDC_RESET_PIN is defined in setup.hpp)
	GPIO_Init.Mode   = GPIO_MODE_OUTPUT_PP;             //Set pin mode to output in push/pull mode
	GPIO_Init.Pull   = GPIO_PULLUP;                     //Enable pull-up resistor as reset is active-low
	GPIO_Init.Speed  = GPIO_SPEED_FREQ_LOW;             //Set pin's output speed to low
	HAL_GPIO_Init(QAD_LTDC_RESET_PORT, &GPIO_Init);     //Initialize pin. (QAD_LTDC_RESET_PORT is defined in setup.hpp)


	//Reset OTM8009A Controller
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_RESET); //Pull reset pin low
	HAL_Delay(20);                                                              //Delay for 20ms to allow for reset process
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_SET);   //Pull reset pin high
	HAL_Delay(10);                                                              //Delay for 10ms to allow reset process time to complete


	//Initialize LTDC Peripheral Clock
	__HAL_RCC_LTDC_CLK_ENABLE();
	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_LTDC_RELEASE_RESET();

	//Initialize DSI Peripheral Clock
	__HAL_RCC_DSI_CLK_ENABLE();
	__HAL_RCC_DSI_FORCE_RESET();
	__HAL_RCC_DSI_RELEASE_RESET();

	//Prepare Instances for DSI and LTDC handles (required if needing to call imp_deinit() due to failed initialization)
  m_sDSIHandle.Instance = DSI;
  m_sLTDCHandle.Instance = LTDC;


  //-------------------------
  //Initialize DSI peripheral

  //Confirm DSI is currently uninitialized
  HAL_DSI_DeInit(&m_sDSIHandle);

  //Fill out DSI PLL Initialization structure as required for DSI to operate at required clock speed
  DSI_PLLInitTypeDef DSI_PLLInit = {0};
  DSI_PLLInit.PLLNDIV = 100;
  DSI_PLLInit.PLLIDF  = DSI_PLL_IN_DIV5;
  DSI_PLLInit.PLLODF  = DSI_PLL_OUT_DIV1;

  //Fill out required DSI initialization details
  m_sDSIHandle.Init.NumberOfLanes = DSI_TWO_DATA_LANES;                   //Set DSI as using two data lanes
  m_sDSIHandle.Init.TXEscapeCkdiv = m_uLCDLaneByteClk / m_uLCDClkDivisor; //Set TX Escape Clock Division

  //Initialize DSI, calling imp_deinit() if initialization fails
  if (HAL_DSI_Init(&m_sDSIHandle, &DSI_PLLInit) != HAL_OK) {
  	imp_deinit();
  	return QA_Fail;
  }


  //----------------------------------
  //Initialize DSI video configuration

  //Generate base DSI timing parameters
  uint32_t HACT = QAD_LTDC_WIDTH;    //Horizontal Active period in pixels
  uint32_t VACT = QAD_LTDC_HEIGHT;   //Vertical Active period in pixels
  uint32_t VSA  = 12;                //Period that vertical sync is active in pixels
  uint32_t VBP  = 12;                //Vertical Back Porch in pixels
  uint32_t VFP  = 12;                //Vertical Front Porch in pixels
  uint32_t HSA  = 63;                //Period that horizontal sync is active in pixels
  uint32_t HBP  = 120;               //Horizontal Back Porch in pixels
  uint32_t HFP  = 120;               //Horizontal Front Porch in pixels

  //Fill out details of DSI Configuration Handle
  m_sDSICfgHandle.VirtualChannelID             = 0;                                      //Set virtual channel ID
  m_sDSICfgHandle.ColorCoding                  = DSI_RGB888;                             //Set pixel format to 24bit RGB (8bits for each Red, Green and Blue)
  m_sDSICfgHandle.VSPolarity                   = DSI_VSYNC_ACTIVE_HIGH;                  //Set Vertical Sync signal to active high
  m_sDSICfgHandle.HSPolarity                   = DSI_HSYNC_ACTIVE_HIGH;                  //Set Horizontal Sync signal to active high
  m_sDSICfgHandle.DEPolarity                   = DSI_DATA_ENABLE_ACTIVE_HIGH;            //Set Data Enable signal to active high
  m_sDSICfgHandle.Mode                         = DSI_VID_MODE_BURST;                     //Set Video Mode to burst mode
  m_sDSICfgHandle.NullPacketSize               = 0xFFF;                                  //Set null packet size
  m_sDSICfgHandle.NumberOfChunks               = 0;                                      //Set number of chunks
  m_sDSICfgHandle.PacketSize                   = HACT;                                   //Set packet size to horizontal active period

  m_sDSICfgHandle.HorizontalSyncActive         = (HSA * m_uLCDLaneByteClk) /             //Set duration for horizontal sync signal (in lane byte clock cycles)
  		                                           m_uLCDClockRate;

  m_sDSICfgHandle.HorizontalBackPorch          = (HSA * m_uLCDLaneByteClk) /             //Set duration for horizontal back porch (in lane byte clock cycles)
  		                                           m_uLCDClockRate;

  m_sDSICfgHandle.HorizontalLine               = ((HACT + HSA + HBP + HFP) *             //Set duration for a horizontal line (in lane byte clock cycles)
  		                                           m_uLCDLaneByteClk) / m_uLCDClockRate;

  m_sDSICfgHandle.VerticalSyncActive           = VSA;                                    //Set duration for vertical sync signal
  m_sDSICfgHandle.VerticalBackPorch            = VBP;                                    //Set duration for vertical back porch
  m_sDSICfgHandle.VerticalFrontPorch           = VFP;                                    //Set duration for vertical front porch
  m_sDSICfgHandle.VerticalActive               = VACT;                                   //Set duration of vertical active period

  m_sDSICfgHandle.LPCommandEnable              = DSI_LP_COMMAND_ENABLE;  //Enable DSI low power command
  m_sDSICfgHandle.LPLargestPacketSize          = 16;                     //Set largest packet size for low power commands during VSA, VBP and VFP periods.
  m_sDSICfgHandle.LPVACTLargestPacketSize      = 0;                      //Set largest packet size for low power commands during VACT period
  m_sDSICfgHandle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;      //Enable low power horizontal front porch
  m_sDSICfgHandle.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;      //Enable low power horizontal back porch
  m_sDSICfgHandle.LPVerticalActiveEnable       = DSI_LP_VACT_ENABLE;     //Enable low power vertical active
  m_sDSICfgHandle.LPVerticalFrontPorchEnable   = DSI_LP_VFP_ENABLE;      //Enable low power vertical front porch
  m_sDSICfgHandle.LPVerticalBackPorchEnable    = DSI_LP_VBP_ENABLE;      //Enable low power vertical back porch
  m_sDSICfgHandle.LPVerticalSyncActiveEnable   = DSI_LP_VSYNC_ENABLE;    //Enable low power vertical sync

  //Configure DSI Video Mode,
  if (HAL_DSI_ConfigVideoMode(&m_sDSIHandle, &m_sDSICfgHandle) != HAL_OK) {
  	imp_deinit();
  	return QA_Fail;
  }


  //--------------------------
  //Initialize LTDC Peripheral

  //Fill out details for LTDC initialization
  m_sLTDCHandle.Init.HorizontalSync     = (HSA - 1);                              //Set horizontal syncronization width
  m_sLTDCHandle.Init.AccumulatedHBP     = (HSA + HBP - 1);                        //Set accumulated horizontal back porch width
  m_sLTDCHandle.Init.AccumulatedActiveW = (QAD_LTDC_WIDTH + HSA + HBP - 1);       //Set accumulated active width
  m_sLTDCHandle.Init.TotalWidth         = (QAD_LTDC_WIDTH + HSA + HBP + HFP - 1); //Set total width
  m_sLTDCHandle.LayerCfg->ImageWidth    = QAD_LTDC_WIDTH;                         //Sets the frame buffer line length (in pixels)
  m_sLTDCHandle.LayerCfg->ImageHeight   = QAD_LTDC_HEIGHT;                        //Sets the number of frame buffer lines (height in pixels)
  m_sLTDCHandle.Init.Backcolor.Red      = 0;                                      //Set background color red value
  m_sLTDCHandle.Init.Backcolor.Green    = 0;                                      //Set background color green value
  m_sLTDCHandle.Init.Backcolor.Blue     = 255;                                    //Set background color blue value
  m_sLTDCHandle.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;                    //Set pixel clock polarity to non-inverted

  //Complete initialization structre details based on DSI Configuration, calling imp_deinit() if failed
  if (HAL_LTDC_StructInitFromVideoConfig(&m_sLTDCHandle, &m_sDSICfgHandle) != HAL_OK) {
  	imp_deinit();
  	return QA_Fail;
  }

  //Initialize LTDC Peripheral
  if (HAL_LTDC_Init(&m_sLTDCHandle) != HAL_OK) {
  	imp_deinit();
  	return QA_Fail;
  }


  //----------------------
  //Initialize LTDC Layers
  LTDC_LayerCfgTypeDef LTDC_Layer;

    //-------
    //Layer 0

  //Set pointer address for layer 0 double buffer
  m_pLayer0Ptr = (void*)0xC0000000;

  //Clear Layer Config structure
  LTDC_Layer = {0};

    //Set Layer as filling entire 800x480 display area of LCD
	LTDC_Layer.WindowX0        = 0;
	LTDC_Layer.WindowX1        = QAD_LTDC_WIDTH;
	LTDC_Layer.WindowY0        = 0;
	LTDC_Layer.WindowY1        = QAD_LTDC_HEIGHT;

	LTDC_Layer.PixelFormat     = LTDC_PIXEL_FORMAT_ARGB4444;   //Set pixel format as 16bits per pixel (ARGB4444)
	LTDC_Layer.FBStartAdress   = (uint32_t)m_pLayer0Ptr;       //Set address of initial front buffer for layer 1
	LTDC_Layer.Alpha           = 255;                          //Specify constant Alpha value used for blending
	LTDC_Layer.Alpha0          = 0;                            //Specify default Alpha value

	  //Set Layer background colour to black
	LTDC_Layer.Backcolor.Red   = 0;
	LTDC_Layer.Backcolor.Green = 0;
	LTDC_Layer.Backcolor.Blue  = 0;

	LTDC_Layer.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;  //Set Blending Factor 1 as Pixel Alpha * Constant Alpha
	LTDC_Layer.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;  //Set Blending Factor 2 as Pixel Alpha * Constant Alpha
	LTDC_Layer.ImageWidth      = QAD_LTDC_WIDTH;               //Set frame buffer width
	LTDC_Layer.ImageHeight     = QAD_LTDC_HEIGHT;              //Set frame buffer height

	//Configure layer 0 using required settings, calling imp_deinit() if initialization fails
	if (HAL_LTDC_ConfigLayer(&m_sLTDCHandle, &LTDC_Layer, 0) != HAL_OK) {
		imp_deinit();
		return QA_Fail;
	}


	  //-------
	  //Layer 1

  //Set pointer address for layer 1 doube buffer
	m_pLayer1Ptr = (void*)((uint32_t)m_pLayer0Ptr + (QAD_LTDC_BUFFERSIZE*2));

  //Clear Layer Config structure
  LTDC_Layer = {0};

    //Set Layer as filling entire 800x480 display area of LCD
	LTDC_Layer.WindowX0        = 0;
	LTDC_Layer.WindowX1        = QAD_LTDC_WIDTH;
	LTDC_Layer.WindowY0        = 0;
	LTDC_Layer.WindowY1        = QAD_LTDC_HEIGHT;

	LTDC_Layer.PixelFormat     = LTDC_PIXEL_FORMAT_ARGB4444;   //Set pixel format as 16bits per pixel (ARGB4444)
	LTDC_Layer.FBStartAdress   = (uint32_t)m_pLayer0Ptr;       //Set address of initial front buffer for layer 1
	LTDC_Layer.Alpha           = 255;                          //Specify constant Alpha value used for blending
	LTDC_Layer.Alpha0          = 0;                            //Specify default Alpha value

	  //Set Layer background colour to black
	LTDC_Layer.Backcolor.Red   = 0;
	LTDC_Layer.Backcolor.Green = 0;
	LTDC_Layer.Backcolor.Blue  = 0;

	LTDC_Layer.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;  //Set Blending Factor 1 as Pixel Alpha * Constant Alpha
	LTDC_Layer.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;  //Set Blending Factor 2 as Pixel Alpha * Constant Alpha
	LTDC_Layer.ImageWidth      = QAD_LTDC_WIDTH;               //Set frame buffer width
	LTDC_Layer.ImageHeight     = QAD_LTDC_HEIGHT;              //Set frame buffer height

	//Configure layer 1 using required settings, calling imp_deinit() if initialization fails
	if (HAL_LTDC_ConfigLayer(&m_sLTDCHandle, &LTDC_Layer, 1) != HAL_OK) {
		imp_deinit();
		return QA_Fail;
	}


	//-------------------------------
	//Enable LTDC Layer 0 and Layer 1
	__HAL_LTDC_LAYER_ENABLE(&m_sLTDCHandle, 0);
	__HAL_LTDC_LAYER_ENABLE(&m_sLTDCHandle, 1);


	//---------------------
	//Enable DSI Peripheral
	HAL_DSI_Start(&m_sDSIHandle);


	//--------------------------------------
	//Initialize OTM8009A Display Controller
	OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);


	//-------------------------------
	//Set Initial Frame Buffer States
	m_eLayer0Back  = QAD_LTDC_Buffer1;   //Set initial layer 0 back buffer as being buffer 1 in double buffer pair
	m_eLayer0Front = QAD_LTDC_Buffer0;   //Set initial layer 0 front buffer as being buffer 0 in double buffer pair
	m_eLayer1Back  = QAD_LTDC_Buffer1;   //Set initial layer 1 back buffer as being buffer 1 in double buffer pair
	m_eLayer1Front = QAD_LTDC_Buffer0;   //Set initial layer 1 front buffer as being buffer 0 in double buffer pair


	//Set Driver State as being initialized
	m_eInitState = QA_Initialized;

	//Return
	return QA_OK;
}


//QAD_LTDC::imp_deinit
//QAD_LTDC Initialization Method
//
//To be called from static method deinit(), or from imp_init() in the event of a failed initialization
//Used to deinitialize the DSI and LTDC peripherals, LTDC layers, reset display controller and deinitialize reset and backlight GPIO pins
void QAD_LTDC::imp_deinit(void) {

	//----------------------
	//Disable DSI Peripheral
	HAL_DSI_Stop(&m_sDSIHandle);


	//-------------------
	//Disable LTDC Layers
	__HAL_LTDC_LAYER_DISABLE(&m_sLTDCHandle, 1);
	__HAL_LTDC_LAYER_DISABLE(&m_sLTDCHandle, 0);


	//----------------------------
	//Deinitialize LTDC Peripheral
	HAL_LTDC_DeInit(&m_sLTDCHandle);


	//---------------------------
	//Deinitialize DSI Peripheral
	HAL_DSI_DeInit(&m_sDSIHandle);


	//---------------------------
	//Disable DSI and LTDC Clocks
	__HAL_RCC_DSI_CLK_DISABLE();
	__HAL_RCC_LTDC_CLK_DISABLE();


	//------------------------
	//Reset Display controller
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN, GPIO_PIN_SET);
	HAL_Delay(10);


	//--------------------------------------------------
	//Deinitialize Reset and Backlight control GPIO Pins
	HAL_GPIO_DeInit(QAD_LTDC_BACKLIGHT_PORT, QAD_LTDC_BACKLIGHT_PIN);
	HAL_GPIO_DeInit(QAD_LTDC_RESET_PORT, QAD_LTDC_RESET_PIN);

}


  //-------------------
  //-------------------
  //DSI Command Methods


//QAD_LTDC::imp_dsi_IO_WriteCmd
//DSI Command Methods
void QAD_LTDC::imp_dsi_IO_WriteCmd(uint32_t uNumParams, uint8_t* pParams) {
	if (uNumParams <= 1) {
		HAL_DSI_ShortWrite(&m_sDSIHandle, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
	} else {
		HAL_DSI_LongWrite(&m_sDSIHandle, 0, DSI_DCS_LONG_PKT_WRITE, uNumParams, pParams[uNumParams], pParams);
	}
}


  //------------------------
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
  //------------------------------
  //QAD_LTDC Layer Control Methods

//QAD_LTDC::imp_flipLayer0Buffers
//QAD_LTDC Layer Control Method
void QAD_LTDC::imp_flipLayer0Buffers(void) {
  if (!m_eInitState)
  	return;

  switch (m_eLayer0Front) {
    case (QAD_LTDC_Buffer0): {
    	m_eLayer0Back  = QAD_LTDC_Buffer0;
    	m_eLayer0Front = QAD_LTDC_Buffer1;
    	uint32_t uAddr = ((uint32_t)m_pLayer0Ptr + QAD_LTDC_BUFFERSIZE);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 0);
    	break;
    }
    case (QAD_LTDC_Buffer1): {
    	m_eLayer0Back  = QAD_LTDC_Buffer1;
    	m_eLayer0Front = QAD_LTDC_Buffer0;
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
    case (QAD_LTDC_Buffer0): {
    	m_eLayer1Back  = QAD_LTDC_Buffer0;
    	m_eLayer1Front = QAD_LTDC_Buffer1;
    	uint32_t uAddr = ((uint32_t)m_pLayer1Ptr + QAD_LTDC_BUFFERSIZE);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 1);
    	break;
    }
    case (QAD_LTDC_Buffer1): {
    	m_eLayer1Back  = QAD_LTDC_Buffer1;
    	m_eLayer1Front = QAD_LTDC_Buffer0;
    	uint32_t uAddr = ((uint32_t)m_pLayer1Ptr);
    	HAL_LTDC_SetAddress(&m_sLTDCHandle, uAddr, 1);
    	break;
    }
  }
}


  //---------------------------
  //---------------------------
  //QAD_LTDC Layer Data Methods

//QAD_LTDC::imp_getLayer0BackBufferPtr
//QAD_LTDC Layer Data Method
void* QAD_LTDC::imp_getLayer0BackBufferPtr(void) {
	if (!m_eInitState)
		return NULL;

	switch (m_eLayer0Back) {
	  case (QAD_LTDC_Buffer0):
	  	return m_pLayer0Ptr;
	  	break;
	  case (QAD_LTDC_Buffer1):
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
	  case (QAD_LTDC_Buffer0):
	  	return (QAD_LTDC_Buffer*)m_pLayer0Ptr;
	  	break;
	  case (QAD_LTDC_Buffer1):
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
	  case (QAD_LTDC_Buffer0):
	  	return m_pLayer1Ptr;
	  	break;
	  case (QAD_LTDC_Buffer1):
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
	  case (QAD_LTDC_Buffer0):
	  	return (QAD_LTDC_Buffer*)m_pLayer1Ptr;
	  	break;
	  case (QAD_LTDC_Buffer1):
	  	return (QAD_LTDC_Buffer*)((uint32_t)m_pLayer1Ptr + QAD_LTDC_BUFFERSIZE);
	  	break;
	}

	return NULL;
}


  //-------------------------------------------------
  //-------------------------------------------------
  //OTM8009A Display Controller Communication Methods

//DSI_IO_WriteCmd
//OTM8009A Display Controller Communication Methods
//Used by the OTM8009A initialization method in the otm8009a.h file
//Calls the static dsi_IO_WriteCmd method in the QAD_LTDC driver class
void DSI_IO_WriteCmd(uint32_t NbrParam, uint8_t* pParams) {
  QAD_LTDC::dsi_IO_WriteCmd(NbrParam, pParams);
}














/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F769I Discovery                                                 */
/*                                                                         */
/*   System: Driver                                                        */
/*   Role: FMC / SDRAM Driver                                              */
/*   Filename: QAD_FMC.cpp                                                 */
/*   Date: 30th September 2021                                             */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "QAD_FMC.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //------------------------------
  //------------------------------
  //QAD_FMC Initialization Methods

//QAD_FMC::imp_init
//QAD_FMC Initialization Method
//
//To be called from static method init()
//Used to initialize the required GPIOs, FMC driver and SDRAM module
//Returns QA_OK if initialization successful, or QA_Fail if initialization fails
QA_Result QAD_FMC::imp_init() {

	//Return if FMC driver is already initialized
	if (m_eState)
		return QA_OK;


	//----------------
	//Initialize GPIOs
	GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;            //Set pins as alternate function in push-pull mode
	GPIO_Init.Pull      = GPIO_PULLUP;                //Enable pull-up resistors on pins
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;  //Set pin speed to very high
	GPIO_Init.Alternate = GPIO_AF12_FMC;              //Set alternate function to suit use with FMC peripheral

	  //Initialize required pins from GPIO port D
	GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_Init);

		//Initialize required pins from GPIO port E
	GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_Init);

		//Initialize required pins from GPIO port F
	GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF, &GPIO_Init);

		//Initialize required pins from GPIO port G
	GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG, &GPIO_Init);

		//Initialize required pins from GPIO port H
	GPIO_Init.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOH, &GPIO_Init);

		//Initialize required pins from GPIO port I
	GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOI, &GPIO_Init);


	//-------------------------
	//Initialize FMC Peripheral

	//Initialize FMC Clock
	__HAL_RCC_FMC_CLK_ENABLE();

	//Fill FMC initialization details into Handle structure
	//NOTE: The settings need to meet the specifications of the specific SDRAM module being used
	//The below settings are specifically set to suit the module provided on the STM32F769I Discovery board
	m_sHandle.Instance                = FMC_SDRAM_DEVICE;                     //Set FMC SDRAM Device as required instance
	m_sHandle.Init.SDBank             = FMC_SDRAM_BANK1;                      //Set SDRAM as being connected to FMCs SDRAM bank 1
	m_sHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;          //Set size of column address to 8 bits
	m_sHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;            //Set size of row address to 12 bits
	m_sHandle.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;           //Set data bus width to 32 bits
	m_sHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;         //Set number of SDRAM module internal banks to 4
	m_sHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;              //Set column address strobe (CAS) latency to 3 cycles
	m_sHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;   //Disable write protection, as we want to be able to both read and write to SDRAM
	m_sHandle.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;             //Set SDRAM clock period to 2 clock cycles
	m_sHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;              //Enable read burst
	m_sHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;              //Set read pipe delay to 0 clock cycles

	//Fill SDRAM timing details into timing structure
	//NOTE: The timings need to meet the specifications of the specific SDRAM module being used
	//The below settings are specifically set to suit the module provided on the STM32F769I Discovery board
	FMC_SDRAM_TimingTypeDef SDRAM_Timing = {0};
	SDRAM_Timing.LoadToActiveDelay    = 2;  //Set delay between a Load Mode Register command and an Active or Refresh command to 2 clock cycles
	SDRAM_Timing.ExitSelfRefreshDelay = 7;  //Set delay between releasing Self Refresh command and issuing of Active command to 7 clock cycles
	SDRAM_Timing.SelfRefreshTime      = 4;  //Set minimum Self Refresh period to 4 clock cycles
	SDRAM_Timing.RowCycleDelay        = 7;  //Set delay between Refresh command and Activate command, or between consecutive Refresh commands to 7 clock cycles
	SDRAM_Timing.WriteRecoveryTime    = 2;  //Set Write Recovery Time to 2 clock cycles
	SDRAM_Timing.RPDelay              = 2;  //Set delay between a Precharge command and any other command to 2 clock cycles
	SDRAM_Timing.RCDDelay             = 2;  //Set delay between an Activate command and a Read/Write command to 2 clock cycles

	//Initialize the FMC peripheral using the required settings and timings to suit the specific SDRAM module
	if (HAL_SDRAM_Init(&m_sHandle, &SDRAM_Timing) != HAL_OK) {
		return QA_Fail;
	}

  //-------------------
	//Initialize SDRAM IC

	FMC_SDRAM_CommandTypeDef  SDRAM_Cmd = {0};

		//Send command to SDRAM module to enable its clock
	SDRAM_Cmd.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;   //Set to Clock Enable Command
	SDRAM_Cmd.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	SDRAM_Cmd.AutoRefreshNumber      = 1;
	SDRAM_Cmd.ModeRegisterDefinition = 0;
	cmd(SDRAM_Cmd);

		//Delay for 1ms to allow time for SDRAM clock to enable
	HAL_Delay(1);

		//Send command to SDRAM module to precharge all banks
	SDRAM_Cmd.CommandMode            = FMC_SDRAM_CMD_PALL;  //Set to Precharge All command
	SDRAM_Cmd.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	SDRAM_Cmd.AutoRefreshNumber      = 1;
	SDRAM_Cmd.ModeRegisterDefinition = 0;
	cmd(SDRAM_Cmd);

		//Send command to SDRAM module to Configure Auto Refresh
	SDRAM_Cmd.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE; //Set to Auto Refresh Mode command
	SDRAM_Cmd.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	SDRAM_Cmd.AutoRefreshNumber      = 8;                              //Set number of consecutive auto refresh commands to 8
	SDRAM_Cmd.ModeRegisterDefinition = 0;
	cmd(SDRAM_Cmd);

		//Program Mode Register
	SDRAM_Cmd.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;        //Set to Load Mode Register command
	SDRAM_Cmd.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	SDRAM_Cmd.AutoRefreshNumber      = 1;
	SDRAM_Cmd.ModeRegisterDefinition = m_uModeReg_BurstLength_1 |      //Set Read burst length to 1 32bit word
			                               m_uModeReg_BurstType_Seq |      //Set burst mode to sequential
																		 m_uModeReg_CASLatency_3 |       //Set column address strobe (CAS) latency to 3 clock cycles (to match setting provided to FMC)
																		 m_uModeReg_OpMode_Std |         //Set operation mode to standard
																		 m_uModeReg_WriteBurst_Single;   //Set write burst length to 1 32bit word
	cmd(SDRAM_Cmd);


	//---------------------------------
	//Program SDRAM module refresh rate
	HAL_SDRAM_ProgramRefreshRate(&m_sHandle, m_uRefresh);

	//Set Driver State
	m_eState = QA_Initialized; //Set driver as being initializeed

	//Return
	return QA_OK;
}


  //--------------------
  //QAD_FMC Test Methods

//QAD_FMC::imp_test
//QAD_FMC Test Method
//
//Used to perform read and write test of full 16 megabytes of SDRAM
//Returns QA_OK if test passes, or QA_Fail if test is not successful
QA_Result QAD_FMC::imp_test(void) {

	//Write values to whole 16 megabytes of SDRAM
	for (uint32_t i=0; i < QAD_FMC_32BITWORD_COUNT; i++) {
		QAD_FMC_Data->words[i] = i;
	}

	//Read values from while 16 megabytes of SDRAM, and confirm that they match the values that were written to it
	for (uint32_t i=0; i < QAD_FMC_32BITWORD_COUNT; i++) {

		//If data read doesn't match data written then return QA_Fail
		if (QAD_FMC_Data->words[i] != i)
			return QA_Fail;
	}

	//Return QA_OK as if we reach this point then the test has passed
	return QA_OK;
}


//QAD_FMC::cmd
//QAD_FMC Initialization Method
//
//Used to send commands to SDRAM module
//pCmd - FMC_SDRAM_CommandTypeDef containing details of command to be sent to SDRAM module
void QAD_FMC::cmd(FMC_SDRAM_CommandTypeDef& pCmd) {
	HAL_SDRAM_SendCommand(&m_sHandle, &pCmd, m_uTimeout);
}





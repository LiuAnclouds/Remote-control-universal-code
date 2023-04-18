/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


/////////////////// FEATURE SELECT /////////////////////////////////
#if (__PROJECT_8278_BLE_SAMPLE__)

#define BLE_APP_PM_ENABLE					1
#define PM_DEEPSLEEP_RETENTION_ENABLE		1
#define TEST_CONN_CURRENT_ENABLE            0 //test connection current, disable UI to have a pure power
#define BLE_REMOTE_SECURITY_ENABLE      	1

#elif(__PROJECT_8258_BLE_SAMPLE__)

#define BLE_APP_PM_ENABLE					1
#define PM_DEEPSLEEP_RETENTION_ENABLE		1
#define TEST_CONN_CURRENT_ENABLE            0 //test connection current, disable UI to have a pure power
#define BLE_REMOTE_SECURITY_ENABLE      	1

#endif





/////////////// Sample Test Board Select Configuration ///////////////////////////
#define EVK_C1T139A30_V1P2					1     //TLSR8258DK48
#define DONGLE_C1T139A3_V2P0A				2     //8258 dongle board
#define EVK_C1T197A30_V1P0					3	  //TLSR8278 EVK 48pin
#define DONGLE_C1T201A3_V1P0				4	  //8278 dongle board

#if (__PROJECT_8278_BLE_SAMPLE__)
#define SAMPLE_BOARD_SELECT					EVK_C1T197A30_V1P0   //default use TLSR8278DK48
#else
#define SAMPLE_BOARD_SELECT					EVK_C1T139A30_V1P2   //default use TLSR8258DK48
#endif


#if (SAMPLE_BOARD_SELECT == EVK_C1T139A30_V1P2)
	#if (TEST_CONN_CURRENT_ENABLE) //test current, disable keyboard
			#define	UI_KEYBOARD_ENABLE						0
	#else
			#define	UI_KEYBOARD_ENABLE						1
	#endif


	#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
			//---------------  KeyMatrix PB2/PB3/PB4/PB5 -----------------------------
			#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
			#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

			#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid



			#define			CR_VOL_UP				0xf0  ////
			#define			CR_VOL_DN				0xf1




			#define		KB_MAP_NORMAL	{	CR_VOL_UP,		VK_1,	 \
											CR_VOL_DN,		VK_2 }



			//////////////////// KEY CONFIG (EVK board) ///////////////////////////
			#define  KB_DRIVE_PINS  {GPIO_PB4, GPIO_PB5}
			#define  KB_SCAN_PINS   {GPIO_PB2, GPIO_PB3}

			//drive pin as gpio
			#define	PB4_FUNC				AS_GPIO
			#define	PB5_FUNC				AS_GPIO

			//drive pin need 100K pulldown
			#define	PULL_WAKEUP_SRC_PB4		MATRIX_ROW_PULL
			#define	PULL_WAKEUP_SRC_PB5		MATRIX_ROW_PULL

			//drive pin open input to read gpio wakeup level
			#define PB4_INPUT_ENABLE		1
			#define PB5_INPUT_ENABLE		1

			//scan pin as gpio
			#define	PB2_FUNC				AS_GPIO
			#define	PB3_FUNC				AS_GPIO

			//scan  pin need 10K pullup
			#define	PULL_WAKEUP_SRC_PB2		MATRIX_COL_PULL
			#define	PULL_WAKEUP_SRC_PB3		MATRIX_COL_PULL

			//scan pin open input to read gpio level
			#define PB2_INPUT_ENABLE		1
			#define PB3_INPUT_ENABLE		1



			#define		KB_MAP_NUM		KB_MAP_NORMAL
			#define		KB_MAP_FN		KB_MAP_NORMAL

	#endif

#elif (SAMPLE_BOARD_SELECT == DONGLE_C1T139A3_V2P0A)
	#undef  PM_DEEPSLEEP_RETENTION_ENABLE
	#define PM_DEEPSLEEP_RETENTION_ENABLE					0    //dongle demo no need deepSleepRetention

	#define	UI_BUTTON_ENABLE								1
	#define UI_LED_ENABLE									1

	#if (UI_BUTTON_ENABLE)
			//---------------  Button -------------------------------
			#define	SW1_GPIO				GPIO_PD5
			#define	SW2_GPIO				GPIO_PD6
			#define PD5_FUNC				AS_GPIO
			#define PD6_FUNC				AS_GPIO
			#define PD5_INPUT_ENABLE		1
			#define PD6_INPUT_ENABLE		1
			#define PULL_WAKEUP_SRC_PD5     PM_PIN_PULLUP_10K
			#define PULL_WAKEUP_SRC_PD6     PM_PIN_PULLUP_10K
	#endif



	#if (UI_LED_ENABLE)
			//---------------  LED ----------------------------------
			#define	GPIO_LED_RED			GPIO_PA3
			#define	GPIO_LED_WHITE			GPIO_PB1
			#define	GPIO_LED_GREEN			GPIO_PA2
			#define	GPIO_LED_BLUE			GPIO_PB0
			#define	GPIO_LED_YELLOW			GPIO_PA4

			#define PA3_FUNC				AS_GPIO
			#define PB1_FUNC				AS_GPIO
			#define PA2_FUNC				AS_GPIO
			#define PB0_FUNC				AS_GPIO
			#define PA4_FUNC				AS_GPIO

			#define	PA3_OUTPUT_ENABLE		1
			#define	PB1_OUTPUT_ENABLE		1
			#define PA2_OUTPUT_ENABLE		1
			#define	PB0_OUTPUT_ENABLE		1
			#define	PA4_OUTPUT_ENABLE		1

			#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
	#endif

#elif (SAMPLE_BOARD_SELECT == EVK_C1T197A30_V1P0)

	#if (TEST_CONN_CURRENT_ENABLE) //test current, disable keyboard
			#define	UI_KEYBOARD_ENABLE						0
	#else
			#define	UI_KEYBOARD_ENABLE						1
	#endif


	#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
			//---------------  KeyMatrix PB2/PB3/PB4/PB5 -----------------------------
			#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
			#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

			#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid



			#define			CR_VOL_UP				0xf0  ////
			#define			CR_VOL_DN				0xf1




			#define		KB_MAP_NORMAL	{	CR_VOL_UP,		VK_1,	 \
											CR_VOL_DN,		VK_2 }



			//////////////////// KEY CONFIG (EVK board) ///////////////////////////
			#define  KB_DRIVE_PINS  {GPIO_PB4, GPIO_PB5}
			#define  KB_SCAN_PINS   {GPIO_PB2, GPIO_PB3}

			//drive pin as gpio
			#define	PB4_FUNC				AS_GPIO
			#define	PB5_FUNC				AS_GPIO

			//drive pin need 100K pulldown
			#define	PULL_WAKEUP_SRC_PB4		MATRIX_ROW_PULL
			#define	PULL_WAKEUP_SRC_PB5		MATRIX_ROW_PULL

			//drive pin open input to read gpio wakeup level
			#define PB4_INPUT_ENABLE		1
			#define PB5_INPUT_ENABLE		1

			//scan pin as gpio
			#define	PB2_FUNC				AS_GPIO
			#define	PB3_FUNC				AS_GPIO

			//scan  pin need 10K pullup
			#define	PULL_WAKEUP_SRC_PB2		MATRIX_COL_PULL
			#define	PULL_WAKEUP_SRC_PB3		MATRIX_COL_PULL

			//scan pin open input to read gpio level
			#define PB2_INPUT_ENABLE		1
			#define PB3_INPUT_ENABLE		1



			#define		KB_MAP_NUM		KB_MAP_NORMAL
			#define		KB_MAP_FN		KB_MAP_NORMAL

	#endif

#elif (SAMPLE_BOARD_SELECT == DONGLE_C1T201A3_V1P0)
	#undef  PM_DEEPSLEEP_RETENTION_ENABLE
	#define PM_DEEPSLEEP_RETENTION_ENABLE					0    //dongle demo no need deepSleepRetention

	#define	UI_BUTTON_ENABLE								1
	#define UI_LED_ENABLE									1

	#if (UI_BUTTON_ENABLE)
			//---------------  Button -------------------------------
			#define	SW1_GPIO				GPIO_PD5
			#define	SW2_GPIO				GPIO_PD6
			#define PD5_FUNC				AS_GPIO
			#define PD6_FUNC				AS_GPIO
			#define PD5_INPUT_ENABLE		1
			#define PD6_INPUT_ENABLE		1
			#define PULL_WAKEUP_SRC_PD5     PM_PIN_PULLUP_10K
			#define PULL_WAKEUP_SRC_PD6     PM_PIN_PULLUP_10K
	#endif



	#if (UI_LED_ENABLE)
			//---------------  LED ----------------------------------
			#define	GPIO_LED_RED			GPIO_PA3
			#define	GPIO_LED_WHITE			GPIO_PB1
			#define	GPIO_LED_GREEN			GPIO_PA2
			#define	GPIO_LED_BLUE			GPIO_PB0
			#define	GPIO_LED_YELLOW			GPIO_PA4

			#define PA3_FUNC				AS_GPIO
			#define PB1_FUNC				AS_GPIO
			#define PA2_FUNC				AS_GPIO
			#define PB0_FUNC				AS_GPIO
			#define PA4_FUNC				AS_GPIO

			#define	PA3_OUTPUT_ENABLE		1
			#define	PB1_OUTPUT_ENABLE		1
			#define PA2_OUTPUT_ENABLE		1
			#define	PB0_OUTPUT_ENABLE		1
			#define	PA4_OUTPUT_ENABLE		1

			#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
	#endif

#endif



#ifndef	UI_KEYBOARD_ENABLE
#define UI_KEYBOARD_ENABLE									0
#endif

#ifndef	UI_BUTTON_ENABLE
#define UI_BUTTON_ENABLE									0
#endif

#ifndef	TEST_CONN_CURRENT_ENABLE
#define TEST_CONN_CURRENT_ENABLE							0
#endif



/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG                   DEEP_ANA_REG0 //u8,can save 8 bit info when deep
#define CONN_DEEP_FLG	                    BIT(0) //if 1: conn deep, 0: adv deep






/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	16000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};



/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms





///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
typedef enum
{
	ATT_H_START = 0,


	//// Gap ////
	/**********************************************************************************************/
	GenericAccess_PS_H, 					//UUID: 2800, 	VALUE: uuid 1800
	GenericAccess_DeviceName_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	GenericAccess_DeviceName_DP_H,			//UUID: 2A00,   VALUE: device name
	GenericAccess_Appearance_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	GenericAccess_Appearance_DP_H,			//UUID: 2A01,	VALUE: appearance
	CONN_PARAM_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read
	CONN_PARAM_DP_H,						//UUID: 2A04,   VALUE: connParameter


	//// gatt ////
	/**********************************************************************************************/
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC


	//// device information ////
	/**********************************************************************************************/
	DeviceInformation_PS_H,					//UUID: 2800, 	VALUE: uuid 180A
	DeviceInformation_pnpID_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_pnpID_DP_H,			//UUID: 2A50,	VALUE: PnPtrs


	//// HID ////
	/**********************************************************************************************/
	HID_PS_H, 								//UUID: 2800, 	VALUE: uuid 1812

	//include
	HID_INCLUDE_H,							//UUID: 2802, 	VALUE: include

	//protocol
	HID_PROTOCOL_MODE_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
	HID_PROTOCOL_MODE_DP_H,					//UUID: 2A4E,	VALUE: protocolMode

	//boot keyboard input report
	HID_BOOT_KB_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_BOOT_KB_REPORT_INPUT_DP_H,			//UUID: 2A22, 	VALUE: bootKeyInReport
	HID_BOOT_KB_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: bootKeyInReportCCC

	//boot keyboard output report
	HID_BOOT_KB_REPORT_OUTPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_BOOT_KB_REPORT_OUTPUT_DP_H,		    //UUID: 2A32, 	VALUE: bootKeyOutReport

	//consume report in
	HID_CONSUME_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_CONSUME_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: reportConsumerIn
	HID_CONSUME_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: reportConsumerInCCC
	HID_CONSUME_REPORT_INPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_CONSUMER, TYPE_INPUT

	//keyboard report in
	HID_NORMAL_KB_REPORT_INPUT_CD_H,		//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_NORMAL_KB_REPORT_INPUT_DP_H,		//UUID: 2A4D, 	VALUE: reportKeyIn
	HID_NORMAL_KB_REPORT_INPUT_CCB_H,		//UUID: 2902, 	VALUE: reportKeyInInCCC
	HID_NORMAL_KB_REPORT_INPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_KEYBOARD, TYPE_INPUT

	//keyboard report out
	HID_NORMAL_KB_REPORT_OUTPUT_CD_H,		//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_NORMAL_KB_REPORT_OUTPUT_DP_H,  		//UUID: 2A4D, 	VALUE: reportKeyOut
	HID_NORMAL_KB_REPORT_OUTPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_KEYBOARD, TYPE_OUTPUT

	// report map
	HID_REPORT_MAP_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read
	HID_REPORT_MAP_DP_H,					//UUID: 2A4B, 	VALUE: reportKeyIn
	HID_REPORT_MAP_EXT_REF_H,				//UUID: 2907 	VALUE: extService

	//hid information
	HID_INFORMATION_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read
	HID_INFORMATION_DP_H,					//UUID: 2A4A 	VALUE: hidInformation

	//control point
	HID_CONTROL_POINT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: write_without_rsp
	HID_CONTROL_POINT_DP_H,					//UUID: 2A4C 	VALUE: controlPoint


	//// battery service ////
	/**********************************************************************************************/
	BATT_PS_H, 								//UUID: 2800, 	VALUE: uuid 180f
	BATT_LEVEL_INPUT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	BATT_LEVEL_INPUT_DP_H,					//UUID: 2A19 	VALUE: batVal
	BATT_LEVEL_INPUT_CCB_H,					//UUID: 2902, 	VALUE: batValCCC


	//// Ota ////
	/**********************************************************************************************/
	OTA_PS_H, 								//UUID: 2800, 	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_OUT_DESC_H,						//UUID: 2901, 	VALUE: otaName


	ATT_END_H,

}ATT_HANDLE;


#if (__PROJECT_8258_BLE_SAMPLE__)

#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)


	#define GPIO_CHN0							GPIO_PD0
	#define GPIO_CHN1							GPIO_PD1
	#define GPIO_CHN2							GPIO_PD6
    #define GPIO_CHN3							GPIO_PD7
	#define GPIO_CHN4							GPIO_PA2
	#define GPIO_CHN5							GPIO_PA3
	#define GPIO_CHN6							GPIO_PA4


	#if 1   //analog pull up&down resistance could keep in deepSleep retention stage
		#define PULL_WAKEUP_SRC_PD0					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PD1					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PD6					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PD7					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA2					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA3					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA4					PM_PIN_PULLDOWN_100K


		#define DBG_CHN0_LOW		GPIO_PD0_PULL_DOWN_100K
		#define DBG_CHN0_HIGH		GPIO_PD0_PULL_UP_10K
		#define DBG_CHN0_TOGGLE		GPIO_PD0_PULL_TOGGLE
		#define DBG_CHN1_LOW		GPIO_PD1_PULL_DOWN_100K
		#define DBG_CHN1_HIGH		GPIO_PD1_PULL_UP_10K
		#define DBG_CHN1_TOGGLE		GPIO_PD1_PULL_TOGGLE
		#define DBG_CHN2_LOW		GPIO_PD6_PULL_DOWN_100K
		#define DBG_CHN2_HIGH		GPIO_PD6_PULL_UP_10K
		#define DBG_CHN2_TOGGLE		GPIO_PD6_PULL_TOGGLE
		#define DBG_CHN3_LOW		GPIO_PD7_PULL_DOWN_100K
		#define DBG_CHN3_HIGH		GPIO_PD7_PULL_UP_10K
		#define DBG_CHN3_TOGGLE		GPIO_PD7_PULL_TOGGLE
		#define DBG_CHN4_LOW		GPIO_PA2_PULL_DOWN_100K
		#define DBG_CHN4_HIGH		GPIO_PA2_PULL_UP_10K
		#define DBG_CHN4_TOGGLE		GPIO_PA2_PULL_TOGGLE
		#define DBG_CHN5_LOW		GPIO_PA3_PULL_DOWN_100K
		#define DBG_CHN5_HIGH		GPIO_PA3_PULL_UP_10K
		#define DBG_CHN5_TOGGLE		GPIO_PA3_PULL_TOGGLE
		#define DBG_CHN6_LOW		GPIO_PA4_PULL_DOWN_100K
		#define DBG_CHN6_HIGH		GPIO_PA4_PULL_UP_10K
		#define DBG_CHN6_TOGGLE		GPIO_PA4_PULL_TOGGLE
	#else
		#define PD0_OUTPUT_ENABLE					1
		#define PD1_OUTPUT_ENABLE					1
		#define PD6_OUTPUT_ENABLE					1
		#define PD7_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1


		#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
		#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
		#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
		#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
		#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
		#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
		#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
		#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
		#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
		#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
		#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
		#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
		#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
		#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
		#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
		#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
		#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
		#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
		#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
		#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
		#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
	#endif
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE



#if (TEST_CONN_CURRENT_ENABLE)  //for those none used GPIO, enable all 100K pullDown preventing potential current leakage to get a excellent power consumption

	#define	PULL_WAKEUP_SRC_PA0		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PA1		PM_PIN_PULLDOWN_100K
	//PA5 is DM of USB
	//PA6 is DP of USB
	//PA7 is SWS
	#if(!UI_KEYBOARD_ENABLE)  	//PB2~PB5 is for keyScan
		#define	PULL_WAKEUP_SRC_PB2		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB3		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB4		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB5		PM_PIN_PULLDOWN_100K
	#endif

	#define	PULL_WAKEUP_SRC_PB6		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PB7		PM_PIN_PULLDOWN_100K

	#define	PULL_WAKEUP_SRC_PC0		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC1		PM_PIN_PULLDOWN_100K
	// PC2/PC3 may used for external crystal input
	#define	PULL_WAKEUP_SRC_PC4		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC5		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC6		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC7		PM_PIN_PULLDOWN_100K

	#define	PULL_WAKEUP_SRC_PD0		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD1		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD2		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD3		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD4		PM_PIN_PULLDOWN_100K
	#if(!UI_BUTTON_ENABLE)  //PD5~PD6 is for button
		#define	PULL_WAKEUP_SRC_PD5		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PD6		PM_PIN_PULLDOWN_100K
	#endif

#endif

#else
#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)


	#define GPIO_CHN0							GPIO_PB0
	#define GPIO_CHN1							GPIO_PB1
	#define GPIO_CHN2							GPIO_PB6
    #define GPIO_CHN3							GPIO_PB7
	#define GPIO_CHN4							GPIO_PA2
	#define GPIO_CHN5							GPIO_PA3
	#define GPIO_CHN6							GPIO_PA4


	#if 0   //analog pull up&down resistance could keep in deepSleep retention stage
		#define PULL_WAKEUP_SRC_PB0					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PB1					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PB6					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PB7					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA2					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA3					PM_PIN_PULLDOWN_100K
		#define PULL_WAKEUP_SRC_PA4					PM_PIN_PULLDOWN_100K


		#define DBG_CHN0_LOW		GPIO_PB0_PULL_DOWN_100K
		#define DBG_CHN0_HIGH		GPIO_PB0_PULL_UP_10K
		#define DBG_CHN0_TOGGLE		GPIO_PB0_PULL_TOGGLE
		#define DBG_CHN1_LOW		GPIO_PB1_PULL_DOWN_100K
		#define DBG_CHN1_HIGH		GPIO_PB1_PULL_UP_10K
		#define DBG_CHN1_TOGGLE		GPIO_PB1_PULL_TOGGLE
		#define DBG_CHN2_LOW		GPIO_PB6_PULL_DOWN_100K
		#define DBG_CHN2_HIGH		GPIO_PB6_PULL_UP_10K
		#define DBG_CHN2_TOGGLE		GPIO_PB6_PULL_TOGGLE
		#define DBG_CHN3_LOW		GPIO_PB7_PULL_DOWN_100K
		#define DBG_CHN3_HIGH		GPIO_PB7_PULL_UP_10K
		#define DBG_CHN3_TOGGLE		GPIO_PB7_PULL_TOGGLE
		#define DBG_CHN4_LOW		GPIO_PA2_PULL_DOWN_100K
		#define DBG_CHN4_HIGH		GPIO_PA2_PULL_UP_10K
		#define DBG_CHN4_TOGGLE		GPIO_PA2_PULL_TOGGLE
		#define DBG_CHN5_LOW		GPIO_PA3_PULL_DOWN_100K
		#define DBG_CHN5_HIGH		GPIO_PA3_PULL_UP_10K
		#define DBG_CHN5_TOGGLE		GPIO_PA3_PULL_TOGGLE
		#define DBG_CHN6_LOW		GPIO_PA4_PULL_DOWN_100K
		#define DBG_CHN6_HIGH		GPIO_PA4_PULL_UP_10K
		#define DBG_CHN6_TOGGLE		GPIO_PA4_PULL_TOGGLE
	#else
		#define PB0_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
		#define PB6_OUTPUT_ENABLE					1
		#define PB7_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1

		#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
		#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
		#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
		#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
		#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
		#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
		#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
		#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
		#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
		#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
		#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
		#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
		#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
		#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
		#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
		#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
		#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
		#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
		#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
		#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
		#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
	#endif
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE



#if (TEST_CONN_CURRENT_ENABLE)  //for those none used GPIO, enable all 100K pullDown preventing potential current leakage to get a excellent power consumption

	#define	PULL_WAKEUP_SRC_PA1		PM_PIN_PULLDOWN_100K
	//PA5 is DM of USB
	//PA6 is DP of USB
	//PA7 is SWS
	#if(!UI_KEYBOARD_ENABLE)  	//PB2~PB5 is for keyScan
		#define	PULL_WAKEUP_SRC_PB2		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB3		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB4		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PB5		PM_PIN_PULLDOWN_100K
	#endif

	#define	PULL_WAKEUP_SRC_PB6		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PB7		PM_PIN_PULLDOWN_100K

	#define	PULL_WAKEUP_SRC_PC0		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC1		PM_PIN_PULLDOWN_100K
	// PC2/PC3 may used for external crystal input
	#define	PULL_WAKEUP_SRC_PC4		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC5		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC6		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PC7		PM_PIN_PULLDOWN_100K

	#define	PULL_WAKEUP_SRC_PD2		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD3		PM_PIN_PULLDOWN_100K
	#define	PULL_WAKEUP_SRC_PD4		PM_PIN_PULLDOWN_100K
	#if(!UI_BUTTON_ENABLE)  //PD5~PD6 is for button
		#define	PULL_WAKEUP_SRC_PD5		PM_PIN_PULLDOWN_100K
		#define	PULL_WAKEUP_SRC_PD6		PM_PIN_PULLDOWN_100K
	#endif

#endif

#endif



#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

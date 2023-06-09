/********************************************************************************************************
 * @file     main.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
  * @date     May. 10, 2018
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
#include "app.h"
#include <tl_common.h>
#include "drivers.h"
#include "app_config.h"
#include "stack/ble/ble.h"
#include "vendor/common/blt_common.h"
#include "vendor/common/user_config.h"


extern my_fifo_t hci_rx_fifo;

extern void user_init_normal();
extern void user_init_deepRetn();

extern void main_loop (void);




_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler();

#if(FEATURE_TEST_MODE == TEST_BLE_PHY)
	app_phytest_irq_proc();
#endif
}

_attribute_ram_code_ int main(void)
{
	DBG_CHN0_LOW;   //debug

	blc_pm_select_internal_32k_crystal();

	#if(MCU_CORE_TYPE == MCU_CORE_8258)
		cpu_wakeup_init();
	#elif(MCU_CORE_TYPE == MCU_CORE_8278)
		cpu_wakeup_init(LDO_MODE,EXTERNAL_XTAL_24M);
	#endif

	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(!deepRetWakeUp);

	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
		clock_init(SYS_CLK_16M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		clock_init(SYS_CLK_24M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
		clock_init(SYS_CLK_32M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
		clock_init(SYS_CLK_48M_Crystal);
	#endif


	if(!deepRetWakeUp){//read flash size
		blc_readFlashSize_autoConfigCustomFlashSector();
	}

	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value

	if( deepRetWakeUp ){
		user_init_deepRetn ();
	}
	else{
		user_init_normal ();
	}

	irq_enable();


	while(1)
	{
	#if(MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
	#endif
		main_loop ();
	}
}



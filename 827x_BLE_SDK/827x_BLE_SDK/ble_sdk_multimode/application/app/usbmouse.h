/*
 * @Author: your name
 * @Date: 2023-04-28 18:08:39
 * @LastEditTime: 2023-04-28 18:08:55
 * @LastEditors: 暗夜精灵8
 * @Description: In User Settings Edit
 * @FilePath: /ble_sdk_multimode/application/app/usbmouse.h
 */
/********************************************************************************************************
 * @file     usbmouse.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 30, 2010
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


#include <application/usbstd/HIDClassCommon.h>
#include <application/usbstd/HIDReportData.h>
#include "tl_common.h"
//#include "../common/types.h"
//#include "../common/static_assert.h"
//#include "../common/bit.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	extern "C" {
#endif

//鼠标报告数据长度
#define MOUSE_REPORT_DATA_LEN     (sizeof(mouse_data_t))
//媒体报告数据长度
#define MEDIA_REPORT_DATA_LEN		4
int usbmouse_hid_report(u8 report_id, u8 *data, int cnt);


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
	}
#endif

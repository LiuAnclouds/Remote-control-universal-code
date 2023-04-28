/*
 * @Author: your name
 * @Date: 2023-04-27 10:41:39
 * @LastEditTime: 2023-04-27 10:48:17
 * @LastEditors: 暗夜精灵8
 * @Description: In User Settings Edit
 * @FilePath: /ble_sdk_multimode/application/app/usbkb.h
 */
/********************************************************************************************************
 * @file     usbkb.h 
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
#include "drivers.h"
//#include "../common/types.h"
//#include "../common/static_assert.h"
//#include "../mcu/clock_i.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

//用于定义数据类型，分别表示键盘和鼠标
#define DAT_TYPE_KB			1
#define DAT_TYPE_MOUSE		2

//用于定义USB设备发送数据时的缓冲区大小，包括缓冲区数量和每个缓冲区的大小
#define USB_FIFO_NUM		4
#define USB_FIFO_SIZE		8
//USB发送数据时的缓冲区
extern u8 usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];
//缓冲区的读指针
extern u8 usb_ff_rptr;
//缓冲区的写指针
extern u8 usb_ff_wptr;


//用于定义键盘报告中最多能发送的键值数量
#define KEYBOARD_REPORT_KEY_MAX     6
/// @brief 
///该结构体用于描述键盘HID报告的数据格式
typedef struct {
    //表示键盘修饰键的状态，是一个字节类型的数据
    u8 Modifier; /**< Keyboard modifier byte, indicating pressed modifier keys (a combination of
                       *   \c HID_KEYBOARD_MODIFER_* masks).
                      */
    //保留位，用于OEM自定义，值为0
    u8 Reserved; /**< Reserved for OEM use, always set to 0. */
    //表示当前按下的键值，是一个长度为KEYBOARD_REPORT_KEY_MAX的数组
    //使用键盘扫描码来表示按下的键值
    u8 KeyCode[KEYBOARD_REPORT_KEY_MAX]; /**< Key codes of the currently pressed keys. */
} usbkb_hid_report_t;

int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode);

int usb_hid_report_fifo_proc(void);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif


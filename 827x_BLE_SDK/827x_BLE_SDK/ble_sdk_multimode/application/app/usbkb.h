/*
 * @Author: your name
 * @Date: 2023-04-27 10:41:39
 * @LastEditTime: 2023-04-27 10:48:17
 * @LastEditors: ��ҹ����8
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

//���ڶ����������ͣ��ֱ��ʾ���̺����
#define DAT_TYPE_KB			1
#define DAT_TYPE_MOUSE		2

//���ڶ���USB�豸��������ʱ�Ļ�������С������������������ÿ���������Ĵ�С
#define USB_FIFO_NUM		4
#define USB_FIFO_SIZE		8
//USB��������ʱ�Ļ�����
extern u8 usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];
//�������Ķ�ָ��
extern u8 usb_ff_rptr;
//��������дָ��
extern u8 usb_ff_wptr;


//���ڶ�����̱���������ܷ��͵ļ�ֵ����
#define KEYBOARD_REPORT_KEY_MAX     6
/// @brief 
///�ýṹ��������������HID��������ݸ�ʽ
typedef struct {
    //��ʾ�������μ���״̬����һ���ֽ����͵�����
    u8 Modifier; /**< Keyboard modifier byte, indicating pressed modifier keys (a combination of
                       *   \c HID_KEYBOARD_MODIFER_* masks).
                      */
    //����λ������OEM�Զ��壬ֵΪ0
    u8 Reserved; /**< Reserved for OEM use, always set to 0. */
    //��ʾ��ǰ���µļ�ֵ����һ������ΪKEYBOARD_REPORT_KEY_MAX������
    //ʹ�ü���ɨ��������ʾ���µļ�ֵ
    u8 KeyCode[KEYBOARD_REPORT_KEY_MAX]; /**< Key codes of the currently pressed keys. */
} usbkb_hid_report_t;

int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode);

int usb_hid_report_fifo_proc(void);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif


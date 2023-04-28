/*
 * @Author: your name
 * @Date: 2023-04-25 22:53:33
 * @LastEditTime: 2023-04-27 10:05:36
 * @LastEditors: 暗夜精灵8
 * @Description: In User Settings Edit
 * @FilePath: /ble_sdk_multimode/application/app/usbcdc.h
 */
/********************************************************************************************************
 * @file     usbcdc.h 
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

#include <application/usbstd/CDCClassCommon.h>
#include <application/usbstd/CDCClassDevice.h>
#include "../common/types.h"
#include "../common/static_assert.h"
#include "../common/bit.h"

/* Enable C linkage for C++ Compilers: */
//如果未C++环境，C扩展
#if defined(__cplusplus)
    extern "C" {
#endif

//函数结构体指针
//用于定义CDC设备发送数据时的回调函数，
//该函数用于在数据发送完成后进行一些处理工作
typedef void ( *cdc_handlerFn_t)( u8* pData);

/// @brief 
///结构体类型，用于定义CDC设备发送数据时的缓冲区
typedef struct {
    u8 len;//需要发送数据的长度
    u8 data[1];//需要发送缓冲区的数据
} usbcdc_txBuf_t;


/// @brief 
///枚举类型。表示CDC设备发送数据时的状态
typedef enum usbcdc_sts_e {
    // success = 0
    //表示缓冲区忙碌
    USB_BUSY = 1,
    //表示多块数据发送
    USB_MULTIBLOCK,
} usbcdc_sts_t;


void CDC_Device_ProcessControlRequest(u8 bRequest, u16 wValue, u16 wIndex, u16 wLength);

usbcdc_sts_t usbcdc_sendData(u8* buf, u8 len);
u8   usbcdc_sendBulkData(void);

u8   usbcdc_isAvailable(void);
u8*  usbcdc_getData(void);
void usbcdc_init(void);
void usbcdc_setCB(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb);
void usbcdc_setRxBuf(u8 *buf);




/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
    }
#endif

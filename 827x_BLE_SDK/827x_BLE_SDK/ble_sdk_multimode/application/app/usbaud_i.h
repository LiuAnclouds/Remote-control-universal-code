/********************************************************************************************************
 * @file     usbaud_i.h 
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


#include "drivers.h"
#include "usbaud.h"
#include "../usbstd/AudioClassCommon.h"
#include "../usbstd/usbhw.h"
#include "../usbstd/usbhw_i.h"
#include "../../common/config/user_config.h"
//这些头文件的作用是定义了一些常量、结构体和函数，
//用于 USB 音频设备的驱动程序开发。
//其中包括了一些通用的 USB 音频类定义、USB 音频设备信息结构体、
//USB 音频设备操作函数等。
//这些定义和函数可以帮助开发者更方便地实现 USB 音频设备的功能。




//以下代码使用了条件编译，#if 0表示将代码块中的内容都注释掉，
//相当于暂时禁用这部分代码
//该代码块中，定义了一个USB HID报告描述，用于描述USB音频涉笔的HID报告格式。
//该HID报告描述符包含了多个HID报告项，每个报告项对应一个HID操作。
//如播放、暂停、下一曲、上一曲等
#if 0
static const USB_Descriptor_HIDReport_Datatype_t usbaud_report_desc[] = {
	//指定使用的HID页面为消费者页面。
    HID_RI_USAGE_PAGE(8, 0x0c), /* Consumer Page */
    //指定使用的HID操作类型为消费者控制
	HID_RI_USAGE(8, 0x01)		, /* Consumer Controls */
    //定义HID报告项所述的集合类型为应用程序
    HID_RI_COLLECTION(8, 0x01)		, /* Application */
    //定义HID报告项的ID号为1
    HID_RI_REPORT_ID(8, 0x01)		, /*Report ID*/
    //定义HID报告项的逻辑最小值为0
    HID_RI_LOGICAL_MINIMUM(8, 0x00)		,
    //定义HID报告项的逻辑最大值为1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),
    /***以下为消费者控制操作类型的HID报告定义符**/
    //指定操作类为播放/暂停（切换）
    HID_RI_USAGE(8, 0xcd),  /* Play/Pause (toggle) */
    //指定操作类型为下一曲
	HID_RI_USAGE(8, 0xb5)		, /* Next Track */
    //指定操作类型为上一曲
	HID_RI_USAGE(8, 0xb6)		, /* Previous Track */
    //指定操作类型为停止
	HID_RI_USAGE(8, 0xb7)		, /* Stop */
    /***以下HID报告定义符用于描述消费者控制操作类型的输入输出*****/
    
    //定义每个数据位数为1
    HID_RI_REPORT_SIZE(8, 0x01)		,
    //定义一次传输4个数据
    HID_RI_REPORT_COUNT(8, 0x04),
    //指定输入属性为变量，即数据的值随时间变化
    HID_RI_INPUT(8, HID_IOF_VARIABLE),
    //定义数据的逻辑最小值为0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //定义数据的逻辑最大值为1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),

    //指定操作类为播放
    HID_RI_USAGE(8, 0xb0),  /* Play */
    //指定操作类为暂停
    HID_RI_USAGE(8, 0xb1)		, /* Pause */
    //指定操作类型为快进
    HID_RI_USAGE(8, 0xb3)		, /* Fast Forward */
    //指定操作类型为倒退
    HID_RI_USAGE(8, 0xb4)		, /* Rewind */

    //定义每个数据位数为1
    HID_RI_REPORT_SIZE(8, 0x01)		,
    //定义一次传输4个数据
    HID_RI_REPORT_COUNT(8, 0x04),
    //指定输入属性为变量，即数据的值随时间变化，且没有首选选项
    HID_RI_INPUT(8, HID_IOF_NO_PREFERRED_STATE | HID_IOF_VARIABLE),
    //定义数据的逻辑最小值为0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //定义数据的逻辑最大值为1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),

    //指定操作类型为向左滚动
    HID_RI_USAGE(8, 0xe9),
    //指定操作类型为向右滚动
    HID_RI_USAGE(8, 0xea),
    //指定操作类型为上下滚动
    HID_RI_USAGE(8, 0xe2),
    //定义每个数据位数为1
    HID_RI_REPORT_SIZE(8, 0x01),
    //定义一次传输3个数据
    HID_RI_REPORT_COUNT(8, 0x03),
    //指定输入属性为变量，即数据的值随时间变化，且没有首选选项
    HID_RI_INPUT(8, HID_IOF_NO_PREFERRED_STATE | HID_IOF_VARIABLE),

    //定义每个数据位数为5
    HID_RI_REPORT_SIZE(8, 0x05),
    //定义一次传输1个数据
    HID_RI_REPORT_COUNT(8, 0x01),
    //指定输入属性为变量，即数据的值不变
    HID_RI_INPUT(8, HID_IOF_CONSTANT),
    /**以上HID报告定义符定义了3中滚动操作类型和一个变量，用于控制条滚动***/
    HID_RI_END_COLLECTION(0),

    //指定使用的HID页面为厂商自定义页面
	HID_RI_USAGE_PAGE(16, 0xffa0),
    //指定使用的HID操作类型为第一个自定义类型
    HID_RI_USAGE(8, 0x01),
    //定义HID报告项所述的集合类型为应用程序
    HID_RI_COLLECTION(8, 0x01),  /* Application */
    //定义HID报告项的ID号为2
    HID_RI_REPORT_ID(8, 0x02)		, /*Report ID*/
    //指定操作类型为鼠标左键
    HID_RI_USAGE(8, 0x01)		,
    //定义数据的逻辑最小值为0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //定义逻辑最大值为255
	HID_RI_LOGICAL_MAXIMUM(16, 0x00ff),
    //定义每个数据位数为8
    HID_RI_REPORT_SIZE(8, 0x08),
    //定义一次传输两个数据
    HID_RI_REPORT_COUNT(8, 0x02),
    //指定输出属性为数据，即数据将被传输到主机
    HID_RI_OUTPUT(8, HID_IOF_DATA),
    //指定操作类型为鼠标右键
    HID_RI_USAGE(8, 0x02),  /* mouse? */
    
    //定义每个数据位为8
    HID_RI_REPORT_SIZE(8, 0x08)		,
    //定义一次传输2个数据
    HID_RI_REPORT_COUNT(8, 0x02),
    //指定输入属性为数据，即数据将从主机传输到设备
    HID_RI_INPUT(8, HID_IOF_DATA),
    HID_RI_END_COLLECTION(0),  };
//使用HID报告描述符可以让USB主机识别USB音频设备支持的HID操作类型
//从而可以通过USB HID协议进行控制。
//在这段代码中，使用了HID_RI_开头的宏定义来快速定义HID报告项，
//比手动编写HID报告描述更加方便和易读。
#endif


//
//USB HID报告描述符的定义
static const USB_Descriptor_HIDReport_Datatype_t usbaud_report_desc[] = {
		0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01,
		0x85, 0x01, 0x15, 0x00, 0x25, 0x01, 0x09, 0xcd, 0x09, 0xb5, 0x09, 0xb6,
		0x09, 0xb7, 0x75, 0x01, 0x95, 0x04, 0x81, 0x02, 0x15, 0x00, 0x25, 0x01,
		0x09, 0xb0, 0x09, 0xb1, 0x09, 0xb3, 0x09, 0xb4, 0x75, 0x01, 0x95, 0x04,
		0x81, 0x22, 0x15, 0x00, 0x25, 0x01, 0x09, 0xe9, 0x09, 0xea, 0x09, 0xe2,
		0x75, 0x01, 0x95, 0x03, 0x81, 0x22, 0x75, 0x05, 0x95, 0x01, 0x81, 0x01,
		0xc0, 0x06, 0xa0, 0xff, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x02, 0x09, 0x01,
		0x15, 0x00, 0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x02, 0x91, 0x00, 0x09,
		0x02, 0x75, 0x08, 0x95, 0x02, 0x81, 0x00, 0xc0, };

static inline u8* usbaud_get_report_desc(void) {
	return (u8*) (usbaud_report_desc);
}

static inline u16 usbaud_get_report_desc_size(void) {
	return sizeof(usbaud_report_desc);
}



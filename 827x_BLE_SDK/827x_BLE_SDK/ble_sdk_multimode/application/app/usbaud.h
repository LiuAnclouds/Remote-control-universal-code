/********************************************************************************************************
 * @file     usbaud.h 
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
//#include "../common/assert.h"

/* Enable C linkage for C++ Compilers: */

//这段代码表示在C++环境中使用C语言
//如果当前环境是C++
//则执行extern c
#if defined(__cplusplus)
extern "C" {
#endif
//函数名会根据参数类型和个数进行重载

// telink usb report ctrl command. used mixed with USB_REPORT_NO_EVENT
enum {
	USB_AUD_PLAY_PAUSE		= 0,//开始或暂停
	USB_AUD_NEXT_TRACK		= 1,//下一首
	USB_AUD_PREV_TRACK		= 2,//上一首
	USB_AUD_STOP			= 3,//停止
	USB_AUD_PLAY			= 4,//开始
	USB_AUD_PAUSE			= 5,//暂停
	USB_AUD_FAST_FWD		= 6,//快进
	USB_AUD_REWIND			= 7,//后退
	USB_AUD_VOL_INC			= 0x10,//调高音量
	USB_AUD_VOL_DEC			= 0x11,//降低音量
	USB_AUD_VOL_MUTE 		= 0x12,//停止
};

//麦克风和扬声器
#define AUDIO_FEATURE_ID_SPEAKER	0x02
#define AUDIO_FEATURE_ID_MIC		0x05

typedef struct{
	s16 speaker_vol;//扬声器音量
	s16 mic_vol;//麦克风音量
	s8 speaker_mute;
	s8 mic_mute;
	s8 change;//状态改变
}usb_audio_status_t;


typedef struct {
	u16 vol_cur;
	u16	vol_step;
	u8 	mute;
}speaker_setting_t;


typedef struct {
	u16 vol_cur;//表示音量大小
	u16	vol_step;//步进值
	u8 	mute;
}mic_setting_t;

#define AUDIO_VOLUME_STEP_MAX  11

#define MIC_VOL_MIN			((s16) 0x0000)     /* Volume Minimum Value */
#define MIC_VOL_MAX			((s16) 0x1e00)     /* Volume Maximum Value */
#define	MIC_VOL_RES			0x0180     /* Volume Resolution */
#define MIC_VOL_DEF			0x1800     /* Volume default */
#define	MIC_MAX_STEP		(MIC_VOL_MAX / MIC_VOL_RES)

#define	SPEAKER_VOL_MIN		((s16) 0xa000)     /* Volume Minimum Value */
#define	SPEAKER_VOL_MAX		((s16) 0x0300)     /* Volume Maximum Value */
#define SPEAKER_VOL_RES		0x0180     /* Volume Resolution */
#define SPEAKER_VOL_DEF		0x8000     /* Volume default */
#define SPEAKER_VOL_STEP	400

int usbaud_handle_set_speaker_cmd(int type);//发送扬声器命令
int usbaud_handle_set_mic_cmd(int type);//发送麦克风命令
int usbaud_handle_get_speaker_cmd(int req, int type);//获取扬声器命令
int usbaud_handle_get_mic_cmd(int req, int type);//获取麦克风命令
void usbaud_init(void);//饮品初始化
u8 usbaud_speaker_vol_get(void);//获取扬声器音量大小
u8 usbaud_mic_vol_get(void);//获取麦克风音量大小
void usbaud_mic_en(int en);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif


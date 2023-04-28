/********************************************************************************************************
 * @file     usbaud.c,该程序主要针对音频设备
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
#include "../../common/config/user_config.h"

#include "drivers.h"

#include "usbaud.h"
#include "../usbstd/usbhw.h"
#include "../usbstd/usbhw_i.h"
#include "../usbstd/usb.h"
#include "../usbstd/audioClassCommon.h"

/*************************************************
 * g_audio_hid_chg:
 * 		0x00: no event
 * 		0x01: speaker volume change
 * 		0x02: speaker mute change
 * 		0x11: microphone volume change
 * 		0x12: microphone mute change
 *************************************************/
// 以下代码是USB音频设备的控制函数，用于设置音频模式和发送音频HID报告
static speaker_setting_t speaker_setting;
/// @brief 
static mic_setting_t mic_setting;
// 以上是两个结构体，用存储扬声器和麦克风的设置



/// @brief 
/// @param iso_en 
/// @param mono_en 
/// 该函数用于设置音频模式
/// iso_en表示是否开启等时传输
/// mono_en表示是否开启单声道模式
void usbaud_set_audio_mode(int iso_en, int mono_en)
{
	// 先使用assert宏进行参数有效性检查
	assert(USB_EDP_MIC < 8);
	// 再使用SET_FLD()宏设置USB端点控制寄存器的字段值
	// 使用麦克风时对USB断点进行配置的代码
	SET_FLD(reg_usb_ep_ctrl(USB_EDP_MIC), FLD_USB_EP_EOF_ISO | FLD_USB_EP_MONO);
	// reg_usb_ep_ctrl：表示获取USB麦克风的断点寄存器地址
	//(USB_EDP_MIC)是一个宏定义用于表示USB麦克风的断点

	// FLD_USB_EP_EOF_ISO：表示在端点控制寄存器中设置麦克风端点的传输方式，
	// 为等时传输，并且在每个传输的末尾自动添加一个空闲帧，
	// 此处FLD_USB_EP_EOF_ISO是等时传输模式的标志位

	// FLD_USB_EP_MONO：表示设置USB麦克风的音频通道为单声道，
	// 此处FLD_USB_EP_MONO是单声道的标志位

	// SET_FLD()：是一个宏定义，用于设置指定寄存器中的置定位。

	// 综上所述，该代码的作用是设置USB麦克风的端点控制寄存器，
	// 使其以等时传输的方式传输音频数据，并设置音频通道为单声道。
}


#if 0
/// @brief 
/// @param format 
/// @param volume 
/// 以下内容作用是发送一个音频HID报告
void usbaud_hid_report(char format, char volume) {
	//format表示音频控制命令类型（mp3等)
	//volume表示音量控制命令类型

	//unsigned char sAudioHidReport用于存储要发送的音频HID报告
	unsigned char sAudioHidReport[] = { 0x01,/* report ID */
	format,/*[0]:play/pause,//启动或者暂停
	 [1]:scan next track,//下一首
	 [2]:scan previous track,//上一首
	 [3]:stop,//停止播放
	 [4]:play,//开始播放
	 [5]:pause,//暂停
	 [6]:fast forward,//快进
	 [7]:rewind,	//快退
	 */
	volume /*[0]:volume up//音量提升
	 [1]:volume down//音量下降
	 [2]:mute//静音命令，控制音频设备静音
	 [3:7] 0;//保留命令，为了使视频HID报告长度固定，这里将剩余的5个位设置为0
	 */
	};
	//第一个元素是报告HID，固定位0x01
	//第二个元素是format参数，用于表示音频控制命令类型，具体有8种类型
	//第三个元素是volume参数，用于表示音量控制命令类型，具体有4种类型

	WriteEndPoint(EDP_ID_AUDIO_HID, sAudioHidReport, sizeof(sAudioHidReport));
	//用于向指定的端点发送数据

	//1.EDP_ID_AUDIO_HID是一个常量或宏定义,表示要发送数据的端点ID,也就是目的地的端点。
	//2.sAudioHidReport是一个指向要发送数据的缓冲区指针，也就是要发送的音频HID报告数据
	//3.sizeof(sAudioHidReport)表示要发送的数据的大小，即音频HID报告数据的长度
	//4.WriteEndPoint函数将缓冲区中的数据发送到指定的端点，
	//以便USB主机或设备在进行音频控制时能够收到此数据
}
#endif

// if表示这段代码只有在USB扬声器或USB麦克风功能开启时才会被编译执行
#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE) //  use for volumn control, mute, next, prev track,  move to mouse hid

/// @brief 
/// @param cmd 
/// @param vol 
/// @return 
/// usbaud_hid_report(u8 cmd, u8 vol)函数用于
/// 向音频HID报告的端点发送音频控制命令
/// cmd表示音频控制命令类型，例如播放/暂停、下一曲、上一曲等
/// vol表示音频控制命令的参数，例如音量大小、是否静音等
/// 用于判断音频HID报告的端点是否繁忙，如果繁忙则返回0表示发送失败
int usbaud_hid_report(u8 cmd, u8 vol)
{ 
	if (usbhw_is_ep_busy(USB_EDP_AUDIO_IN))
		return 0;
	// 用于将音频HID报告的端点指针置零，以便从端点的起始位置发送数据
	reg_usb_ep_ptr(USB_EDP_AUDIO_IN) = 0;

	// please refer to keyboard_report_desc
	// 用于向音频HID报告的端点发送音频控制命令类型
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = USB_HID_AUDIO;
	// 用于向音频HID报告的端点发送音频控制命令参数
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = cmd;
	// 用于向音频HID报告的端点发送音频控制命令参数
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = vol;
	// 用于发送完数据后向端点发送数据确认信号，表示数据发送完毕
	reg_usb_ep_ctrl(USB_EDP_AUDIO_IN) = FLD_EP_DAT_ACK; // ACK
	return 1;											// 表示发送成功返回1
}
#endif


#if 0
/// @brief 
/// @param c 
/// @return 
/// if 0该段代码不会被编译器执行
u8 usbaud_handle_report(u8 c) 
{	//该函数用于处理音频HID报告
//c表示音频HID报告的数据，也就是音频控制命令类型和参数
	if (USB_REPORT_NO_EVENT == c) {
		return USB_REPORT_NO_EVENT;
		//表示没有事件要处理
	}
	//用于断言音频HID报告的端点ID小于8
	//如果断言失败，则会触发断言错误
    assert(USB_EDP_AUDIO < 8);

	//用于断言音频HID报告的端点是否繁忙
	//如果繁忙，则返回音频HID报告数据，表示处理失败
	if(reg_usb_ep_ctrl(USB_EDP_AUDIO) & FLD_USB_EP_BUSY)
		return c;

	//用于判断音频HID报告数据是否为释放状态，
	//如果是，发送音频控制命令类型为0
	//参数为0的音频控制命令道音频HID报告的端点
	//并返回USB_REPORT_NO_EVENT
	if(USB_REPORT_RELEASE == c){
		usbaud_hid_report(0, 0);
		return USB_REPORT_NO_EVENT;
	}else
	{	
		//如果不是释放状态
		//就将音频控制命令类型和参数分别发送到HID报告的端点，
		//并返回USB_REPORT_RELEASE
		usbaud_hid_report((c < 0x10) ? (1 << c) : 0
			,(c < 0x10) ? 0 : (1 << (c & 0x0f)));
		return USB_REPORT_RELEASE;
	}
}
#endif

/// @brief 
/// @param vol 表示音量值
/// @return 
///该函数用于计算扬声器音量对应的音量步进值，
///即根据传入的音量值计算出其对应的音量步进值
static u16 usbaud_cal_speaker_step(s16 vol)
{	
	//SPEAKER_VOL_MIN表示扬声器音量的最小值

	//if判断传入的音量值是否小于最小值，如果是，则返回0
	if (vol < SPEAKER_VOL_MIN)
		return 0;
	//SPEAKER_VOL_RES表示每一步音量的增量

	//计算传入的音量值对应的音量步进值
	return (vol - SPEAKER_VOL_MIN) / SPEAKER_VOL_RES;
}

/// @brief 
/// @param vol 表示音量值
/// @return 
///函数用于计算麦克风音量对应的音量步进值，
///同样是根据传入的音量值计算出其对应的音量步进值
static u16 usbaud_cal_mic_step(s16 vol)
{	
	//判断传入的音量值是否小于最小值
	if (vol < MIC_VOL_MIN)
		return 0;
	//计算传入的音量值对应的音量步进值
	return (vol - MIC_VOL_MIN) / MIC_VOL_RES;
}

/// @brief 
/// @param vol 
///函数用于设置扬声器音量，即将传入的音量值设置为当前音量
///并计算出其对应的音量步进值
void usbaud_set_speaker_vol(s16 vol)
{	//current
	speaker_setting.vol_cur = vol;
	//speaker_setting.vol_cur表示当前扬声器音量值
	speaker_setting.vol_step = usbaud_cal_speaker_step(vol);
	//speaker_setting.vol_step表示当前扬声器音量对应的音量步进值

}

/// @brief 
/// @param vol 
///该函数用于设置麦克风音量，同样是将传入的音量值设置为当前音量
///并计算出其对应的音量步进值
void usbaud_set_mic_vol(s16 vol)
{
	mic_setting.vol_cur = vol;
	//表示当前扬声器音量值
	mic_setting.vol_step = usbaud_cal_mic_step(vol);
	//表示当前扬声器音量对应的音量步进值
}

/// @brief 
///定义了一个全局变量g_usb_audio_status，
///用于记录当前USB音频的状态
///例如是否连接、是否开启扬声器或者麦克风
usb_audio_status_t g_usb_audio_status;


/// @brief 
/// @param  
/// @return 
///该函数用于获取当前扬声器音量的音量步进值和静音状态
u8 usbaud_speaker_vol_get(void)
{
	// return ((g_usb_audio_status.speaker_mute << 7) | g_usb_audio_status.speaker_vol);
	return (speaker_setting.mute << 7) | (speaker_setting.vol_step & 0x7f);
	//speaker_setting.mute表示当前扬声器是否处于静音状态
	//speaker_setting.vol_stepb表示当前扬声器音量步进值
}

/// @brief 
/// @param  
/// @return 
u8 usbaud_mic_vol_get(void)
{
	// return ((g_usb_audio_status.speaker_mute << 7) | g_usb_audio_status.speaker_vol);
	return (mic_setting.mute << 7) | (mic_setting.vol_step & 0x7f);
}

/// @brief 
/// @param en 
void usbaud_mic_en(int en)
{
	mic_setting.mute = en;
	//en是需要设置的麦克风静音状态,1表示静音，0表示取消静音
}

/// @brief 
///用与标记音频接口是否设置完成
volatile int aaa_audio_intf_set = 0;



/// @brief 
/// @param type 
/// @param feature_id 
/// @return 
///该函数用于处理音频输出接口的控制请求
///type表示请求的控制类型
///feature_id表示请求的音频特性ID
int usb_audio_class_out_intf_proc(u8 type, u8 feature_id)
{	
	//ret表示是函数执行的结构，0表示成功，1表示失败
	int ret = 0;
	//p_aud是指向全局变量g_usb_audio_status的指针，表示当前USB音频设备的状态
	usb_audio_status_t *p_aud = &g_usb_audio_status;

	//aaa_audio_intf_set表示音频接口是否设置完成的标志，初始化为0
	aaa_audio_intf_set = 0;
	//(type << 8)|feature_id
	//将控制类型和音频特性ID和并成一个16位无符号整数
	aaa_audio_intf_set = (type << 8);
	aaa_audio_intf_set |= feature_id;
	switch (type)
	{
	// mute control and sample
	//表示处理静音控制和采样控制请求
	case 0x01:
		switch (feature_id)
		{
		//表示设置采样频率的请求，暂不处理
		case 0x00: // set sample frequency
			break;
		//表示扬声器是否处于静音状态，根据控制请求的值进行设置
		case AUDIO_FEATURE_ID_SPEAKER: // Feature ID 0x02, Speaker
			//p_aud->speaker_mute表示扬声器是否处于静音状态
			p_aud->speaker_mute = usbhw_read_ctrl_ep_data();
			//表示扬声器状态已经改变
			p_aud->change |= 0x1;
			break;
		//表示处理麦克风静音控制的请求，与处理扬声器静音控制类似
		case AUDIO_FEATURE_ID_MIC: // Feature ID 0x05, MIC
			p_aud->mic_mute = usbhw_read_ctrl_ep_data();
			p_aud->change |= 0x2;
			break;
		default:
			ret = 1;
			break;
		}
		break;

	// volume control
	//音量控制
	case 0x02:
		switch (feature_id)
		{
		//表示扬声器是否处于静音状态，根据控制请求的值进行设置
		case AUDIO_FEATURE_ID_SPEAKER: // Feature ID 0x02, Speaker
			//p_aud->speaker_vol表示当前音量步进值
			p_aud->speaker_vol = usbhw_read_ctrl_ep_data() + (usbhw_read_ctrl_ep_data() << 8);
			p_aud->speaker_vol = ((p_aud->speaker_vol - (short)SPEAKER_VOL_MIN + SPEAKER_VOL_STEP) * (10 - 1)) >> 12;
			if (p_aud->speaker_vol < 0)
			{
				p_aud->speaker_vol = 0;
			}
			p_aud->change |= 0x4;//改变
			break;
		//表示处理麦克风静音控制的请求，与处理扬声器静音控制类似
		case AUDIO_FEATURE_ID_MIC: // Feature ID 0x05, MIC
			//根据音量步进值计算出实际音量的大小
			p_aud->mic_vol = usbhw_read_ctrl_ep_data() + (usbhw_read_ctrl_ep_data() << 8);
			p_aud->change |= 0x8;
			break;
		default:
			ret = 1;//请求失败
			break;
		}

	default:
		break;
	}
	return ret;
}



/// @brief 
/// @param type 
/// @return 
/// return -1 on fail,  0 on success
///扬声器命令处理
int usbaud_handle_set_speaker_cmd(int type)
{	
	//静音处理
	if (type == AUDIO_FEATURE_MUTE)
	{
		speaker_setting.mute = usbhw_read_ctrl_ep_data();
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//音量处理
		u16 val = usbhw_read_ctrl_ep_u16();
		usbaud_set_speaker_vol(val);
	}
	else
	{
		return -1;//失败
	}
	return 0;
}
// return -1 on fail,  0 on success


/// @brief 
/// @param type 
/// @return 
///麦克风
int usbaud_handle_set_mic_cmd(int type)
{	
	//静音处理
	if (type == AUDIO_FEATURE_MUTE)
	{
		mic_setting.mute = usbhw_read_ctrl_ep_data();
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//音量处理
		u16 val = usbhw_read_ctrl_ep_u16();
		usbaud_set_mic_vol(val);
	}
	else
	{
	}
	return 0;//成功
}

// return -1 on fail,  0 on success


/// @brief 
/// @param req 
/// @param type 
/// @return 
///扬声器命令处理
int usbaud_handle_get_speaker_cmd(int req, int type)
{	
	//静音处理
	if (type == AUDIO_FEATURE_MUTE)
	{	
		//写入静音指令
		usbhw_write_ctrl_ep_data(speaker_setting.mute);
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//音量处理
		//req表示请求的控制类型
		switch (req)
		{
		//获取当前音量
		case AUDIO_REQ_GetCurrent:
			usbhw_write_ctrl_ep_u16(speaker_setting.vol_cur);
			break;
		//获取最小音量
		case AUDIO_REQ_GetMinimum:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_MIN);
			break;
		//获取最大音量
		case AUDIO_REQ_GetMaximum:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_MAX);
			break;
		//获取音量步进值
		case AUDIO_REQ_GetResolution:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_RES);
			break;
		default:
			return -1;//失败
		}
	}
	else
	{
		return -1;//失败
	}
	return 0;//成功
}

// return -1 on fail,  0 on success


/// @brief 
/// @param req 
/// @param type 
/// @return 
///麦克风指令
int usbaud_handle_get_mic_cmd(int req, int type)
{	
	//静音处理
	if (type == AUDIO_FEATURE_MUTE)
	{
		usbhw_write_ctrl_ep_data(mic_setting.mute);
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//音量处理
		//req表示请求的控制类型
		switch (req)
		{
		case AUDIO_REQ_GetCurrent:
			usbhw_write_ctrl_ep_u16(mic_setting.vol_cur);
			break;
		case AUDIO_REQ_GetMinimum:
			usbhw_write_ctrl_ep_u16(MIC_VOL_MIN);
			break;
		case AUDIO_REQ_GetMaximum:
			usbhw_write_ctrl_ep_u16(MIC_VOL_MAX);
			break;
		case AUDIO_REQ_GetResolution:
			usbhw_write_ctrl_ep_u16(MIC_VOL_RES);
			break;
		default:
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}
///初始化音频设备
void usbaud_init(void)
{	//表示是否启用麦克风和扬声器设备
	if (USB_MIC_ENABLE && 1 == MIC_CHANNLE_COUNT)
	{	
		// 该函数用于设置音频模式
		// iso_en表示是否开启等时传输
		// mono_en表示是否开启单声道模式
		usbaud_set_audio_mode(1, 1);
	}
//判断是否启用扬声器
#if (USB_SPEAKER_ENABLE)
	usbaud_set_speaker_vol(SPEAKER_VOL_MAX);//最大音量
#endif
#if (USB_MIC_ENABLE)//判断是否启用麦克风
	mic_setting.vol_cur = MIC_VOL_DEF;
#endif
}

/********************************************************************************************************
 * @file     usbaud.c,�ó�����Ҫ�����Ƶ�豸
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
// ���´�����USB��Ƶ�豸�Ŀ��ƺ���������������Ƶģʽ�ͷ�����ƵHID����
static speaker_setting_t speaker_setting;
/// @brief 
static mic_setting_t mic_setting;
// �����������ṹ�壬�ô洢����������˷������



/// @brief 
/// @param iso_en 
/// @param mono_en 
/// �ú�������������Ƶģʽ
/// iso_en��ʾ�Ƿ�����ʱ����
/// mono_en��ʾ�Ƿ���������ģʽ
void usbaud_set_audio_mode(int iso_en, int mono_en)
{
	// ��ʹ��assert����в�����Ч�Լ��
	assert(USB_EDP_MIC < 8);
	// ��ʹ��SET_FLD()������USB�˵���ƼĴ������ֶ�ֵ
	// ʹ����˷�ʱ��USB�ϵ�������õĴ���
	SET_FLD(reg_usb_ep_ctrl(USB_EDP_MIC), FLD_USB_EP_EOF_ISO | FLD_USB_EP_MONO);
	// reg_usb_ep_ctrl����ʾ��ȡUSB��˷�Ķϵ�Ĵ�����ַ
	//(USB_EDP_MIC)��һ���궨�����ڱ�ʾUSB��˷�Ķϵ�

	// FLD_USB_EP_EOF_ISO����ʾ�ڶ˵���ƼĴ�����������˷�˵�Ĵ��䷽ʽ��
	// Ϊ��ʱ���䣬������ÿ�������ĩβ�Զ����һ������֡��
	// �˴�FLD_USB_EP_EOF_ISO�ǵ�ʱ����ģʽ�ı�־λ

	// FLD_USB_EP_MONO����ʾ����USB��˷����Ƶͨ��Ϊ��������
	// �˴�FLD_USB_EP_MONO�ǵ������ı�־λ

	// SET_FLD()����һ���궨�壬��������ָ���Ĵ����е��ö�λ��

	// �����������ô��������������USB��˷�Ķ˵���ƼĴ�����
	// ʹ���Ե�ʱ����ķ�ʽ������Ƶ���ݣ���������Ƶͨ��Ϊ��������
}


#if 0
/// @brief 
/// @param format 
/// @param volume 
/// �������������Ƿ���һ����ƵHID����
void usbaud_hid_report(char format, char volume) {
	//format��ʾ��Ƶ�����������ͣ�mp3��)
	//volume��ʾ����������������

	//unsigned char sAudioHidReport���ڴ洢Ҫ���͵���ƵHID����
	unsigned char sAudioHidReport[] = { 0x01,/* report ID */
	format,/*[0]:play/pause,//����������ͣ
	 [1]:scan next track,//��һ��
	 [2]:scan previous track,//��һ��
	 [3]:stop,//ֹͣ����
	 [4]:play,//��ʼ����
	 [5]:pause,//��ͣ
	 [6]:fast forward,//���
	 [7]:rewind,	//����
	 */
	volume /*[0]:volume up//��������
	 [1]:volume down//�����½�
	 [2]:mute//�������������Ƶ�豸����
	 [3:7] 0;//�������Ϊ��ʹ��ƵHID���泤�ȹ̶������ｫʣ���5��λ����Ϊ0
	 */
	};
	//��һ��Ԫ���Ǳ���HID���̶�λ0x01
	//�ڶ���Ԫ����format���������ڱ�ʾ��Ƶ�����������ͣ�������8������
	//������Ԫ����volume���������ڱ�ʾ���������������ͣ�������4������

	WriteEndPoint(EDP_ID_AUDIO_HID, sAudioHidReport, sizeof(sAudioHidReport));
	//������ָ���Ķ˵㷢������

	//1.EDP_ID_AUDIO_HID��һ��������궨��,��ʾҪ�������ݵĶ˵�ID,Ҳ����Ŀ�ĵصĶ˵㡣
	//2.sAudioHidReport��һ��ָ��Ҫ�������ݵĻ�����ָ�룬Ҳ����Ҫ���͵���ƵHID��������
	//3.sizeof(sAudioHidReport)��ʾҪ���͵����ݵĴ�С������ƵHID�������ݵĳ���
	//4.WriteEndPoint�������������е����ݷ��͵�ָ���Ķ˵㣬
	//�Ա�USB�������豸�ڽ�����Ƶ����ʱ�ܹ��յ�������
}
#endif

// if��ʾ��δ���ֻ����USB��������USB��˷繦�ܿ���ʱ�Żᱻ����ִ��
#if (USB_SPEAKER_ENABLE || USB_MIC_ENABLE) //  use for volumn control, mute, next, prev track,  move to mouse hid

/// @brief 
/// @param cmd 
/// @param vol 
/// @return 
/// usbaud_hid_report(u8 cmd, u8 vol)��������
/// ����ƵHID����Ķ˵㷢����Ƶ��������
/// cmd��ʾ��Ƶ�����������ͣ����粥��/��ͣ����һ������һ����
/// vol��ʾ��Ƶ��������Ĳ���������������С���Ƿ�����
/// �����ж���ƵHID����Ķ˵��Ƿ�æ�������æ�򷵻�0��ʾ����ʧ��
int usbaud_hid_report(u8 cmd, u8 vol)
{ 
	if (usbhw_is_ep_busy(USB_EDP_AUDIO_IN))
		return 0;
	// ���ڽ���ƵHID����Ķ˵�ָ�����㣬�Ա�Ӷ˵����ʼλ�÷�������
	reg_usb_ep_ptr(USB_EDP_AUDIO_IN) = 0;

	// please refer to keyboard_report_desc
	// ��������ƵHID����Ķ˵㷢����Ƶ������������
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = USB_HID_AUDIO;
	// ��������ƵHID����Ķ˵㷢����Ƶ�����������
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = cmd;
	// ��������ƵHID����Ķ˵㷢����Ƶ�����������
	reg_usb_ep_dat(USB_EDP_AUDIO_IN) = vol;
	// ���ڷ��������ݺ���˵㷢������ȷ���źţ���ʾ���ݷ������
	reg_usb_ep_ctrl(USB_EDP_AUDIO_IN) = FLD_EP_DAT_ACK; // ACK
	return 1;											// ��ʾ���ͳɹ�����1
}
#endif


#if 0
/// @brief 
/// @param c 
/// @return 
/// if 0�öδ��벻�ᱻ������ִ��
u8 usbaud_handle_report(u8 c) 
{	//�ú������ڴ�����ƵHID����
//c��ʾ��ƵHID��������ݣ�Ҳ������Ƶ�����������ͺͲ���
	if (USB_REPORT_NO_EVENT == c) {
		return USB_REPORT_NO_EVENT;
		//��ʾû���¼�Ҫ����
	}
	//���ڶ�����ƵHID����Ķ˵�IDС��8
	//�������ʧ�ܣ���ᴥ�����Դ���
    assert(USB_EDP_AUDIO < 8);

	//���ڶ�����ƵHID����Ķ˵��Ƿ�æ
	//�����æ���򷵻���ƵHID�������ݣ���ʾ����ʧ��
	if(reg_usb_ep_ctrl(USB_EDP_AUDIO) & FLD_USB_EP_BUSY)
		return c;

	//�����ж���ƵHID���������Ƿ�Ϊ�ͷ�״̬��
	//����ǣ�������Ƶ������������Ϊ0
	//����Ϊ0����Ƶ�����������ƵHID����Ķ˵�
	//������USB_REPORT_NO_EVENT
	if(USB_REPORT_RELEASE == c){
		usbaud_hid_report(0, 0);
		return USB_REPORT_NO_EVENT;
	}else
	{	
		//��������ͷ�״̬
		//�ͽ���Ƶ�����������ͺͲ����ֱ��͵�HID����Ķ˵㣬
		//������USB_REPORT_RELEASE
		usbaud_hid_report((c < 0x10) ? (1 << c) : 0
			,(c < 0x10) ? 0 : (1 << (c & 0x0f)));
		return USB_REPORT_RELEASE;
	}
}
#endif

/// @brief 
/// @param vol ��ʾ����ֵ
/// @return 
///�ú������ڼ���������������Ӧ����������ֵ��
///�����ݴ��������ֵ��������Ӧ����������ֵ
static u16 usbaud_cal_speaker_step(s16 vol)
{	
	//SPEAKER_VOL_MIN��ʾ��������������Сֵ

	//if�жϴ��������ֵ�Ƿ�С����Сֵ������ǣ��򷵻�0
	if (vol < SPEAKER_VOL_MIN)
		return 0;
	//SPEAKER_VOL_RES��ʾÿһ������������

	//���㴫�������ֵ��Ӧ����������ֵ
	return (vol - SPEAKER_VOL_MIN) / SPEAKER_VOL_RES;
}

/// @brief 
/// @param vol ��ʾ����ֵ
/// @return 
///�������ڼ�����˷�������Ӧ����������ֵ��
///ͬ���Ǹ��ݴ��������ֵ��������Ӧ����������ֵ
static u16 usbaud_cal_mic_step(s16 vol)
{	
	//�жϴ��������ֵ�Ƿ�С����Сֵ
	if (vol < MIC_VOL_MIN)
		return 0;
	//���㴫�������ֵ��Ӧ����������ֵ
	return (vol - MIC_VOL_MIN) / MIC_VOL_RES;
}

/// @brief 
/// @param vol 
///�������������������������������������ֵ����Ϊ��ǰ����
///����������Ӧ����������ֵ
void usbaud_set_speaker_vol(s16 vol)
{	//current
	speaker_setting.vol_cur = vol;
	//speaker_setting.vol_cur��ʾ��ǰ����������ֵ
	speaker_setting.vol_step = usbaud_cal_speaker_step(vol);
	//speaker_setting.vol_step��ʾ��ǰ������������Ӧ����������ֵ

}

/// @brief 
/// @param vol 
///�ú�������������˷�������ͬ���ǽ����������ֵ����Ϊ��ǰ����
///����������Ӧ����������ֵ
void usbaud_set_mic_vol(s16 vol)
{
	mic_setting.vol_cur = vol;
	//��ʾ��ǰ����������ֵ
	mic_setting.vol_step = usbaud_cal_mic_step(vol);
	//��ʾ��ǰ������������Ӧ����������ֵ
}

/// @brief 
///������һ��ȫ�ֱ���g_usb_audio_status��
///���ڼ�¼��ǰUSB��Ƶ��״̬
///�����Ƿ����ӡ��Ƿ���������������˷�
usb_audio_status_t g_usb_audio_status;


/// @brief 
/// @param  
/// @return 
///�ú������ڻ�ȡ��ǰ��������������������ֵ�;���״̬
u8 usbaud_speaker_vol_get(void)
{
	// return ((g_usb_audio_status.speaker_mute << 7) | g_usb_audio_status.speaker_vol);
	return (speaker_setting.mute << 7) | (speaker_setting.vol_step & 0x7f);
	//speaker_setting.mute��ʾ��ǰ�������Ƿ��ھ���״̬
	//speaker_setting.vol_stepb��ʾ��ǰ��������������ֵ
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
	//en����Ҫ���õ���˷羲��״̬,1��ʾ������0��ʾȡ������
}

/// @brief 
///��������Ƶ�ӿ��Ƿ��������
volatile int aaa_audio_intf_set = 0;



/// @brief 
/// @param type 
/// @param feature_id 
/// @return 
///�ú������ڴ�����Ƶ����ӿڵĿ�������
///type��ʾ����Ŀ�������
///feature_id��ʾ�������Ƶ����ID
int usb_audio_class_out_intf_proc(u8 type, u8 feature_id)
{	
	//ret��ʾ�Ǻ���ִ�еĽṹ��0��ʾ�ɹ���1��ʾʧ��
	int ret = 0;
	//p_aud��ָ��ȫ�ֱ���g_usb_audio_status��ָ�룬��ʾ��ǰUSB��Ƶ�豸��״̬
	usb_audio_status_t *p_aud = &g_usb_audio_status;

	//aaa_audio_intf_set��ʾ��Ƶ�ӿ��Ƿ�������ɵı�־����ʼ��Ϊ0
	aaa_audio_intf_set = 0;
	//(type << 8)|feature_id
	//���������ͺ���Ƶ����ID�Ͳ���һ��16λ�޷�������
	aaa_audio_intf_set = (type << 8);
	aaa_audio_intf_set |= feature_id;
	switch (type)
	{
	// mute control and sample
	//��ʾ���������ƺͲ�����������
	case 0x01:
		switch (feature_id)
		{
		//��ʾ���ò���Ƶ�ʵ������ݲ�����
		case 0x00: // set sample frequency
			break;
		//��ʾ�������Ƿ��ھ���״̬�����ݿ��������ֵ��������
		case AUDIO_FEATURE_ID_SPEAKER: // Feature ID 0x02, Speaker
			//p_aud->speaker_mute��ʾ�������Ƿ��ھ���״̬
			p_aud->speaker_mute = usbhw_read_ctrl_ep_data();
			//��ʾ������״̬�Ѿ��ı�
			p_aud->change |= 0x1;
			break;
		//��ʾ������˷羲�����Ƶ������봦��������������������
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
	//��������
	case 0x02:
		switch (feature_id)
		{
		//��ʾ�������Ƿ��ھ���״̬�����ݿ��������ֵ��������
		case AUDIO_FEATURE_ID_SPEAKER: // Feature ID 0x02, Speaker
			//p_aud->speaker_vol��ʾ��ǰ��������ֵ
			p_aud->speaker_vol = usbhw_read_ctrl_ep_data() + (usbhw_read_ctrl_ep_data() << 8);
			p_aud->speaker_vol = ((p_aud->speaker_vol - (short)SPEAKER_VOL_MIN + SPEAKER_VOL_STEP) * (10 - 1)) >> 12;
			if (p_aud->speaker_vol < 0)
			{
				p_aud->speaker_vol = 0;
			}
			p_aud->change |= 0x4;//�ı�
			break;
		//��ʾ������˷羲�����Ƶ������봦��������������������
		case AUDIO_FEATURE_ID_MIC: // Feature ID 0x05, MIC
			//������������ֵ�����ʵ�������Ĵ�С
			p_aud->mic_vol = usbhw_read_ctrl_ep_data() + (usbhw_read_ctrl_ep_data() << 8);
			p_aud->change |= 0x8;
			break;
		default:
			ret = 1;//����ʧ��
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
///�����������
int usbaud_handle_set_speaker_cmd(int type)
{	
	//��������
	if (type == AUDIO_FEATURE_MUTE)
	{
		speaker_setting.mute = usbhw_read_ctrl_ep_data();
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//��������
		u16 val = usbhw_read_ctrl_ep_u16();
		usbaud_set_speaker_vol(val);
	}
	else
	{
		return -1;//ʧ��
	}
	return 0;
}
// return -1 on fail,  0 on success


/// @brief 
/// @param type 
/// @return 
///��˷�
int usbaud_handle_set_mic_cmd(int type)
{	
	//��������
	if (type == AUDIO_FEATURE_MUTE)
	{
		mic_setting.mute = usbhw_read_ctrl_ep_data();
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//��������
		u16 val = usbhw_read_ctrl_ep_u16();
		usbaud_set_mic_vol(val);
	}
	else
	{
	}
	return 0;//�ɹ�
}

// return -1 on fail,  0 on success


/// @brief 
/// @param req 
/// @param type 
/// @return 
///�����������
int usbaud_handle_get_speaker_cmd(int req, int type)
{	
	//��������
	if (type == AUDIO_FEATURE_MUTE)
	{	
		//д�뾲��ָ��
		usbhw_write_ctrl_ep_data(speaker_setting.mute);
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//��������
		//req��ʾ����Ŀ�������
		switch (req)
		{
		//��ȡ��ǰ����
		case AUDIO_REQ_GetCurrent:
			usbhw_write_ctrl_ep_u16(speaker_setting.vol_cur);
			break;
		//��ȡ��С����
		case AUDIO_REQ_GetMinimum:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_MIN);
			break;
		//��ȡ�������
		case AUDIO_REQ_GetMaximum:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_MAX);
			break;
		//��ȡ��������ֵ
		case AUDIO_REQ_GetResolution:
			usbhw_write_ctrl_ep_u16(SPEAKER_VOL_RES);
			break;
		default:
			return -1;//ʧ��
		}
	}
	else
	{
		return -1;//ʧ��
	}
	return 0;//�ɹ�
}

// return -1 on fail,  0 on success


/// @brief 
/// @param req 
/// @param type 
/// @return 
///��˷�ָ��
int usbaud_handle_get_mic_cmd(int req, int type)
{	
	//��������
	if (type == AUDIO_FEATURE_MUTE)
	{
		usbhw_write_ctrl_ep_data(mic_setting.mute);
	}
	else if (type == AUDIO_FEATURE_VOLUME)
	{	
		//��������
		//req��ʾ����Ŀ�������
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
///��ʼ����Ƶ�豸
void usbaud_init(void)
{	//��ʾ�Ƿ�������˷���������豸
	if (USB_MIC_ENABLE && 1 == MIC_CHANNLE_COUNT)
	{	
		// �ú�������������Ƶģʽ
		// iso_en��ʾ�Ƿ�����ʱ����
		// mono_en��ʾ�Ƿ���������ģʽ
		usbaud_set_audio_mode(1, 1);
	}
//�ж��Ƿ�����������
#if (USB_SPEAKER_ENABLE)
	usbaud_set_speaker_vol(SPEAKER_VOL_MAX);//�������
#endif
#if (USB_MIC_ENABLE)//�ж��Ƿ�������˷�
	mic_setting.vol_cur = MIC_VOL_DEF;
#endif
}

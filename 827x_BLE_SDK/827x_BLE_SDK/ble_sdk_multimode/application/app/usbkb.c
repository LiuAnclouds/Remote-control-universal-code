/********************************************************************************************************
 * @file     usbkb.c ,�ó�����Ҫ��Լ���
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
/*������õ���֪ʶ*/
/*
1. ��USB HIDЭ���У�
ϵͳ������ý�尴����ͨ����ͬ��Usage Page��Usage ID�����ֵġ�
ϵͳ������Usage PageΪ0x01��Usage IDΪ0x80��
ý�尴����Usage PageΪ0x0C��Usage IDΪ0x01��
2. USB HIDЭ��涨���ͷŰ���ʱ��ֻ��Ҫ����һ������Ϊ1�����ݱ��ģ�
������������Ϊ0����ˣ����ͷ�ϵͳ����ʱ��
ֻ��Ҫ����һ������Ϊ1�����ݱ��ģ�����������Ϊ0�����ͷ�ý�尴��ʱ��
��Ҫ����һ������Ϊ`MEDIA_REPORT_DATA_LEN`�����ݱ��ģ�
��������ȫ����Ϊ0��
*/

#include "tl_common.h"

#if(1)




#include "drivers.h"
#include "usbkb.h"
#include "usbmouse.h"
#include "../usbstd/usb.h"
#include "../usbstd/usbhw.h"
#include "../usbstd/usbhw_i.h"
#include "../usbstd/usbkeycode.h"

#include "../rf_frame.h"


/// @brief 
///������
u8 usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];

/// @brief 
///��������ȡָ��
u8 usb_ff_rptr = 0;
/// @brief 
///������д��ָ��
u8 usb_ff_wptr = 0;


int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode);
void usbkb_hid_report(kb_data_t *data);

/// @brief 
///��̬���飬����ϵͳ�����������ֵ
static u8 vk_sys_map[VK_SYS_CNT] = {
	//��Դ��   	˯�߼�		���Ѽ�
	VK_POWER_V, VK_SLEEP_V, VK_WAKEUP_V
};
/// @brief 
///��̬���飬����ý�尴���������ֵ
static vk_ext_t vk_media_map[VK_MEDIA_CNT] = {
    {VK_W_SRCH_V},/*����������*/
    {VK_HOME_V},/*��ҳ��*/
    {VK_W_BACK_V},/*��ҳ���˼�*/
    {VK_W_FORWRD_V},/*��ҳǰ����*/
    {VK_W_STOP_V},/*��ҳֹͣ��*/
    {VK_W_REFRESH_V},/*��ҳˢ�¼�*/
    {VK_W_FAV_V},/*��ҳ�ղؼ�*/
    {VK_MEDIA_V},/*ý���*/
    {VK_MAIL_V},/*��ʾ�ʼ���*/
    {VK_CAL_V},/*��ʾ������*/
    {VK_MY_COMP_V},/*��ʾ�ҵĵ��Լ�*/
    {VK_NEXT_TRK_V},/*��ʾ��һ����*/
    {VK_PREV_TRK_V},/*��ʾ��һ����*/
    {VK_STOP_V},/*��ʾֹͣ��*/
    {VK_PLAY_PAUSE_V},/*��ʾ������ͣ��*/
    {VK_W_MUTE_V},/*��ʾ������*/
    {VK_VOL_UP_V},/*��ʾ�������߼�*/
    {VK_VOL_DN_V},/*��ʾ������С��*/
};
/// @brief 
///ö�ٱ������ں����̵��¼�����
enum{
	/*��ͨ����*/
    KB_NORMAL_RELEASE_MASK = BIT(0),
	/*ϵͳ����*/
    KB_SYS_RELEASE_MASK = BIT(1),
	/*ý�尴��*/
    KB_MEDIA_RELEASE_MASK = BIT(2),
};



#define  USBKB_BUFF_DATA_NUM    8
//static

/// @brief 
///�������ݻ�����
kb_data_t kb_dat_buff[USBKB_BUFF_DATA_NUM];

//static
u8  usbkb_wptr, usbkb_rptr;
/// @brief ��ʾ��ǰUSB����δ�ͷŵİ�������
static u32 usbkb_not_released;
/// @brief ��ʾ��һ��USB���̷������ݵ�ʱ��
static u32 usbkb_data_report_time;


/// @brief ���������ݵ����λ�����kb_dat_buff��
/// @param packet_kb 
void usbkb_add_frame (rf_packet_keyboard_t *packet_kb)
{	
	 //����pno ����������ݵĸ���
	u8 new_data_num = packet_kb->pno; 
	for(u8 i=0;i<new_data_num;i++)
	{
			memcpy4((int*)(&kb_dat_buff[usbkb_wptr]), (int*)(&packet_kb->data[i*sizeof(kb_data_t)]), sizeof(kb_data_t));
			//ÿ�θ�����Ϻ󣬽�дָ��+1
			BOUND_INC_POW2(usbkb_wptr,USBKB_BUFF_DATA_NUM);
			//���дָ��׷�϶�ָ��
			if(usbkb_wptr == usbkb_wptr)
			{
					break;
			}
	}
}

/// @brief ����USB�������ݱ���
/// @param  
void usbkb_report_frame(void)
{	
	//���дָ�벻���ڶ�ָ�룬˵���м���������Ҫ����
	if(usbkb_wptr != usbkb_rptr){
		//�ӻ������ж�ȡһ���������ݽṹ
		kb_data_t *data = (kb_data_t *)(&kb_dat_buff[usbkb_rptr]);
        //���ͼ������ݽṹ
		usbkb_hid_report(data);
		//����ָ��+1
		BOUND_INC_POW2(usbkb_rptr,USBKB_BUFF_DATA_NUM);
	}
	return;
}
/// @brief �ͷ�δ�ͷŵ���ͨ����
/// @param  
static void usbkb_release_normal_key(void){
	//���usbkb_not_released�е���ͨ�����ͷű�־λ������
	//˵������ͨ������Ҫ�ͷ�
	//KB_NORMAL_RELEASE_MASK����ͨ����
	if(usbkb_not_released & KB_NORMAL_RELEASE_MASK){
		//������δ�ͷŵ���ͨ�����ļ�������
		u8 normal_keycode[KEYBOARD_REPORT_KEY_MAX] = {0};
		//����usbkb_hid_report_normal(0, normal_keycode)����
		//���������ݱ��ģ����ͷ���Щ����
		//������ͳɹ�������ͨ�����ͷű�־λ����
		if(usbkb_hid_report_normal(0, normal_keycode)){
			//λ���㺯��������ͨ�����ͷű�־λ����
			BM_CLR(usbkb_not_released, KB_NORMAL_RELEASE_MASK);
		}
	}
}
/// @brief �ͷ�δ�ͷŵ�ϵͳ����
/// @param  
static void usbkb_release_sys_key(void){
	//���usbkb_not_released�е�ϵͳ�����ͷű�־λ������
	//˵������ͨ������Ҫ�ͷ�
	//KB_SYS_RELEASE_MASK��ϵͳ����
	if(usbkb_not_released & KB_SYS_RELEASE_MASK){
		//������δ�ͷŵ�ϵͳ�����ļ�������
		u32 release_data = 0;
		//����usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&release_data)����
		//���������ݱ��ģ����ͷ���Щ����
		//������ͳɹ�����ϵͳ�����ͷű�־λ����
		if(usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&release_data), 1)){
			//λ���㺯������ϵͳ�����ͷű�־λ����
			BM_CLR(usbkb_not_released, KB_SYS_RELEASE_MASK);
		}
	}
}
/// @brief �ͷ�δ�ͷŵ�ý�尴��
/// @param  
static void usbkb_release_media_key(void){
	//���usbkb_not_released�е�ý�尴���ͷű�־λ������
	//˵������ͨ������Ҫ�ͷ�
	//KB_SYS_RELEASE_MASK��ý�尴��
	if(usbkb_not_released & KB_MEDIA_RELEASE_MASK){
		//������δ�ͷŵ�ý�尴���ļ�������
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
		//����usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)����
		//���������ݱ��ģ����ͷ���Щ����
		//������ͳɹ�����ý�尴���ͷű�־λ����
		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//λ���㺯������ϵͳ�����ͷű�־λ����
			BM_CLR(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}
}
/// @brief �����ͷŰ�������������Ҫ��ͽ���
/// @param  
static void usbkb_release_keys(void){
	// ��Ҫ����usbkb_release_sys_key ��usbkb_release_normal_key ��˳��
	// ����Ϊ���� usbkb_release_sys_key,  usbkb_release_media_key �м���һ����ʱ
	usbkb_release_sys_key();
	usbkb_release_normal_key();
	usbkb_release_media_key();
}
/// @brief ����Ƿ���δ�ͷŵİ���
void usbkb_release_check(){
	//����У��ҳ����˼��ʱ�䣬������ͷź������ͷ�
	if(usbkb_not_released && clock_time_exceed(usbkb_data_report_time, USB_KEYBOARD_RELEASE_TIMEOUT)){
		usbkb_release_keys();	 //  release keys
	}

}
/// @brief ������ļ��������еİ������ͷ����������������ͨ�����ĸ���
/// @param keycode ��������ָ��
/// @param cnt ��������
/// @param normal_key //���ڴ洢��ͨ����������ָ��
/// @param ext_key //���ڴ洢��չ������ָ��
/// @return 
///  normal_keycode ���淵����ͨ����
int usbkb_separate_key_types(u8 *keycode, u8 cnt, u8 *normal_key, u8 *ext_key){
    //һ�������ڼ�Ķ��ԣ������ж�KB_RETURN_KEY_MAX�Ƿ�С�ڵ���KEYBOARD_REPORT_KEY_MAX
    //����������������������޷�ͨ��
	STATIC_ASSERT(KB_RETURN_KEY_MAX <= KEYBOARD_REPORT_KEY_MAX);
    //����ʱ�Ķ��ԣ������жϴ���ļ��������Ƿ�С�ڵ���KB_RETURN_KEY_MAX
	//�����������������������ն�
	assert(cnt <= KB_RETURN_KEY_MAX);
	//��ͨ��������
    int normal_cnt = 0;
	//�������飬�ж�����չ����������ͨ����
	//VK_EXT_START/End:������USB HIDЭ���е���չ�������ʼ�ͽ���ֵ��
	//���������ڵ���VK_EXT_START��С��VK_EXT_End����˵���ð���Ϊ��չ����
    foreach(i, cnt){
        if(keycode[i] >= VK_EXT_START && keycode[i] < VK_EXT_END){
        	*ext_key = keycode[i];//��չ����
        }else{
            normal_key[normal_cnt++] = keycode[i];//��ͨ����
        }
    }
	return normal_cnt;
}
/// @brief ����ͨ�����ļ���Ϳ��Ƽ������USB HID���沢���͵�����
/// @param ctrl_key ���Ƽ�
/// @param keycode ��������ָ��
/// @return 
int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode){
	//�����ж�USB EDI�Ƿ�æµ��æµ�򷵻ط�0ֵ
	if(usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN)){
		//��USB FIFO�����������е���һ�����ÿռ��ַ����pDataָ��
		//����usb_ff_wptr��ֵ+1��ʹ��ָ����һ�����ÿռ�
		//& (USB_FIFO_NUM - 1)�������ǽ�usb_ff_wptr�������䷶Χ�ڣ���ʵ��ѭ������
		u8 *pData = (u8 *)&usb_fifo[usb_ff_wptr++ & (USB_FIFO_NUM - 1)];
		//���������ͱ��Ϊ������������
		pData[0] = DAT_TYPE_KB;
		//�����Ƽ�д�뱨��������
		pData[1] = ctrl_key;
		//�����������е�ǰ6�����븴�Ƶ�����������
		memcpy(pData + 2, keycode, 6);
		//����USB FIFO����ʹ�õĿռ��С�������ж��Ƿ������
		int fifo_use = (usb_ff_wptr - usb_ff_rptr) & (USB_FIFO_NUM*2-1);
		//�ж��Ƿ�������������ʹ�õĿռ��С����USB FIFO�Ĵ�С�����������
		if (fifo_use > USB_FIFO_NUM) {
			usb_ff_rptr++;//����ֵ+1���������������
			//fifo overflow, overlap older data
		}

		return 0;
	}
	//��USB EDI��ָ������Ϊ0����ʾ��EDP�Ѿ�������ϡ�
	reg_usb_ep_ptr(USB_EDP_KEYBOARD_IN) = 0;



///���´��������ּ������ݷ���ģʽ
///һ���Ǵ���CRCУ��ķ��ͷ�ʽ
///��һ���ǲ���CRCУ��ķ��ͷ�ʽ

//�궨���Ƿ������CRCУ�鹦��
#if USB_SOFTWARE_CRC_CHECK

	//crc_in���ڴ洢��Ҫ����CRCУ�������
	unsigned char crc_in[KEYBOARD_REPORT_KEY_MAX+2];
	//crc��crch�ֱ��ʾCRCУ��ֵ��CRCУ��ֵ�ĸ�8λ
	unsigned short crc;
	unsigned int crch;

	crc_in[0] =  ctrl_key;
	crc_in[1] =  0;
	//���ڱ������飬ʵ�ֶ�crc_in����ĳ�ʼ��
	foreach(i, KEYBOARD_REPORT_KEY_MAX){
		crc_in[i+2] = keycode[i];
	}
	//USB_CRC16���ڼ���CRCУ��ֵ
	crc = USB_CRC16 (crc_in, KEYBOARD_REPORT_KEY_MAX+2);
	//����8λ
	crch = crc >> 8;
	//�����ж��Ƿ���Ҫ��У��ֵ���н���
	if ((crch==0x06) || (crch==0x04) || (crch == 0x00))
	{
		unsigned int tmp = crc_in[2];
		crc_in[2] = crc_in[3];
		crc_in[3] = tmp;
	}
	// please refer to keyboard_report_desc
	foreach(i, (KEYBOARD_REPORT_KEY_MAX+2)){
		//�����ݷ��͵�USB�˵�
	    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = crc_in[i];
	}

#else
	//����У������
	// please refer to keyboard_report_desc
	//�����ݷ��͸�USB�˿�
	//ctrl_key�����Ƽ�
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = ctrl_key;
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = 0;//resv
	foreach(i, KEYBOARD_REPORT_KEY_MAX){
		//��������ָ��
	    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = keycode[i];
	}

#endif
//	reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK;		// ACK
	//����USB�˵㣬������ACKӦ���źţ���ʾ�����Ѿ��������
	reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK | (edp_toggle[USB_EDP_KEYBOARD_IN] ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
	///���������ڼ�¼USB�˵�����ݴ���״̬��ÿ�η�������ʱ��
	///��Ҫ������ֵ��ֵ,��ѡ��ͬ�����ݻ�������
	///^=��ʾ������
	edp_toggle[USB_EDP_KEYBOARD_IN] ^= 1;

	return 1;
}
/// @brief �ú������ڷ�����ͨ����������
/// @param ctrl_key ��������״̬
/// @param keycode ���µļ�ֵ
/// @param cnt ��ʾ���µļ���
static inline void usbkb_report_normal_key(int ctrl_key, u8 *keycode, int cnt){
	//�����ж��Ƿ���Ҫ�������ݣ����cnt����0����ctrl_key��Ϊ0��
	//����Ҫ��������
	if(cnt > 0 || ctrl_key)
	{	
		//������ͨ��������
		if(usbkb_hid_report_normal(ctrl_key, keycode)){
			//BM_SE��������usbkb_not_released��ֵ����ʾδ�ͷ�
		    BM_SET(usbkb_not_released, KB_NORMAL_RELEASE_MASK);
		}
	}else//����Ҫ�������ݣ���ʾû�а������£��������а������Ѿ��ͷ�
	{	
		//�ͷ�������ͨ����
		usbkb_release_normal_key();
	}
}
/// @brief �ú������ڷ���ϵͳ����������
/// @param ext_key ��ʾ���µ�ϵͳ��ֵ
static inline void usbkb_report_sys_key(u8 ext_key)
{	
	//�ж��Ƿ�Ϊϵͳ��ֵ
	if(ext_key >= VK_SYS_START && ext_key < VK_SYS_END)
	{
		//��ʾϵͳ��ֵ��VK_STS_MAP(ϵͳ����)�ϵ��±�
		int idx = ext_key - VK_SYS_START;

		if(usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&vk_sys_map[idx]), 1)){	// assert sys key len == 1, check descriptor
			//BM_SE��������usbkb_not_released��ֵ����ʾδ�ͷ�
			BM_SET(usbkb_not_released, KB_SYS_RELEASE_MASK);
		}
	}else//����Ҫ�������ݣ���ʾû�а������£��������а������Ѿ��ͷ�
	{	
		//�ͷ�������ͨ����
		usbkb_release_sys_key();
	}
}
/// @brief �ú������ڷ���ý�尴��������
/// @param ext_key ��ʾ���µ�ý���ֵ
static inline void usbkb_report_media_key(u8 ext_key){
	//�ж��Ƿ�Ϊý���ֵ
	if(ext_key >= VK_MEDIA_START && ext_key < VK_MEDIA_END){
		STATIC_ASSERT(VK_EXT_LEN <= MOUSE_REPORT_DATA_LEN);
	//ý���ֵ��ʼ��Ϊ0
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
	//ý���ֵ��vk_media_map(ý�����)�е��±�
		int idx = ext_key - VK_MEDIA_START;
		//ѭ����ý���ֵ��ֵ��ext_keycode����
		foreach(i, VK_EXT_LEN){
			ext_keycode[i] = vk_media_map[idx].val[i];
		}

		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//BM_SE��������usbkb_not_released��ֵ����ʾδ�ͷ�
			BM_SET(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}else//����Ҫ�������ݣ���ʾû�а������£��������а������Ѿ��ͷ�
	{	
		//�ͷ�������ͨ����
		usbkb_release_media_key();
	}
}

/// @brief �ú������ڷ��������߰���������
/// @param consumer_key ��ʾ���µ������߼�ֵ
void usbkb_report_consumer_key(u16 consumer_key)
{	
	//�ж��Ƿ��������߼�ֵ
	if(consumer_key)
	{
		//���������߼�ֵ��ֵ0����ʼ����
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
		//ѭ���������߼�ֵ��ֵ��ext_keycode����
		foreach(i, VK_EXT_LEN){
			ext_keycode[i] = consumer_key;
			consumer_key >>=8;
		}

		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//���º��ʾδ�ͷ�
			//BM_SE��������usbkb_not_released��ֵ����ʾδ�ͷ�
			BM_SET(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}else//����Ҫ�������ݣ���ʾû�а������£��������а������Ѿ��ͷ�
	{	
		//�ͷ�������ͨ����
		usbkb_release_media_key();
	}

	//���ڼ�¼���ݱ����ʫ�䣬�������������һ�η��͵�ʱ��
    usbkb_data_report_time = clock_time();

}


/// @brief �ú������ڱȽ��������������Ƿ���ͬ
/// @param a ��������1��ָ��
/// @param b ��������2��ָ��
/// @return 
int kb_is_data_same(kb_data_t *a, kb_data_t *b)
{	
	//���a��bΪ��ָ��
	if(!a || !b){
		return 0;
	}
	//��������������ݵİ�����������Ƽ�����ͬ
	if((a->cnt != b->cnt) || (a->ctrl_key != b->ctrl_key))
		return 0;
	//ѭ�����ڱȽ������������ݵ�ÿ������ֵ
	foreach(i, a->cnt)
	{	
		//����в���ͬ
		if(a->keycode[i] != b->keycode[i])
		{
			return 0;
		}
	}
	//�������ͬ������1
	return 1;
}
/// @brief �ú������ڼ����̰����Ƿ��ظ������ұ������һ�εļ�������
/// @param data ��ǰ��������
/// @return 
static inline int usbkb_check_repeat_and_save(kb_data_t *data){
    //����assert����dat�Ƿ�Ϊ��ָ��
	assert(data);
	//����һ����̬���������ڱ������һ�εļ�������
	static kb_data_t last_data;
	//�жϵ�ǰ�ļ������ݺ����һ�εļ��������Ƿ���ͬ
	int same = kb_is_data_same(&last_data, data);
	//�������ͬ
	if(!same)
	{	
		//�����ݶ�������last_data��̬������
		STATIC_ASSERT(sizeof(last_data) == 8);
	    ((u32*) (&last_data))[0] = ((u32*) (data))[0];
	    ((u32*) (&last_data))[1] = ((u32*) (data))[1];
	}
	//����same˵���Ƿ��ظ�
	return same;
}

/// @brief ���ڴ���������ݲ�����HID����
/// @param data ��ʾ��ǰ�ļ�������
void usbkb_hid_report(kb_data_t *data){
	//��ʹ��assert���data�Ƿ�Ϊ��ָ��
    assert(data);
	//ext_key��չ������,normal_key_cnt��ͨ������
    u8 ext_key = VK_EXT_END, normal_key_cnt = 0;
	//���ڱ�����������
    u8 normal_keycode[KEYBOARD_REPORT_KEY_MAX] = {0};

	//�����ǰ�������ݵ���������KB_RETURN_KEY_MAX,��ֱ�ӷ��أ�Ϊ�˷�ֹ�������ݰ���Ӱ��
	if(data->cnt > KB_RETURN_KEY_MAX){	// must,   in case bad packets
		return;
	}

	/*	http://msdn.microsoft.com/en-us/windows/hardware/gg462991.aspx
		It is also important to notice that any re-triggering of events should be done by software timers in the host system and not by hardware timers in the device itself.
		For example, if the user keeps pressing the Volume Increment button, the device should only generate one input report with this state information
	*/
	//�����ǰû�а�������
	if((data->cnt > 0 || data->ctrl_key) && usbkb_check_repeat_and_save(data)){
		if(usbkb_not_released){
			usbkb_data_report_time = clock_time();
			return;
		}
	}

	//�����ǰ�а�������
	if(data->cnt > 0){
		//  keycode���ദ��,��Ϊ��չ��������������
	    normal_key_cnt = usbkb_separate_key_types(data->keycode, data->cnt, normal_keycode, &ext_key);
	}
	//������ͨ��������
	usbkb_report_normal_key(data->ctrl_key, normal_keycode, normal_key_cnt);
	//����ϵͳ��������
	usbkb_report_sys_key(ext_key);
	//����ý�尴������
	usbkb_report_media_key(ext_key);
	//���һ�η��ͱ����ʱ��
    usbkb_data_report_time = clock_time();
}

/// @brief �ú������ڶԼ��̰������г�ʼ��
void usbkb_init(){
	//ev_on_poll(EV_POLL_KEYBOARD_RELEASE_CHECK, usbkb_release_check);
}

/// @brief ���ڴ���USB���ݱ�����в�����HID����
/// @param  
/// @return 
int usb_hid_report_fifo_proc(void)
{	
	//�����ָ����дָ����ȣ����ʾ������û�����ݣ�ֱ�ӷ���
	if(usb_ff_rptr == usb_ff_wptr){
		return 0;
	}
	//ָ��usb_fifo�����У���ȡָ�����ڵĵ�ַ
	u8 *pData = (u8 *)&usb_fifo[usb_ff_rptr & (USB_FIFO_NUM - 1)];
	//�����ǰ�����Ǽ�������
	if(pData[0] == DAT_TYPE_KB)
	{
		//����usbhw_is_ep_busy�����ж��Ƿ��ж˵��ڴ�������
		if(usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN))
		{
			return 0;
		}
		else{
			//���û��
			//���ú������ͼ���HID����
			usbkb_hid_report_normal(pData[1], pData + 2);
			//��ָ��+1
			usb_ff_rptr ++;

			return 1;
		}
	}
	else if(pData[0] == DAT_TYPE_MOUSE)//���δ�������
	{	
		//����usbhw_is_ep_busy�����ж��Ƿ��ж˵��ڴ�������
		if(usbhw_is_ep_busy(USB_EDP_MOUSE))
		{
			return 0;
		}
		else//û�ж˵��ڴ�������
		{
			//���ú������ͼ���HID����
			usbmouse_hid_report(pData[1], pData + 4, pData[2]);
			//��ָ��+1
			usb_ff_rptr ++;

			return 1;
		}
	}


	return 0;
}


#endif

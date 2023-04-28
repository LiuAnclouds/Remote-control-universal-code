/********************************************************************************************************
 * @file     usbmouse.c ���ó�����Ҫ������
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

#include "tl_common.h"
#if(USB_MOUSE_ENABLE)

#include "usbmouse.h"
#include "usbkb.h"
#include "../usbstd/usb.h"
#include "../usbstd/usbhw.h"
#include "../usbstd/usbhw_i.h"
#include "../rf_frame.h"
//�öδ��붨����һ���꣬�����ж��Ƿ������ƽ��ģʽ
//���û�ж��壬��Ĭ�Ϲر�ƽ��ģʽ
#ifndef	USB_MOUSE_REPORT_SMOOTH
#define	USB_MOUSE_REPORT_SMOOTH	0
#endif

//�ô��궨��������ݻ������ĳ���Ϊ8
#define  USBMOUSE_BUFF_DATA_NUM    8
/// @brief ��껺����
static mouse_data_t mouse_dat_buff[USBMOUSE_BUFF_DATA_NUM];
/// @brief ��дָ��
static u8  usbmouse_wptr, usbmouse_rptr;
/// @brief USB���ݻ�������δ�ͷŵ���������
static u32 usbmouse_not_released;
/// @brief ��ʾUSB������ݱ���ʱ�䣬���ڿ���USB���ݵķ���Ƶ��
static u32 usbmouse_data_report_time;


/// @brief ���ڽ����յ���rf_packet_mouse_t���͵�����֡��ӵ�USB������ݻ�����
/// @param packet_mouse 
void usbmouse_add_frame (rf_packet_mouse_t *packet_mouse){
	//����pno ����������ݵĸ���
	u8 new_data_num = packet_mouse->pno;  
	//ѭ��
	for(u8 i=0;i<new_data_num;i++)
	{		
			//����memcpy4�������������ݼ��뻺����
			memcpy4((int*)(&mouse_dat_buff[usbmouse_wptr]), (int*)(&packet_mouse->data[i*sizeof(mouse_data_t)]), sizeof(mouse_data_t));
			//��дָ����ǰ�ƽ�һ��λ��
			//�ú����������ǽ�ָ��ֵ+1
			BOUND_INC_POW2(usbmouse_wptr,USBMOUSE_BUFF_DATA_NUM);
			//�����дָ����ȣ���������������
			if(usbmouse_wptr == usbmouse_rptr)
			{
					//BOUND_INC_POW2(usbmouse_rptr,USBMOUSE_BUFF_DATA_NUM);
					break;
			}
	}
}

/// @brief �����껺�������Ƿ���δ�ͷŵ�����
void usbmouse_release_check(){
	//usbmouse_not_released=0˵��û��δ�ͷŵ�����
	//clock_time_exceed(usbmouse_data_report_time, USB_MOUSE_RELEASE_TIMEOUT)
	//���ʱ�����˺�һ����������˵����Ҫ���ͱ���
	if(usbmouse_not_released && clock_time_exceed(usbmouse_data_report_time, USB_MOUSE_RELEASE_TIMEOUT)){
	    //���ͷŰ�����ֵ0
		u32 release_data = 0;
		//���ú������ͱ���
	    if(usbmouse_hid_report(USB_HID_MOUSE, (u8*)(&release_data), MOUSE_REPORT_DATA_LEN)){
		    //Ϊ�ͷŵİ�������Ϊ0
			usbmouse_not_released = 0;
	    }
	}
}

/// @brief �ú������ڴ�USB������ݻ�������ȡ���ݣ��������ݷ��ͳ�ȥ
void usbmouse_report_frame(){
//������Ϊƽ��ģʽ
#if 	USB_MOUSE_REPORT_SMOOTH
	static u32 tick = 0;
	//��� USB�˵�����æµ�У����¼��ǰʱ�䲢���أ��ȴ���һ�ε���
	if(usbhw_is_ep_busy(USB_EDP_MOUSE)) {
			tick = clock_time ();
	}
	//���㻺������δ�����ݵĸ���
	u8 diff = (usbmouse_wptr - usbmouse_rptr) & (USBMOUSE_BUFF_DATA_NUM - 1);
	//���δ��ȡ���ݵĸ���С��3��
	//�Ҿ����ϴε��õ�ʱ�䲻����5000��ʱ�����ڣ��򷵻أ��ȴ���һ�ε���
	if (diff < 3 && !clock_time_exceed (tick, 5000)) {
		return;
	}
#endif
	//�����ָ�벻����дָ�룬˵���뻺��������δ��ȡ������
	if(usbmouse_wptr != usbmouse_rptr){
		//������������ɵ����ݶ�ȡ���������ҽ���ת��Ϊu32���͵ı���
        u32 data = *(u32*)(&mouse_dat_buff[usbmouse_rptr]);	// that is   >  0
        //�������������USB��������������ݱ���
		int ret = usbmouse_hid_report(USB_HID_MOUSE,(u8*)(&data), MOUSE_REPORT_DATA_LEN);
		if(ret){
			//������ָ����ǰ�ƽ�һ��λ��
            BOUND_INC_POW2(usbmouse_rptr,USBMOUSE_BUFF_DATA_NUM);
		}
		if(0 == data && ret){			//  successfully  release the key
			//δ�ͷŵİ���Ϊ0
			usbmouse_not_released = 0;
		}else{
			//��δ�ͷŵİ���
			usbmouse_not_released = 1;
			//���һ�η������ݵ�ʱ��
			usbmouse_data_report_time = clock_time();
		}
	}
	return;
}

/// @brief �ú������������ڷ�����갴�����ݵ�HID����
/// @param report_id ��ʾ����������
/// @param data ��ʾ���µİ����ĵ�ַ
/// @param cnt ��ʾ��ֵ�ĳ���
/// @return 
int usbmouse_hid_report(u8 report_id, u8 *data, int cnt){
	//unsigned char crc_in[8];
	//unsigned short crc;
	//unsigned int crch;
	//�����ж��Ƿ�cnt<8
    assert(cnt<8);
	//���USB���˵���æµ
	if(usbhw_is_ep_busy(USB_EDP_MOUSE)){

		//pData��ʾ���������ݣ�sub_fifo��ʾ������
		u8 *pData = (u8 *)&usb_fifo[usb_ff_wptr++ & (USB_FIFO_NUM - 1)];
		pData[0] = DAT_TYPE_MOUSE;//�������
		pData[1] = report_id;//����ID
		pData[2] = cnt;//���ݳ���
		//������data��ֵ��pData�У��������ݿ�������������
		memcpy(pData + 4, data, cnt);
		//fifo_use��ʾ�������Ѿ�ʹ�õĿռ��С
		int fifo_use = (usb_ff_wptr - usb_ff_rptr) & (USB_FIFO_NUM*2-1);
		if (fifo_use > USB_FIFO_NUM) {
			usb_ff_rptr++;
			//fifo overflow, overlap older data
		}

		return 0;//����ʧ��
	}
	//��������USB�Ķ˵�ָ�롣
	//���ڷ���������Ҫ�Ӷ˵�Ļ������ж�ȡ���ݣ������Ҫ������ָ��Ϊ0
	reg_usb_ep_ptr(USB_EDP_MOUSE) = 0;

	// please refer to usbmouse_i.h mouse_report_desc
	//���������ʾUSB���ı���Э�����ͣ�
	extern u8 usb_mouse_report_proto;
	//���Ϊ0����ʹ������׼����Э��
	if (!usb_mouse_report_proto) {
		//��˵㻺������д������
		//���η���3���ֽڵ�����
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[0];
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[1];
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[2];
	}
	else //����ʹ�������չ����Э��
	{	
		//���뻺������д�뱨��ID
		reg_usb_ep_dat(USB_EDP_MOUSE) = report_id;
		//��˵㻺������д������
		foreach(i, cnt){
			reg_usb_ep_dat(USB_EDP_MOUSE) = data[i];
		}
	}
//	reg_usb_ep_ctrl(USB_EDP_MOUSE) = FLD_EP_DAT_ACK;		// ACK
	//�ú�����������USB���Ķ˵���ƼĴ���
	//���ƼĴ����ж��λ�������������ƶ˵�Ĺ���״̬
	//�����Ƿ���Ҫ�������ݡ����ݷ�����Ϻ��Ƿ���Ҫȷ��
	reg_usb_ep_ctrl(USB_EDP_MOUSE) = FLD_EP_DAT_ACK | (edp_toggle[USB_EDP_MOUSE] ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
	//FLD_EP_DAT_ACK��ʾ���ݷ�����Ϻ���Ҫ����ACK(ȷ���ź�)��
	//�Ը�֪USB�����Ѿ��ɹ����յ�����

	//FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0�ֱ��ʾ����״̬Ϊ0��1
	//ÿ�δ������Ҫ�����ݴ���״̬ȡ�����Ա��´δ���ʹ�ò�ͬ�����ݴ���״̬

	//�ñ�����һ����־λ�����ڼ�¼��ǰ���ݰ��Ĵ���״̬��
	//USB���ݴ���ʱ��ÿת��һ�����ݰ�����Ҫ�л�һ�����ݴ���״̬
	//��ȷ�����ݴ������ȷ��
	edp_toggle[USB_EDP_MOUSE] ^= 1;

	return 1;//�������
}


void usbmouse_init(){
}

#endif

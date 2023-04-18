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
//��Щͷ�ļ��������Ƕ�����һЩ�������ṹ��ͺ�����
//���� USB ��Ƶ�豸���������򿪷���
//���а�����һЩͨ�õ� USB ��Ƶ�ඨ�塢USB ��Ƶ�豸��Ϣ�ṹ�塢
//USB ��Ƶ�豸���������ȡ�
//��Щ����ͺ������԰��������߸������ʵ�� USB ��Ƶ�豸�Ĺ��ܡ�




//���´���ʹ�����������룬#if 0��ʾ��������е����ݶ�ע�͵���
//�൱����ʱ�����ⲿ�ִ���
//�ô�����У�������һ��USB HID������������������USB��Ƶ��ʵ�HID�����ʽ��
//��HID���������������˶��HID�����ÿ���������Ӧһ��HID������
//�粥�š���ͣ����һ������һ����
#if 0
static const USB_Descriptor_HIDReport_Datatype_t usbaud_report_desc[] = {
	//ָ��ʹ�õ�HIDҳ��Ϊ������ҳ�档
    HID_RI_USAGE_PAGE(8, 0x0c), /* Consumer Page */
    //ָ��ʹ�õ�HID��������Ϊ�����߿���
	HID_RI_USAGE(8, 0x01)		, /* Consumer Controls */
    //����HID�����������ļ�������ΪӦ�ó���
    HID_RI_COLLECTION(8, 0x01)		, /* Application */
    //����HID�������ID��Ϊ1
    HID_RI_REPORT_ID(8, 0x01)		, /*Report ID*/
    //����HID��������߼���СֵΪ0
    HID_RI_LOGICAL_MINIMUM(8, 0x00)		,
    //����HID��������߼����ֵΪ1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),
    /***����Ϊ�����߿��Ʋ������͵�HID���涨���**/
    //ָ��������Ϊ����/��ͣ���л���
    HID_RI_USAGE(8, 0xcd),  /* Play/Pause (toggle) */
    //ָ����������Ϊ��һ��
	HID_RI_USAGE(8, 0xb5)		, /* Next Track */
    //ָ����������Ϊ��һ��
	HID_RI_USAGE(8, 0xb6)		, /* Previous Track */
    //ָ����������Ϊֹͣ
	HID_RI_USAGE(8, 0xb7)		, /* Stop */
    /***����HID���涨����������������߿��Ʋ������͵��������*****/
    
    //����ÿ������λ��Ϊ1
    HID_RI_REPORT_SIZE(8, 0x01)		,
    //����һ�δ���4������
    HID_RI_REPORT_COUNT(8, 0x04),
    //ָ����������Ϊ�����������ݵ�ֵ��ʱ��仯
    HID_RI_INPUT(8, HID_IOF_VARIABLE),
    //�������ݵ��߼���СֵΪ0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //�������ݵ��߼����ֵΪ1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),

    //ָ��������Ϊ����
    HID_RI_USAGE(8, 0xb0),  /* Play */
    //ָ��������Ϊ��ͣ
    HID_RI_USAGE(8, 0xb1)		, /* Pause */
    //ָ����������Ϊ���
    HID_RI_USAGE(8, 0xb3)		, /* Fast Forward */
    //ָ����������Ϊ����
    HID_RI_USAGE(8, 0xb4)		, /* Rewind */

    //����ÿ������λ��Ϊ1
    HID_RI_REPORT_SIZE(8, 0x01)		,
    //����һ�δ���4������
    HID_RI_REPORT_COUNT(8, 0x04),
    //ָ����������Ϊ�����������ݵ�ֵ��ʱ��仯����û����ѡѡ��
    HID_RI_INPUT(8, HID_IOF_NO_PREFERRED_STATE | HID_IOF_VARIABLE),
    //�������ݵ��߼���СֵΪ0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //�������ݵ��߼����ֵΪ1
    HID_RI_LOGICAL_MAXIMUM(8, 0x01),

    //ָ����������Ϊ�������
    HID_RI_USAGE(8, 0xe9),
    //ָ����������Ϊ���ҹ���
    HID_RI_USAGE(8, 0xea),
    //ָ����������Ϊ���¹���
    HID_RI_USAGE(8, 0xe2),
    //����ÿ������λ��Ϊ1
    HID_RI_REPORT_SIZE(8, 0x01),
    //����һ�δ���3������
    HID_RI_REPORT_COUNT(8, 0x03),
    //ָ����������Ϊ�����������ݵ�ֵ��ʱ��仯����û����ѡѡ��
    HID_RI_INPUT(8, HID_IOF_NO_PREFERRED_STATE | HID_IOF_VARIABLE),

    //����ÿ������λ��Ϊ5
    HID_RI_REPORT_SIZE(8, 0x05),
    //����һ�δ���1������
    HID_RI_REPORT_COUNT(8, 0x01),
    //ָ����������Ϊ�����������ݵ�ֵ����
    HID_RI_INPUT(8, HID_IOF_CONSTANT),
    /**����HID���涨���������3�й����������ͺ�һ�����������ڿ���������***/
    HID_RI_END_COLLECTION(0),

    //ָ��ʹ�õ�HIDҳ��Ϊ�����Զ���ҳ��
	HID_RI_USAGE_PAGE(16, 0xffa0),
    //ָ��ʹ�õ�HID��������Ϊ��һ���Զ�������
    HID_RI_USAGE(8, 0x01),
    //����HID�����������ļ�������ΪӦ�ó���
    HID_RI_COLLECTION(8, 0x01),  /* Application */
    //����HID�������ID��Ϊ2
    HID_RI_REPORT_ID(8, 0x02)		, /*Report ID*/
    //ָ����������Ϊ������
    HID_RI_USAGE(8, 0x01)		,
    //�������ݵ��߼���СֵΪ0
    HID_RI_LOGICAL_MINIMUM(8, 0x00),
    //�����߼����ֵΪ255
	HID_RI_LOGICAL_MAXIMUM(16, 0x00ff),
    //����ÿ������λ��Ϊ8
    HID_RI_REPORT_SIZE(8, 0x08),
    //����һ�δ�����������
    HID_RI_REPORT_COUNT(8, 0x02),
    //ָ���������Ϊ���ݣ������ݽ������䵽����
    HID_RI_OUTPUT(8, HID_IOF_DATA),
    //ָ����������Ϊ����Ҽ�
    HID_RI_USAGE(8, 0x02),  /* mouse? */
    
    //����ÿ������λΪ8
    HID_RI_REPORT_SIZE(8, 0x08)		,
    //����һ�δ���2������
    HID_RI_REPORT_COUNT(8, 0x02),
    //ָ����������Ϊ���ݣ������ݽ����������䵽�豸
    HID_RI_INPUT(8, HID_IOF_DATA),
    HID_RI_END_COLLECTION(0),  };
//ʹ��HID����������������USB����ʶ��USB��Ƶ�豸֧�ֵ�HID��������
//�Ӷ�����ͨ��USB HIDЭ����п��ơ�
//����δ����У�ʹ����HID_RI_��ͷ�ĺ궨�������ٶ���HID�����
//���ֶ���дHID�����������ӷ�����׶���
#endif


//
//USB HID�����������Ķ���
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



/********************************************************************************************************
 * @file     usbmouse.c ，该程序主要针对鼠标
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
//该段代码定义了一个宏，用于判断是否开启鼠标平滑模式
//如果没有定义，则默认关闭平滑模式
#ifndef	USB_MOUSE_REPORT_SMOOTH
#define	USB_MOUSE_REPORT_SMOOTH	0
#endif

//该处宏定义鼠标数据缓冲区的长度为8
#define  USBMOUSE_BUFF_DATA_NUM    8
/// @brief 鼠标缓冲区
static mouse_data_t mouse_dat_buff[USBMOUSE_BUFF_DATA_NUM];
/// @brief 读写指针
static u8  usbmouse_wptr, usbmouse_rptr;
/// @brief USB数据缓冲区中未释放的数据数量
static u32 usbmouse_not_released;
/// @brief 表示USB鼠标数据报告时间，用于控制USB数据的发送频率
static u32 usbmouse_data_report_time;


/// @brief 用于将接收到的rf_packet_mouse_t类型的数据帧添加到USB鼠标数据缓冲区
/// @param packet_mouse 
void usbmouse_add_frame (rf_packet_mouse_t *packet_mouse){
	//根据pno 获得最新数据的个数
	u8 new_data_num = packet_mouse->pno;  
	//循环
	for(u8 i=0;i<new_data_num;i++)
	{		
			//利用memcpy4函数将最新数据加入缓冲区
			memcpy4((int*)(&mouse_dat_buff[usbmouse_wptr]), (int*)(&packet_mouse->data[i*sizeof(mouse_data_t)]), sizeof(mouse_data_t));
			//将写指针向前推进一个位置
			//该函数的作用是将指针值+1
			BOUND_INC_POW2(usbmouse_wptr,USBMOUSE_BUFF_DATA_NUM);
			//如果读写指针相等，书名缓冲区已满
			if(usbmouse_wptr == usbmouse_rptr)
			{
					//BOUND_INC_POW2(usbmouse_rptr,USBMOUSE_BUFF_DATA_NUM);
					break;
			}
	}
}

/// @brief 检查鼠标缓冲区中是否有未释放的数据
void usbmouse_release_check(){
	//usbmouse_not_released=0说明没有未释放的数据
	//clock_time_exceed(usbmouse_data_report_time, USB_MOUSE_RELEASE_TIMEOUT)
	//如果时间差超过了后一个参数，则说明需要发送报告
	if(usbmouse_not_released && clock_time_exceed(usbmouse_data_report_time, USB_MOUSE_RELEASE_TIMEOUT)){
	    //将释放按键赋值0
		u32 release_data = 0;
		//利用函数发送报告
	    if(usbmouse_hid_report(USB_HID_MOUSE, (u8*)(&release_data), MOUSE_REPORT_DATA_LEN)){
		    //为释放的按键数据为0
			usbmouse_not_released = 0;
	    }
	}
}

/// @brief 该函数用于从USB鼠标数据缓冲区读取数据，并将数据发送出去
void usbmouse_report_frame(){
//如果鼠标为平滑模式
#if 	USB_MOUSE_REPORT_SMOOTH
	static u32 tick = 0;
	//如果 USB端点正在忙碌中，则记录当前时间并返回，等待下一次调用
	if(usbhw_is_ep_busy(USB_EDP_MOUSE)) {
			tick = clock_time ();
	}
	//计算缓冲区中未读数据的个数
	u8 diff = (usbmouse_wptr - usbmouse_rptr) & (USBMOUSE_BUFF_DATA_NUM - 1);
	//如果未读取数据的个数小于3，
	//且距离上次调用的时间不超过5000个时钟周期，则返回，等待下一次调用
	if (diff < 3 && !clock_time_exceed (tick, 5000)) {
		return;
	}
#endif
	//如果读指针不等于写指针，说明与缓冲区中有未读取的数据
	if(usbmouse_wptr != usbmouse_rptr){
		//将缓冲区中最旧的数据读取出来，并且将其转换为u32类型的变量
        u32 data = *(u32*)(&mouse_dat_buff[usbmouse_rptr]);	// that is   >  0
        //这个函数用于向USB主机发送鼠标数据报告
		int ret = usbmouse_hid_report(USB_HID_MOUSE,(u8*)(&data), MOUSE_REPORT_DATA_LEN);
		if(ret){
			//将鼠标读指针向前推进一个位置
            BOUND_INC_POW2(usbmouse_rptr,USBMOUSE_BUFF_DATA_NUM);
		}
		if(0 == data && ret){			//  successfully  release the key
			//未释放的按键为0
			usbmouse_not_released = 0;
		}else{
			//有未释放的按键
			usbmouse_not_released = 1;
			//最后一次发送数据的时间
			usbmouse_data_report_time = clock_time();
		}
	}
	return;
}

/// @brief 该函数向主机用于发送鼠标按键数据的HID报告
/// @param report_id 表示按键的类型
/// @param data 表示按下的按键的地址
/// @param cnt 表示键值的长度
/// @return 
int usbmouse_hid_report(u8 report_id, u8 *data, int cnt){
	//unsigned char crc_in[8];
	//unsigned short crc;
	//unsigned int crch;
	//首先判断是否cnt<8
    assert(cnt<8);
	//如果USB鼠标端点在忙碌
	if(usbhw_is_ep_busy(USB_EDP_MOUSE)){

		//pData表示缓存区数据，sub_fifo表示缓存区
		u8 *pData = (u8 *)&usb_fifo[usb_ff_wptr++ & (USB_FIFO_NUM - 1)];
		pData[0] = DAT_TYPE_MOUSE;//鼠标类型
		pData[1] = report_id;//报告ID
		pData[2] = cnt;//数据长度
		//将数据data赋值到pData中，即将数据拷贝到缓存区中
		memcpy(pData + 4, data, cnt);
		//fifo_use表示缓冲区已经使用的空间大小
		int fifo_use = (usb_ff_wptr - usb_ff_rptr) & (USB_FIFO_NUM*2-1);
		if (fifo_use > USB_FIFO_NUM) {
			usb_ff_rptr++;
			//fifo overflow, overlap older data
		}

		return 0;//发送失败
	}
	//用于设置USB的端点指针。
	//由于发送数据需要从端点的缓冲区中读取数据，因此需要先设置指针为0
	reg_usb_ep_ptr(USB_EDP_MOUSE) = 0;

	// please refer to usbmouse_i.h mouse_report_desc
	//这个变量表示USB鼠标的报告协议类型，
	extern u8 usb_mouse_report_proto;
	//如果为0，则使用鼠标标准报告协议
	if (!usb_mouse_report_proto) {
		//向端点缓存区中写入数据
		//依次发送3个字节的数据
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[0];
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[1];
		reg_usb_ep_dat(USB_EDP_MOUSE) = data[2];
	}
	else //否则使用鼠标扩展报告协议
	{	
		//先想缓存区中写入报告ID
		reg_usb_ep_dat(USB_EDP_MOUSE) = report_id;
		//向端点缓存区中写入数据
		foreach(i, cnt){
			reg_usb_ep_dat(USB_EDP_MOUSE) = data[i];
		}
	}
//	reg_usb_ep_ctrl(USB_EDP_MOUSE) = FLD_EP_DAT_ACK;		// ACK
	//该函数用于设置USB鼠标的端点控制寄存器
	//控制寄存器有多个位，可以用来控制端点的工作状态
	//比如是否需要发送数据、数据发送完毕后是否需要确认
	reg_usb_ep_ctrl(USB_EDP_MOUSE) = FLD_EP_DAT_ACK | (edp_toggle[USB_EDP_MOUSE] ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
	//FLD_EP_DAT_ACK表示数据发送完毕后需要发送ACK(确认信号)，
	//以告知USB主句已经成功接收到数据

	//FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0分别表示传输状态为0和1
	//每次传输后，需要将数据传输状态取反，以便下次传输使用不同的数据传输状态

	//该变量是一个标志位，用于记录当前数据包的传输状态。
	//USB数据传输时，每转出一个数据包就需要切换一次数据传输状态
	//以确保数据传输的正确性
	edp_toggle[USB_EDP_MOUSE] ^= 1;

	return 1;//发送完成
}


void usbmouse_init(){
}

#endif

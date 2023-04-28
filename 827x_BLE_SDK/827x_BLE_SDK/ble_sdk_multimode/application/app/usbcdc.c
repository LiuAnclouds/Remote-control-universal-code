/********************************************************************************************************
 * @file     usbcdc.c ,该程序主要针对CDC协议
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

//或许是你需要的知识:
//CDC一种USB设备类别，用于定义USB设备和主机之间的通信协议，
//例如USB转串口线或者USB串口转换器等设备

//ACK是计算机网络中的一种确认值信号，表示数据包已经成功接收

#if(USB_CDC_ENABLE)//一个宏定义
//USB CDC设备类的头文件
#include "usbcdc.h"
//USB硬件抽象层的头文件
#include "../drivers/usbhw.h"
//USB硬件抽象层的内部头文件
#include "../drivers/usbhw_i.h"
//USB协议的头文件
#include "../drivers/usb.h"
//事件驱动机制的头文件
#include "../os/ev.h"
//#include "../os/sys.h"
//事件队列的头文件
#include "../os/ev_queue.h"
//事件缓冲区的头文件
#include "../os/ev_buffer.h"



/// @brief 
/// @param value 
//写入一个32位的数据到USB CDC设备的输出缓冲区中
void usbcdc_write32(u32 value);
/// @brief 
/// @param value 
//从USB CDC设备的输入缓冲区中读取一个32位的数据
void usbcdc_read32(u32* value);
/// @brief 
/// @param arg 
/// @return 
//设备接收数据超时的回调函数
int usbcdc_recvTimeoutCb(void* arg);


/// @brief 
//定义了一个名位cdc_ctrl_t的结构体
//用语存储USB CDC设备的控制信息，
//包括输入缓冲区、输出缓冲区、定时器、回调函数等
typedef struct {
	//指向CDC设备接收数据缓冲区的指针
	//接收数据存储到缓存区中
    u8 *rxBuf;
	//指向CDC设备发送数据缓冲区的指针
	//读取缓存区数据并发送到主机
	u8 *txBuf;

	/* Following variables are used in the RX more than CDC_TXRX_EPSIZE */
	//用于计时的事件结构体，用于定时检测是否接收到完整的数据包
	ev_time_event_t timer;
	//用于记录接收缓冲区中已经接收到的数据长度的变量
	//每次接收到数据时，都会将数据存储到接收缓冲区中，并更新lastindex变量的值
	u8  lastIndex;

	//记录需要发送的数据长度的变量，表示在发送数据时需要发送的数据长度。
	//如果发送的数据长度小鱼CDC_TXRX_EPSIZE，
	//则lenToSend的值为实际需要发送的数据长度
	//反之，其值为CDC_TXRX_EPSIZE
	u16 lenToSend;
	//用于记录已经发送的数据长度的变量，每次发送数据时，
	//都会更新lastSendIndex的值，表示已经发送的数据长度。
	u16 lastSendIndex;
	
	//CDC设备接收数据时的回调函数指针，用于在接收到数据时进行处理。
	cdc_handlerFn_t rxCb;
	//CDC设备接收数据时的回调函数指针，用于在发送完成后进行处理。
    cdc_handlerFn_t txCb;
	
} cdc_ctrl_t;

//静态编译
#ifdef STATIC_V_INST
//当静态编译时，用cdc_vs来代替cdc_v
cdc_ctrl_t cdc_vs;
#endif
//定义了一个指向cdc_ctrl_t类型的指针
//用于指向USB CDC设备的控制信息
cdc_ctrl_t *cdc_v;





/// @brief 
//定义了一个结构体变量
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
	//.Config =
	//config存储CDC设备的配置信息
	//包括控制接口号、数据输入端点号和大小
	//数据输出端点号、通知端点号和大小等
    {	
		//控制接口的编号
	    0, //ControlInterfaceNumber
		//数据输入端点的编号，即数据从主机流向设备的端点号，通常为CDC_TX_EPNUM
		CDC_TX_EPNUM, // DataINEndpointNumber
		//数据输入端点的大小，即每次可以传输的数据大小，通常为CDC_TXRX_EPSIZE
		CDC_TXRX_EPSIZE, // DataINEndpointSize
		//输入输入端点是否使用双缓冲区，通常为false
		false, // DataINEndpointDoubleBank

		//数据输出端点的编号，即数据从主机流向设备的端点号，通常为CDC_RX_EPNUM
		CDC_RX_EPNUM,  // DataOUTEndpointNumber
		//数据输出端点的大小，即每次可以传输的数据大小，通常为CDC_TXRX_EPSIZE
		CDC_TXRX_EPSIZE, // DataOUTEndpointSize
		//输入输入端点是否使用双缓冲区，通常为false
		false, // DataOUTEndpointDoubleBank

		//通知端点的编号，用于向主机发送通知信息，通常为CDC_NOTIFICATION_EPNUM
		CDC_NOTIFICATION_EPNUM, // NotificationEndpointNumber
		//通知端点的大小，即每次可以传输的数据大小，通常为CDC_NOTIFICATION_EPSIZE
		CDC_NOTIFICATION_EPSIZE, // NotificationEndpointSize
		//通知端点是否使用双缓冲区，通常为false
		false,  // NotificationEndpointDoubleBank
	},
};
//指向VirtualSerial_CDC_Interface的指针，用语指向CDC设备的信息结构体
USB_ClassInfo_CDC_Device_t *CDCInterfaceInfo = &VirtualSerial_CDC_Interface; 




/// @brief 
/// @param bRequest //表示请求类型码
/// @param wValue //表示请求参数
/// @param wIndex //表示请求目标
/// @param wLength//表示请求数据长度
//该函数是USB CDC设备接受到控制请求时的处理函数
void CDC_Device_ProcessControlRequest(u8 bRequest, u16 wValue, u16 wIndex, u16 wLength)
{	
	//判断请求目标是否为控制接口，如过不是则直接返回
	if (wIndex != CDCInterfaceInfo->Config.ControlInterfaceNumber)
	  return;
	//根据请求类型码进行不同的处理
	switch (bRequest)
	{	
		//请求获取CDC设备的波特率、字符格式、奇偶校验和数据位数等信息。
		//在处理时CDC设备将这些信息写入输出缓冲区中，以方便主机读取
		case CDC_REQ_GetLineEncoding:
			//将CDC设备的波特率写入输出缓冲区
            usbcdc_write32(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
			//将CDC设备的字符格式写入输出缓冲区
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.CharFormat);
            //将CDC设备的奇偶校验写入输出缓冲区
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.ParityType);
            //将CDC设备的数据位数写入输出缓冲区
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.DataBits);
			break;
		//请求设置CDC设备的波特率、字符格式、奇偶校验和数据位数等信息。	
		//在处理时CDC设备将从缓冲区读取这些信息，并更新自身的状态信息
		case CDC_REQ_SetLineEncoding:
			//将输入缓冲区中的波特率读入CDC设备的状态信息中
			usbcdc_read32(&CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
			//将输入缓冲区中的字符格式读入CDC设备的状态信息中
			CDCInterfaceInfo->State.LineEncoding.CharFormat  = usbhw_read_ctrl_ep_data();
			//将输入缓冲区中的奇偶校验读入CDC设备的状态信息中
			CDCInterfaceInfo->State.LineEncoding.ParityType  = usbhw_read_ctrl_ep_data();
			//将输入缓冲区中的数据位数读入CDC设备的状态信息中
			CDCInterfaceInfo->State.LineEncoding.DataBits    = usbhw_read_ctrl_ep_data();

			//EVENT_CDC_Device_LineEncodingChanged(CDCInterfaceInfo);
			break;
		//该请求是设置CDC设备的控制信号，例如DTR、RTS等	
		//在该请求处理函数中,CDC设备将主机发送的控制信号保存在自身的状态信息中
		case CDC_REQ_SetControlLineState:
			//将主机发送的控制信号保存在CDC设备的状态信息中
			CDCInterfaceInfo->State.ControlLineStates.HostToDevice = wValue;
			//EVENT_CDC_Device_ControLineStateChanged(CDCInterfaceInfo);
			break;
		//该请求是发送一个断线信号。
		//在该请求处理函数中,CDC设备暂时没有实现处理代码	
		case CDC_REQ_SendBreak:

			break;
	}
}
/// @brief 
/// @param value 
///该段代码实现了对32位无符号整形数据value的写操作，用于处理CDC设备的波特率信息
void usbcdc_write32(u32 value)
{	//具体做法是将valu拆分为四个8位字节，分别写入输出缓冲区

	//将value的低8位写入输出缓冲区
	usbhw_write_ctrl_ep_data(value&0xff);
	//将其他三个字节一次写入输出缓冲区
	usbhw_write_ctrl_ep_data((value>>8)&0xff);
	usbhw_write_ctrl_ep_data((value>>16)&0xff);
	usbhw_write_ctrl_ep_data((value>>24)&0xff);
}

/// @brief 
/// @param value 
///该段代码是从USB控制端点的输入缓冲区读取一个32位无符号整型数据，
///并将其存储到指定的内存地址中
void usbcdc_read32(u32* value)
{	
	//32位存储数据
	u32 temp = 0;
	//从输入缓冲区中读取第一个字节，并将其存储到value中
	*value = usbhw_read_ctrl_ep_data();
	//从输入缓冲区中读取第二个字节，并存储到temp中
	temp =  usbhw_read_ctrl_ep_data();
	//将temp的值左移八位后与value做或运算，将第二个字节的值存储到value的低16位中
	//这里把temp和value合到了一起，形成了2个字节
	*value = (temp << 8) | (*value); 
	temp = 0;

	//从输入缓冲区读取第三个字节，并将其存储到temp中
	temp =  usbhw_read_ctrl_ep_data();
	//将temp左移16位后与value做或运算
	//这里是把3个字节合到一起
	*value = (temp << 16) | (*value); 
	temp = 0;
	////从输入缓冲区读取第四个字节，并将其存储到temp中
	//这里是把4个字节合到一起
	temp =  usbhw_read_ctrl_ep_data();
	*value = (temp << 24) | (*value); 
	
}


/// @brief 
/// @param  
///USB CDC设备的初始化函数
void usbcdc_init(void)
{
	/* Init UART parameters */
	//波特率
	CDCInterfaceInfo->State.LineEncoding.BaudRateBPS = 115200;
	//字符格式
	CDCInterfaceInfo->State.LineEncoding.CharFormat = 0;
	//奇偶校验
	CDCInterfaceInfo->State.LineEncoding.ParityType = 0;
	//数据位数
	CDCInterfaceInfo->State.LineEncoding.DataBits = 8;
	
	//初始化了结构体变量cdc_v，并把cdc_vs赋值为初值
	cdc_v = &cdc_vs;
	//lastindex用于记录接收缓冲区中已经接收到的数据长度的变量
	//初始化为0表示接收缓冲区目前没有数据被接收
    cdc_v->lastIndex = 0;
	//timer计时结构体，在CDC设备接收数据时，会启动一个计时器
	//如果在规定时间内没有接收到完整的数据包，则会调用usbcdc_recvTimeoutCb进行处理
    cdc_v->timer.cb = usbcdc_recvTimeoutCb;
}

/// @brief 
/// @param buf 
///该函数的作用是设置接收缓冲区。
void usbcdc_setRxBuf(u8 *buf)
{	
	///具体实现是将传入的buf指针赋值给cdc_v结构体变量的rxBuf成员变量
	///以便在CDC设备接收到数据时。将数据存储到指定的接收缓冲区中。
	cdc_v->rxBuf = buf;
}

/// @brief 
/// @param rxFunc 
/// @param txCb 
///该函数用于设置CDC设备接收和发送数据时的回调函数
void usbcdc_setCB(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb)
{	
	//将传入的rxFunc函数指针赋值给cdc_v结构体变量的rxCb成员变量
	//以便在CDC设备接收到数据时，调用rxFunc函数进行处理
	cdc_v->rxCb = rxFunc;
	//将传入的rxCb函数指针赋值给cdc_v结构体变量的rxCb成员变量
	//以便在CDC设备发送数据完成时，调用rxFunc函数进行处理
    cdc_v->txCb = txCb;
}

/// @brief 
/// @param arg 
/// @return 
///该函数用于处理CDC设备接收数据超时的情况
//该函数需要在CDC设备接收数据时启动计时器，如果在规定时间内没有接收到完整数据包
//则会调用该函数进行处理
int usbcdc_recvTimeoutCb(void* arg)
{	
	//8位指针
	u8* p;
	//首先将CDC设备接收数据时已经接收到的数据长度lastIndex置为0，
	//表示重新开始接收数据，表示重新开始接收数据。
	cdc_v->lastIndex = 0;

	/* Clear the buffer */
	//清除缓存
	//然后将CDC设备接收数据时的缓冲区rxBuf的指针赋值为NULL，
	//表示清空缓冲区，准备重新接收数据。
	p = cdc_v->rxBuf;
	cdc_v->rxBuf = NULL;

	/* Callback */
	//回调
	//最后调用rxCb,将接收到的数据缓冲区p作为参数传入回调函数中进行处理
	if (cdc_v->rxCb) {
		cdc_v->rxCb(p);
	}
	//表示不再需要继续计时，结束计时工作
	return -1;
}


/// @brief 
/// @param
///该函数用于处理CDC设备接收到数据后的操作  
void usbcdc_recvData(void)
{
	u8 i;
	u8 *p;
	u8 len;
	u8 fEnd = 0;
	
	/* No buffer */
	//首先判断CDC设备接收数据时的缓冲区rxBud是否为空
	//如果为空，则进入死循环，等待缓冲区被分配。
	if (!cdc_v->rxBuf) {
		while(1);
	}
	//如果CDC设备接收数据时的计时器未超时，则取消计时器的工作
	if (!is_timer_expired(&cdc_v->timer)) {
		ev_unon_timer(&cdc_v->timer);
	}

	//通过reg_usb_ep_ptr函数获取CDC设备接收数据时的缓冲区中数据的长度len
	//并判断是否为CDC_TXRX_EPSIZE（表示数据长度已经达到缓冲区的最大容量）
	//如果是则表示已经接收到完整的数据包，
	//否则表示数据尚未完成接收，需要继续等待数据的接收
	len = reg_usb_ep_ptr(CDC_RX_EPNUM & 0x07);
	fEnd = (len == CDC_TXRX_EPSIZE) ? 0 : 1;
	//调用该函数重置CDC设备接收数据时的缓冲区指针，准备接收下一段数据
	usbhw_reset_ep_ptr(CDC_RX_EPNUM);

	//通过循环将CDC设备接收数据时的缓冲区中的数据一个一个读取出来，
	//并存储到rxBuf缓冲区中，同时更新lastIndex变量的值，表示已经接收到的数据长度，
	for (i = 0; i < len; i++) {
		cdc_v->rxBuf[cdc_v->lastIndex++] = usbhw_read_ep_data(CDC_RX_EPNUM);
	}

	//如果接收到的数据长度已经达到缓冲区的最大容量
	//则表示已经接收到完整的数据包，需要对数据进行处理
	if (fEnd) {
		//此时，将lastIndex置为0，表示重新开始接收数据

		cdc_v->lastIndex = 0;

		/* Clear the buffer */
		//将rxBuf缓冲区指针赋值NULL，清空缓存，准备重新接收数据
		p = cdc_v->rxBuf;
		cdc_v->rxBuf = NULL;

		/* Callback */
		//回调
		//调用CDC设备接收数据时的回调函数指针rxCb
		//将接收到的数据缓冲区p作为计时器，等待剩余数据的接收。
		if (cdc_v->rxCb) {
			cdc_v->rxCb(p);
		}
	} else {
		//启动计时器
		ev_on_timer(&cdc_v->timer, 500);
	}
}

//8位数组
u8 T_BUF[60];
//32位数据
u32 T_CNT;
/// @brief 
/// @param  
/// @return 
///该函数用于向USB主机发送CDC设备的数据
u8 usbcdc_sendBulkData(void)
{
	u16 len;
	
	/* Wait until not busy */
	//首先判断CDC设备发送数据时的缓冲区是否忙碌，如果忙碌，则返回0
	//等待下一次中断再次调用该函数
	if (usbhw_is_ep_busy(CDC_TX_EPNUM)) {
        /* Return to wait IRQ come again */
        return 0;
    }

    /* Get the length to send in this bulk transaction */
	//通过判断cdc_v->lenToSend与CDC_TXRX_EPSIZE的大小关系
	//来确定本次要发送的数据长度。
	len = (cdc_v->lenToSend > CDC_TXRX_EPSIZE) ? CDC_TXRX_EPSIZE : cdc_v->lenToSend;
	cdc_v->lenToSend -= len;

	//如果要发送的长度为0，则返回0
	if (len == 0) {
		return 0;
	}
	
	//调用函数将CDC_TX_EPNUM端点的指针清零，准备写入数据
    reg_usb_ep_ptr(CDC_TX_EPNUM) = 0;

	/* Write data to USB fifo */
	//通过循环将CDC设备发送数据时的缓冲区中的数据一个一个读取出来，
	//并存储到T_BUF缓冲区中，同时写入USB FIFO中，
	//更新lastIndex变量的值，表示已经发送的数据长度
	foreach (i, len) {
    	T_BUF[i] = cdc_v->txBuf[cdc_v->lastSendIndex];
        reg_usb_ep_dat(CDC_TX_EPNUM) = cdc_v->txBuf[cdc_v->lastSendIndex++];
    }

	/* Write ACK */
	//调用函数发送ACK确认信号，表示数据已经发送完成。
	//然后通过循环等待USB端点不再忙碌，如果等待时间超过10000次循环
	//则认为发送失败，清除掉USB端点的bit(0)
    reg_usb_ep_ctrl(CDC_TX_EPNUM) = FLD_EP_DAT_ACK;        // ACK
    u16 t = 0;
    while(usbhw_is_ep_busy(CDC_TX_EPNUM)) {
        if (t++ > 10000) {
            T_CNT++;
            reg_usb_ep_ctrl(CDC_TX_EPNUM) &= 0xfe; // clear bit(0)
        }
    };

    /* TX transaction finish */
	//如果CDC设备数据发送完成，则进行一些清理工作，包括
	//以下值的清零，
	if (cdc_v->lenToSend == 0) {
		//已经发送的数据长度
		cdc_v->lenToSend = 0;
		//接收到的数据长度
		cdc_v->lastSendIndex = 0;
		//该代码片段在数据发送完成后进行一些清理工作
		//调用cdc_v->txCb（回调函数）函数表示数据发送完成，等待下一次数据发送
        if (cdc_v->txCb) {
			//将cdc_v->txBuf清空，表示数据发送完成，等待下一次数据发送。
            EV_SCHEDULE_TASK(cdc_v->txCb, cdc_v->txBuf);
			
        }

		cdc_v->txBuf = NULL;
	}

	return len;
}


/// @brief 
/// @param buf 缓冲区指针
/// @param len 数据长度
/// @return 
///该函数用于向USB主机发送CDC设备的数据
usbcdc_sts_t usbcdc_sendData(u8 *buf, u8 len)
{	
	//首先判断CDC设备发送数据时的缓冲区是否忙碌
	if (cdc_v->txBuf) {
		//如果忙碌，则返回USB_BUSY，表示上一次数据还未发送完成
		//需要等待上一次数据发送完成后再次发送数据。
		return USB_BUSY;
	}

	/* Init the bulk transfer */
	//初始化CDC设备发送数据时的缓冲区
	//包括设置需要发送的数据长度len
    cdc_v->lenToSend = len;
	//发送缓冲区指针buf
	cdc_v->txBuf = buf;
	//已经已经发送的数据长度
	cdc_v->lastSendIndex = 0;

    /* Send first bulk */
	//调用函数发送第一批函数
	usbcdc_sendBulkData();
	//通过函数发送ACK确认信号，表示已经准备好接收USB主机发送的数据
	usbhw_data_ep_ack(USB_EDP_CDC_OUT);
	
	//返回成功
	return SUCCESS;
	
   
}


/// @brief 
/// @param  
/// @return 
///该函数用于判断CDC设备是否可以发送数据
u8 usbcdc_isAvailable(void)
{
	return (cdc_v->txBuf == NULL);
}


#endif  /* USB_CDC_ENABLE */

/********************************************************************************************************
 * @file     usbcdc.c ,�ó�����Ҫ���CDCЭ��
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

//����������Ҫ��֪ʶ:
//CDCһ��USB�豸������ڶ���USB�豸������֮���ͨ��Э�飬
//����USBת�����߻���USB����ת�������豸

//ACK�Ǽ���������е�һ��ȷ��ֵ�źţ���ʾ���ݰ��Ѿ��ɹ�����

#if(USB_CDC_ENABLE)//һ���궨��
//USB CDC�豸���ͷ�ļ�
#include "usbcdc.h"
//USBӲ��������ͷ�ļ�
#include "../drivers/usbhw.h"
//USBӲ���������ڲ�ͷ�ļ�
#include "../drivers/usbhw_i.h"
//USBЭ���ͷ�ļ�
#include "../drivers/usb.h"
//�¼��������Ƶ�ͷ�ļ�
#include "../os/ev.h"
//#include "../os/sys.h"
//�¼����е�ͷ�ļ�
#include "../os/ev_queue.h"
//�¼���������ͷ�ļ�
#include "../os/ev_buffer.h"



/// @brief 
/// @param value 
//д��һ��32λ�����ݵ�USB CDC�豸�������������
void usbcdc_write32(u32 value);
/// @brief 
/// @param value 
//��USB CDC�豸�����뻺�����ж�ȡһ��32λ������
void usbcdc_read32(u32* value);
/// @brief 
/// @param arg 
/// @return 
//�豸�������ݳ�ʱ�Ļص�����
int usbcdc_recvTimeoutCb(void* arg);


/// @brief 
//������һ����λcdc_ctrl_t�Ľṹ��
//����洢USB CDC�豸�Ŀ�����Ϣ��
//�������뻺�������������������ʱ�����ص�������
typedef struct {
	//ָ��CDC�豸�������ݻ�������ָ��
	//�������ݴ洢����������
    u8 *rxBuf;
	//ָ��CDC�豸�������ݻ�������ָ��
	//��ȡ���������ݲ����͵�����
	u8 *txBuf;

	/* Following variables are used in the RX more than CDC_TXRX_EPSIZE */
	//���ڼ�ʱ���¼��ṹ�壬���ڶ�ʱ����Ƿ���յ����������ݰ�
	ev_time_event_t timer;
	//���ڼ�¼���ջ��������Ѿ����յ������ݳ��ȵı���
	//ÿ�ν��յ�����ʱ�����Ὣ���ݴ洢�����ջ������У�������lastindex������ֵ
	u8  lastIndex;

	//��¼��Ҫ���͵����ݳ��ȵı�������ʾ�ڷ�������ʱ��Ҫ���͵����ݳ��ȡ�
	//������͵����ݳ���С��CDC_TXRX_EPSIZE��
	//��lenToSend��ֵΪʵ����Ҫ���͵����ݳ���
	//��֮����ֵΪCDC_TXRX_EPSIZE
	u16 lenToSend;
	//���ڼ�¼�Ѿ����͵����ݳ��ȵı�����ÿ�η�������ʱ��
	//�������lastSendIndex��ֵ����ʾ�Ѿ����͵����ݳ��ȡ�
	u16 lastSendIndex;
	
	//CDC�豸��������ʱ�Ļص�����ָ�룬�����ڽ��յ�����ʱ���д���
	cdc_handlerFn_t rxCb;
	//CDC�豸��������ʱ�Ļص�����ָ�룬�����ڷ�����ɺ���д���
    cdc_handlerFn_t txCb;
	
} cdc_ctrl_t;

//��̬����
#ifdef STATIC_V_INST
//����̬����ʱ����cdc_vs������cdc_v
cdc_ctrl_t cdc_vs;
#endif
//������һ��ָ��cdc_ctrl_t���͵�ָ��
//����ָ��USB CDC�豸�Ŀ�����Ϣ
cdc_ctrl_t *cdc_v;





/// @brief 
//������һ���ṹ�����
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
	//.Config =
	//config�洢CDC�豸��������Ϣ
	//�������ƽӿںš���������˵�źʹ�С
	//��������˵�š�֪ͨ�˵�źʹ�С��
    {	
		//���ƽӿڵı��
	    0, //ControlInterfaceNumber
		//��������˵�ı�ţ������ݴ����������豸�Ķ˵�ţ�ͨ��ΪCDC_TX_EPNUM
		CDC_TX_EPNUM, // DataINEndpointNumber
		//��������˵�Ĵ�С����ÿ�ο��Դ�������ݴ�С��ͨ��ΪCDC_TXRX_EPSIZE
		CDC_TXRX_EPSIZE, // DataINEndpointSize
		//��������˵��Ƿ�ʹ��˫��������ͨ��Ϊfalse
		false, // DataINEndpointDoubleBank

		//��������˵�ı�ţ������ݴ����������豸�Ķ˵�ţ�ͨ��ΪCDC_RX_EPNUM
		CDC_RX_EPNUM,  // DataOUTEndpointNumber
		//��������˵�Ĵ�С����ÿ�ο��Դ�������ݴ�С��ͨ��ΪCDC_TXRX_EPSIZE
		CDC_TXRX_EPSIZE, // DataOUTEndpointSize
		//��������˵��Ƿ�ʹ��˫��������ͨ��Ϊfalse
		false, // DataOUTEndpointDoubleBank

		//֪ͨ�˵�ı�ţ���������������֪ͨ��Ϣ��ͨ��ΪCDC_NOTIFICATION_EPNUM
		CDC_NOTIFICATION_EPNUM, // NotificationEndpointNumber
		//֪ͨ�˵�Ĵ�С����ÿ�ο��Դ�������ݴ�С��ͨ��ΪCDC_NOTIFICATION_EPSIZE
		CDC_NOTIFICATION_EPSIZE, // NotificationEndpointSize
		//֪ͨ�˵��Ƿ�ʹ��˫��������ͨ��Ϊfalse
		false,  // NotificationEndpointDoubleBank
	},
};
//ָ��VirtualSerial_CDC_Interface��ָ�룬����ָ��CDC�豸����Ϣ�ṹ��
USB_ClassInfo_CDC_Device_t *CDCInterfaceInfo = &VirtualSerial_CDC_Interface; 




/// @brief 
/// @param bRequest //��ʾ����������
/// @param wValue //��ʾ�������
/// @param wIndex //��ʾ����Ŀ��
/// @param wLength//��ʾ�������ݳ���
//�ú�����USB CDC�豸���ܵ���������ʱ�Ĵ�����
void CDC_Device_ProcessControlRequest(u8 bRequest, u16 wValue, u16 wIndex, u16 wLength)
{	
	//�ж�����Ŀ���Ƿ�Ϊ���ƽӿڣ����������ֱ�ӷ���
	if (wIndex != CDCInterfaceInfo->Config.ControlInterfaceNumber)
	  return;
	//����������������в�ͬ�Ĵ���
	switch (bRequest)
	{	
		//�����ȡCDC�豸�Ĳ����ʡ��ַ���ʽ����żУ�������λ������Ϣ��
		//�ڴ���ʱCDC�豸����Щ��Ϣд������������У��Է���������ȡ
		case CDC_REQ_GetLineEncoding:
			//��CDC�豸�Ĳ�����д�����������
            usbcdc_write32(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
			//��CDC�豸���ַ���ʽд�����������
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.CharFormat);
            //��CDC�豸����żУ��д�����������
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.ParityType);
            //��CDC�豸������λ��д�����������
			usbhw_write_ctrl_ep_data(CDCInterfaceInfo->State.LineEncoding.DataBits);
			break;
		//��������CDC�豸�Ĳ����ʡ��ַ���ʽ����żУ�������λ������Ϣ��	
		//�ڴ���ʱCDC�豸���ӻ�������ȡ��Щ��Ϣ�������������״̬��Ϣ
		case CDC_REQ_SetLineEncoding:
			//�����뻺�����еĲ����ʶ���CDC�豸��״̬��Ϣ��
			usbcdc_read32(&CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);
			//�����뻺�����е��ַ���ʽ����CDC�豸��״̬��Ϣ��
			CDCInterfaceInfo->State.LineEncoding.CharFormat  = usbhw_read_ctrl_ep_data();
			//�����뻺�����е���żУ�����CDC�豸��״̬��Ϣ��
			CDCInterfaceInfo->State.LineEncoding.ParityType  = usbhw_read_ctrl_ep_data();
			//�����뻺�����е�����λ������CDC�豸��״̬��Ϣ��
			CDCInterfaceInfo->State.LineEncoding.DataBits    = usbhw_read_ctrl_ep_data();

			//EVENT_CDC_Device_LineEncodingChanged(CDCInterfaceInfo);
			break;
		//������������CDC�豸�Ŀ����źţ�����DTR��RTS��	
		//�ڸ�����������,CDC�豸���������͵Ŀ����źű����������״̬��Ϣ��
		case CDC_REQ_SetControlLineState:
			//���������͵Ŀ����źű�����CDC�豸��״̬��Ϣ��
			CDCInterfaceInfo->State.ControlLineStates.HostToDevice = wValue;
			//EVENT_CDC_Device_ControLineStateChanged(CDCInterfaceInfo);
			break;
		//�������Ƿ���һ�������źš�
		//�ڸ�����������,CDC�豸��ʱû��ʵ�ִ������	
		case CDC_REQ_SendBreak:

			break;
	}
}
/// @brief 
/// @param value 
///�öδ���ʵ���˶�32λ�޷�����������value��д���������ڴ���CDC�豸�Ĳ�������Ϣ
void usbcdc_write32(u32 value)
{	//���������ǽ�valu���Ϊ�ĸ�8λ�ֽڣ��ֱ�д�����������

	//��value�ĵ�8λд�����������
	usbhw_write_ctrl_ep_data(value&0xff);
	//�����������ֽ�һ��д�����������
	usbhw_write_ctrl_ep_data((value>>8)&0xff);
	usbhw_write_ctrl_ep_data((value>>16)&0xff);
	usbhw_write_ctrl_ep_data((value>>24)&0xff);
}

/// @brief 
/// @param value 
///�öδ����Ǵ�USB���ƶ˵�����뻺������ȡһ��32λ�޷����������ݣ�
///������洢��ָ�����ڴ��ַ��
void usbcdc_read32(u32* value)
{	
	//32λ�洢����
	u32 temp = 0;
	//�����뻺�����ж�ȡ��һ���ֽڣ�������洢��value��
	*value = usbhw_read_ctrl_ep_data();
	//�����뻺�����ж�ȡ�ڶ����ֽڣ����洢��temp��
	temp =  usbhw_read_ctrl_ep_data();
	//��temp��ֵ���ư�λ����value�������㣬���ڶ����ֽڵ�ֵ�洢��value�ĵ�16λ��
	//�����temp��value�ϵ���һ���γ���2���ֽ�
	*value = (temp << 8) | (*value); 
	temp = 0;

	//�����뻺������ȡ�������ֽڣ�������洢��temp��
	temp =  usbhw_read_ctrl_ep_data();
	//��temp����16λ����value��������
	//�����ǰ�3���ֽںϵ�һ��
	*value = (temp << 16) | (*value); 
	temp = 0;
	////�����뻺������ȡ���ĸ��ֽڣ�������洢��temp��
	//�����ǰ�4���ֽںϵ�һ��
	temp =  usbhw_read_ctrl_ep_data();
	*value = (temp << 24) | (*value); 
	
}


/// @brief 
/// @param  
///USB CDC�豸�ĳ�ʼ������
void usbcdc_init(void)
{
	/* Init UART parameters */
	//������
	CDCInterfaceInfo->State.LineEncoding.BaudRateBPS = 115200;
	//�ַ���ʽ
	CDCInterfaceInfo->State.LineEncoding.CharFormat = 0;
	//��żУ��
	CDCInterfaceInfo->State.LineEncoding.ParityType = 0;
	//����λ��
	CDCInterfaceInfo->State.LineEncoding.DataBits = 8;
	
	//��ʼ���˽ṹ�����cdc_v������cdc_vs��ֵΪ��ֵ
	cdc_v = &cdc_vs;
	//lastindex���ڼ�¼���ջ��������Ѿ����յ������ݳ��ȵı���
	//��ʼ��Ϊ0��ʾ���ջ�����Ŀǰû�����ݱ�����
    cdc_v->lastIndex = 0;
	//timer��ʱ�ṹ�壬��CDC�豸��������ʱ��������һ����ʱ��
	//����ڹ涨ʱ����û�н��յ����������ݰ���������usbcdc_recvTimeoutCb���д���
    cdc_v->timer.cb = usbcdc_recvTimeoutCb;
}

/// @brief 
/// @param buf 
///�ú��������������ý��ջ�������
void usbcdc_setRxBuf(u8 *buf)
{	
	///����ʵ���ǽ������bufָ�븳ֵ��cdc_v�ṹ�������rxBuf��Ա����
	///�Ա���CDC�豸���յ�����ʱ�������ݴ洢��ָ���Ľ��ջ������С�
	cdc_v->rxBuf = buf;
}

/// @brief 
/// @param rxFunc 
/// @param txCb 
///�ú�����������CDC�豸���պͷ�������ʱ�Ļص�����
void usbcdc_setCB(cdc_handlerFn_t rxFunc, cdc_handlerFn_t txCb)
{	
	//�������rxFunc����ָ�븳ֵ��cdc_v�ṹ�������rxCb��Ա����
	//�Ա���CDC�豸���յ�����ʱ������rxFunc�������д���
	cdc_v->rxCb = rxFunc;
	//�������rxCb����ָ�븳ֵ��cdc_v�ṹ�������rxCb��Ա����
	//�Ա���CDC�豸�����������ʱ������rxFunc�������д���
    cdc_v->txCb = txCb;
}

/// @brief 
/// @param arg 
/// @return 
///�ú������ڴ���CDC�豸�������ݳ�ʱ�����
//�ú�����Ҫ��CDC�豸��������ʱ������ʱ��������ڹ涨ʱ����û�н��յ��������ݰ�
//�����øú������д���
int usbcdc_recvTimeoutCb(void* arg)
{	
	//8λָ��
	u8* p;
	//���Ƚ�CDC�豸��������ʱ�Ѿ����յ������ݳ���lastIndex��Ϊ0��
	//��ʾ���¿�ʼ�������ݣ���ʾ���¿�ʼ�������ݡ�
	cdc_v->lastIndex = 0;

	/* Clear the buffer */
	//�������
	//Ȼ��CDC�豸��������ʱ�Ļ�����rxBuf��ָ�븳ֵΪNULL��
	//��ʾ��ջ�������׼�����½������ݡ�
	p = cdc_v->rxBuf;
	cdc_v->rxBuf = NULL;

	/* Callback */
	//�ص�
	//������rxCb,�����յ������ݻ�����p��Ϊ��������ص������н��д���
	if (cdc_v->rxCb) {
		cdc_v->rxCb(p);
	}
	//��ʾ������Ҫ������ʱ��������ʱ����
	return -1;
}


/// @brief 
/// @param
///�ú������ڴ���CDC�豸���յ����ݺ�Ĳ���  
void usbcdc_recvData(void)
{
	u8 i;
	u8 *p;
	u8 len;
	u8 fEnd = 0;
	
	/* No buffer */
	//�����ж�CDC�豸��������ʱ�Ļ�����rxBud�Ƿ�Ϊ��
	//���Ϊ�գ��������ѭ�����ȴ������������䡣
	if (!cdc_v->rxBuf) {
		while(1);
	}
	//���CDC�豸��������ʱ�ļ�ʱ��δ��ʱ����ȡ����ʱ���Ĺ���
	if (!is_timer_expired(&cdc_v->timer)) {
		ev_unon_timer(&cdc_v->timer);
	}

	//ͨ��reg_usb_ep_ptr������ȡCDC�豸��������ʱ�Ļ����������ݵĳ���len
	//���ж��Ƿ�ΪCDC_TXRX_EPSIZE����ʾ���ݳ����Ѿ��ﵽ�����������������
	//��������ʾ�Ѿ����յ����������ݰ���
	//�����ʾ������δ��ɽ��գ���Ҫ�����ȴ����ݵĽ���
	len = reg_usb_ep_ptr(CDC_RX_EPNUM & 0x07);
	fEnd = (len == CDC_TXRX_EPSIZE) ? 0 : 1;
	//���øú�������CDC�豸��������ʱ�Ļ�����ָ�룬׼��������һ������
	usbhw_reset_ep_ptr(CDC_RX_EPNUM);

	//ͨ��ѭ����CDC�豸��������ʱ�Ļ������е�����һ��һ����ȡ������
	//���洢��rxBuf�������У�ͬʱ����lastIndex������ֵ����ʾ�Ѿ����յ������ݳ��ȣ�
	for (i = 0; i < len; i++) {
		cdc_v->rxBuf[cdc_v->lastIndex++] = usbhw_read_ep_data(CDC_RX_EPNUM);
	}

	//������յ������ݳ����Ѿ��ﵽ���������������
	//���ʾ�Ѿ����յ����������ݰ�����Ҫ�����ݽ��д���
	if (fEnd) {
		//��ʱ����lastIndex��Ϊ0����ʾ���¿�ʼ��������

		cdc_v->lastIndex = 0;

		/* Clear the buffer */
		//��rxBuf������ָ�븳ֵNULL����ջ��棬׼�����½�������
		p = cdc_v->rxBuf;
		cdc_v->rxBuf = NULL;

		/* Callback */
		//�ص�
		//����CDC�豸��������ʱ�Ļص�����ָ��rxCb
		//�����յ������ݻ�����p��Ϊ��ʱ�����ȴ�ʣ�����ݵĽ��ա�
		if (cdc_v->rxCb) {
			cdc_v->rxCb(p);
		}
	} else {
		//������ʱ��
		ev_on_timer(&cdc_v->timer, 500);
	}
}

//8λ����
u8 T_BUF[60];
//32λ����
u32 T_CNT;
/// @brief 
/// @param  
/// @return 
///�ú���������USB��������CDC�豸������
u8 usbcdc_sendBulkData(void)
{
	u16 len;
	
	/* Wait until not busy */
	//�����ж�CDC�豸��������ʱ�Ļ������Ƿ�æµ�����æµ���򷵻�0
	//�ȴ���һ���ж��ٴε��øú���
	if (usbhw_is_ep_busy(CDC_TX_EPNUM)) {
        /* Return to wait IRQ come again */
        return 0;
    }

    /* Get the length to send in this bulk transaction */
	//ͨ���ж�cdc_v->lenToSend��CDC_TXRX_EPSIZE�Ĵ�С��ϵ
	//��ȷ������Ҫ���͵����ݳ��ȡ�
	len = (cdc_v->lenToSend > CDC_TXRX_EPSIZE) ? CDC_TXRX_EPSIZE : cdc_v->lenToSend;
	cdc_v->lenToSend -= len;

	//���Ҫ���͵ĳ���Ϊ0���򷵻�0
	if (len == 0) {
		return 0;
	}
	
	//���ú�����CDC_TX_EPNUM�˵��ָ�����㣬׼��д������
    reg_usb_ep_ptr(CDC_TX_EPNUM) = 0;

	/* Write data to USB fifo */
	//ͨ��ѭ����CDC�豸��������ʱ�Ļ������е�����һ��һ����ȡ������
	//���洢��T_BUF�������У�ͬʱд��USB FIFO�У�
	//����lastIndex������ֵ����ʾ�Ѿ����͵����ݳ���
	foreach (i, len) {
    	T_BUF[i] = cdc_v->txBuf[cdc_v->lastSendIndex];
        reg_usb_ep_dat(CDC_TX_EPNUM) = cdc_v->txBuf[cdc_v->lastSendIndex++];
    }

	/* Write ACK */
	//���ú�������ACKȷ���źţ���ʾ�����Ѿ�������ɡ�
	//Ȼ��ͨ��ѭ���ȴ�USB�˵㲻��æµ������ȴ�ʱ�䳬��10000��ѭ��
	//����Ϊ����ʧ�ܣ������USB�˵��bit(0)
    reg_usb_ep_ctrl(CDC_TX_EPNUM) = FLD_EP_DAT_ACK;        // ACK
    u16 t = 0;
    while(usbhw_is_ep_busy(CDC_TX_EPNUM)) {
        if (t++ > 10000) {
            T_CNT++;
            reg_usb_ep_ctrl(CDC_TX_EPNUM) &= 0xfe; // clear bit(0)
        }
    };

    /* TX transaction finish */
	//���CDC�豸���ݷ�����ɣ������һЩ������������
	//����ֵ�����㣬
	if (cdc_v->lenToSend == 0) {
		//�Ѿ����͵����ݳ���
		cdc_v->lenToSend = 0;
		//���յ������ݳ���
		cdc_v->lastSendIndex = 0;
		//�ô���Ƭ�������ݷ�����ɺ����һЩ������
		//����cdc_v->txCb���ص�������������ʾ���ݷ�����ɣ��ȴ���һ�����ݷ���
        if (cdc_v->txCb) {
			//��cdc_v->txBuf��գ���ʾ���ݷ�����ɣ��ȴ���һ�����ݷ��͡�
            EV_SCHEDULE_TASK(cdc_v->txCb, cdc_v->txBuf);
			
        }

		cdc_v->txBuf = NULL;
	}

	return len;
}


/// @brief 
/// @param buf ������ָ��
/// @param len ���ݳ���
/// @return 
///�ú���������USB��������CDC�豸������
usbcdc_sts_t usbcdc_sendData(u8 *buf, u8 len)
{	
	//�����ж�CDC�豸��������ʱ�Ļ������Ƿ�æµ
	if (cdc_v->txBuf) {
		//���æµ���򷵻�USB_BUSY����ʾ��һ�����ݻ�δ�������
		//��Ҫ�ȴ���һ�����ݷ�����ɺ��ٴη������ݡ�
		return USB_BUSY;
	}

	/* Init the bulk transfer */
	//��ʼ��CDC�豸��������ʱ�Ļ�����
	//����������Ҫ���͵����ݳ���len
    cdc_v->lenToSend = len;
	//���ͻ�����ָ��buf
	cdc_v->txBuf = buf;
	//�Ѿ��Ѿ����͵����ݳ���
	cdc_v->lastSendIndex = 0;

    /* Send first bulk */
	//���ú������͵�һ������
	usbcdc_sendBulkData();
	//ͨ����������ACKȷ���źţ���ʾ�Ѿ�׼���ý���USB�������͵�����
	usbhw_data_ep_ack(USB_EDP_CDC_OUT);
	
	//���سɹ�
	return SUCCESS;
	
   
}


/// @brief 
/// @param  
/// @return 
///�ú��������ж�CDC�豸�Ƿ���Է�������
u8 usbcdc_isAvailable(void)
{
	return (cdc_v->txBuf == NULL);
}


#endif  /* USB_CDC_ENABLE */

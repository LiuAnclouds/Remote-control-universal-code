/********************************************************************************************************
 * @file     usbkb.c ,该程序主要针对键盘
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
/*你可能用到的知识*/
/*
1. 在USB HID协议中，
系统按键和媒体按键是通过不同的Usage Page和Usage ID来区分的。
系统按键的Usage Page为0x01，Usage ID为0x80；
媒体按键的Usage Page为0x0C，Usage ID为0x01。
2. USB HID协议规定，释放按键时，只需要发送一个长度为1的数据报文，
并将键码设置为0。因此，在释放系统按键时，
只需要发送一个长度为1的数据报文，并将数据置为0；在释放媒体按键时，
需要发送一个长度为`MEDIA_REPORT_DATA_LEN`的数据报文，
并将数据全部置为0。
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
///缓冲区
u8 usb_fifo[USB_FIFO_NUM][USB_FIFO_SIZE];

/// @brief 
///缓冲区读取指针
u8 usb_ff_rptr = 0;
/// @brief 
///缓冲区写入指针
u8 usb_ff_wptr = 0;


int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode);
void usbkb_hid_report(kb_data_t *data);

/// @brief 
///静态数组，包含系统按键的虚拟键值
static u8 vk_sys_map[VK_SYS_CNT] = {
	//电源键   	睡眠键		唤醒键
	VK_POWER_V, VK_SLEEP_V, VK_WAKEUP_V
};
/// @brief 
///静态数组，包含媒体按键的虚拟键值
static vk_ext_t vk_media_map[VK_MEDIA_CNT] = {
    {VK_W_SRCH_V},/*网络搜索键*/
    {VK_HOME_V},/*主页键*/
    {VK_W_BACK_V},/*网页后退键*/
    {VK_W_FORWRD_V},/*网页前进键*/
    {VK_W_STOP_V},/*网页停止键*/
    {VK_W_REFRESH_V},/*网页刷新键*/
    {VK_W_FAV_V},/*网页收藏键*/
    {VK_MEDIA_V},/*媒体键*/
    {VK_MAIL_V},/*表示邮件键*/
    {VK_CAL_V},/*表示日历键*/
    {VK_MY_COMP_V},/*表示我的电脑键*/
    {VK_NEXT_TRK_V},/*表示下一曲键*/
    {VK_PREV_TRK_V},/*表示上一曲键*/
    {VK_STOP_V},/*表示停止键*/
    {VK_PLAY_PAUSE_V},/*表示播放暂停键*/
    {VK_W_MUTE_V},/*表示静音键*/
    {VK_VOL_UP_V},/*表示音量调高键*/
    {VK_VOL_DN_V},/*表示音量减小键*/
};
/// @brief 
///枚举变量，内含键盘的事件类型
enum{
	/*普通按键*/
    KB_NORMAL_RELEASE_MASK = BIT(0),
	/*系统按键*/
    KB_SYS_RELEASE_MASK = BIT(1),
	/*媒体按键*/
    KB_MEDIA_RELEASE_MASK = BIT(2),
};



#define  USBKB_BUFF_DATA_NUM    8
//static

/// @brief 
///键盘数据缓冲区
kb_data_t kb_dat_buff[USBKB_BUFF_DATA_NUM];

//static
u8  usbkb_wptr, usbkb_rptr;
/// @brief 表示当前USB键盘未释放的按键数量
static u32 usbkb_not_released;
/// @brief 表示上一次USB键盘发送数据的时间
static u32 usbkb_data_report_time;


/// @brief 将参数传递到环形缓冲区kb_dat_buff中
/// @param packet_kb 
void usbkb_add_frame (rf_packet_keyboard_t *packet_kb)
{	
	 //根据pno 获得最新数据的个数
	u8 new_data_num = packet_kb->pno; 
	for(u8 i=0;i<new_data_num;i++)
	{
			memcpy4((int*)(&kb_dat_buff[usbkb_wptr]), (int*)(&packet_kb->data[i*sizeof(kb_data_t)]), sizeof(kb_data_t));
			//每次复制完毕后，将写指针+1
			BOUND_INC_POW2(usbkb_wptr,USBKB_BUFF_DATA_NUM);
			//如果写指针追上读指针
			if(usbkb_wptr == usbkb_wptr)
			{
					break;
			}
	}
}

/// @brief 发送USB键盘数据报文
/// @param  
void usbkb_report_frame(void)
{	
	//如果写指针不等于读指针，说明有键盘数据需要发送
	if(usbkb_wptr != usbkb_rptr){
		//从缓冲区中读取一个键盘数据结构
		kb_data_t *data = (kb_data_t *)(&kb_dat_buff[usbkb_rptr]);
        //发送键盘数据结构
		usbkb_hid_report(data);
		//将读指针+1
		BOUND_INC_POW2(usbkb_rptr,USBKB_BUFF_DATA_NUM);
	}
	return;
}
/// @brief 释放未释放的普通按键
/// @param  
static void usbkb_release_normal_key(void){
	//如果usbkb_not_released中的普通按键释放标志位被设置
	//说明有普通按键需要释放
	//KB_NORMAL_RELEASE_MASK：普通按键
	if(usbkb_not_released & KB_NORMAL_RELEASE_MASK){
		//将所有未释放的普通按键的键盘清零
		u8 normal_keycode[KEYBOARD_REPORT_KEY_MAX] = {0};
		//调用usbkb_hid_report_normal(0, normal_keycode)函数
		//来发送数据报文，以释放这些按键
		//如果发送成功，将普通按键释放标志位清零
		if(usbkb_hid_report_normal(0, normal_keycode)){
			//位运算函数，将普通按键释放标志位清零
			BM_CLR(usbkb_not_released, KB_NORMAL_RELEASE_MASK);
		}
	}
}
/// @brief 释放未释放的系统按键
/// @param  
static void usbkb_release_sys_key(void){
	//如果usbkb_not_released中的系统按键释放标志位被设置
	//说明有普通按键需要释放
	//KB_SYS_RELEASE_MASK：系统按键
	if(usbkb_not_released & KB_SYS_RELEASE_MASK){
		//将所有未释放的系统按键的键盘清零
		u32 release_data = 0;
		//调用usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&release_data)函数
		//来发送数据报文，以释放这些按键
		//如果发送成功，将系统按键释放标志位清零
		if(usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&release_data), 1)){
			//位运算函数，将系统按键释放标志位清零
			BM_CLR(usbkb_not_released, KB_SYS_RELEASE_MASK);
		}
	}
}
/// @brief 释放未释放的媒体按键
/// @param  
static void usbkb_release_media_key(void){
	//如果usbkb_not_released中的媒体按键释放标志位被设置
	//说明有普通按键需要释放
	//KB_SYS_RELEASE_MASK：媒体按键
	if(usbkb_not_released & KB_MEDIA_RELEASE_MASK){
		//将所有未释放的媒体按键的键盘清零
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
		//调用usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)函数
		//来发送数据报文，以释放这些按键
		//如果发送成功，将媒体按键释放标志位清零
		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//位运算函数，将系统按键释放标志位清零
			BM_CLR(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}
}
/// @brief 运行释放按键函数，内有要求和解释
/// @param  
static void usbkb_release_keys(void){
	// 不要调换usbkb_release_sys_key 和usbkb_release_normal_key 的顺序，
	// 这是为了让 usbkb_release_sys_key,  usbkb_release_media_key 中间有一定延时
	usbkb_release_sys_key();
	usbkb_release_normal_key();
	usbkb_release_media_key();
}
/// @brief 检查是否还有未释放的按键
void usbkb_release_check(){
	//如果有，且超过了检查时间，则调用释放函数来释放
	if(usbkb_not_released && clock_time_exceed(usbkb_data_report_time, USB_KEYBOARD_RELEASE_TIMEOUT)){
		usbkb_release_keys();	 //  release keys
	}

}
/// @brief 将传入的键码数组中的按键类型分离出来，并返回普通按键的个数
/// @param keycode 键码数组指针
/// @param cnt 键码数量
/// @param normal_key //用于存储普通按键的数组指针
/// @param ext_key //用于存储扩展按键的指针
/// @return 
///  normal_keycode 里面返回普通按键
int usbkb_separate_key_types(u8 *keycode, u8 cnt, u8 *normal_key, u8 *ext_key){
    //一个编译期间的断言，用于判断KB_RETURN_KEY_MAX是否小于等于KEYBOARD_REPORT_KEY_MAX
    //如果不满足该条件，则编译无法通过
	STATIC_ASSERT(KB_RETURN_KEY_MAX <= KEYBOARD_REPORT_KEY_MAX);
    //运行时的断言，用于判断传入的键码数量是否小于等于KB_RETURN_KEY_MAX
	//如果不满足该条件，则程序会终端
	assert(cnt <= KB_RETURN_KEY_MAX);
	//普通键码数量
    int normal_cnt = 0;
	//遍历数组，判断是扩展按键还是普通按键
	//VK_EXT_START/End:定义在USB HID协议中的扩展键码的起始和结束值。
	//如果键码大于等于VK_EXT_START且小于VK_EXT_End，则说明该按键为扩展按键
    foreach(i, cnt){
        if(keycode[i] >= VK_EXT_START && keycode[i] < VK_EXT_END){
        	*ext_key = keycode[i];//扩展按键
        }else{
            normal_key[normal_cnt++] = keycode[i];//普通按键
        }
    }
	return normal_cnt;
}
/// @brief 将普通按键的键码和控制键打包成USB HID报告并发送到主机
/// @param ctrl_key 控制键
/// @param keycode 键码数组指针
/// @return 
int usbkb_hid_report_normal(u8 ctrl_key, u8 *keycode){
	//用于判断USB EDI是否忙碌，忙碌则返回非0值
	if(usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN)){
		//将USB FIFO（缓冲区）中的下一个可用空间地址赋给pData指针
		//并将usb_ff_wptr的值+1，使其指向下一个可用空间
		//& (USB_FIFO_NUM - 1)的作用是将usb_ff_wptr限制在其范围内，以实现循环利用
		u8 *pData = (u8 *)&usb_fifo[usb_ff_wptr++ & (USB_FIFO_NUM - 1)];
		//将数据类型标记为键盘数据类型
		pData[0] = DAT_TYPE_KB;
		//将控制键写入报告数据中
		pData[1] = ctrl_key;
		//将键码数组中的前6个键码复制到报告数据中
		memcpy(pData + 2, keycode, 6);
		//计算USB FIFO中已使用的空间大小，用于判断是否发生溢出
		int fifo_use = (usb_ff_wptr - usb_ff_rptr) & (USB_FIFO_NUM*2-1);
		//判断是否发生溢出，如果已使用的空间大小大于USB FIFO的大小，则发生了溢出
		if (fifo_use > USB_FIFO_NUM) {
			usb_ff_rptr++;//将其值+1，丢弃最早的数据
			//fifo overflow, overlap older data
		}

		return 0;
	}
	//将USB EDI的指针设置为0，表示该EDP已经发送完毕。
	reg_usb_ep_ptr(USB_EDP_KEYBOARD_IN) = 0;



///以下代码是两种键盘数据发送模式
///一种是带有CRC校验的发送方式
///另一种是不带CRC校验的发送方式

//宏定义是否开启软件CRC校验功能
#if USB_SOFTWARE_CRC_CHECK

	//crc_in用于存储需要进行CRC校验的数据
	unsigned char crc_in[KEYBOARD_REPORT_KEY_MAX+2];
	//crc和crch分别表示CRC校验值和CRC校验值的高8位
	unsigned short crc;
	unsigned int crch;

	crc_in[0] =  ctrl_key;
	crc_in[1] =  0;
	//用于遍历数组，实现对crc_in数组的初始化
	foreach(i, KEYBOARD_REPORT_KEY_MAX){
		crc_in[i+2] = keycode[i];
	}
	//USB_CRC16用于计算CRC校验值
	crc = USB_CRC16 (crc_in, KEYBOARD_REPORT_KEY_MAX+2);
	//右移8位
	crch = crc >> 8;
	//用于判断是否需要对校验值进行交换
	if ((crch==0x06) || (crch==0x04) || (crch == 0x00))
	{
		unsigned int tmp = crc_in[2];
		crc_in[2] = crc_in[3];
		crc_in[3] = tmp;
	}
	// please refer to keyboard_report_desc
	foreach(i, (KEYBOARD_REPORT_KEY_MAX+2)){
		//将数据发送到USB端点
	    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = crc_in[i];
	}

#else
	//不用校验的情况
	// please refer to keyboard_report_desc
	//将数据发送给USB端口
	//ctrl_key：控制键
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = ctrl_key;
	reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = 0;//resv
	foreach(i, KEYBOARD_REPORT_KEY_MAX){
		//键码数组指针
	    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = keycode[i];
	}

#endif
//	reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK;		// ACK
	//控制USB端点，并发送ACK应答信号，表示数据已经发送完成
	reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK | (edp_toggle[USB_EDP_KEYBOARD_IN] ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0);  // ACK
	///该数组用于记录USB端点的数据传输状态，每次发送数据时，
	///需要根据数值的值,来选择不同的数据缓冲区。
	///^=表示异或操作
	edp_toggle[USB_EDP_KEYBOARD_IN] ^= 1;

	return 1;
}
/// @brief 该函数用于发送普通按键的数据
/// @param ctrl_key 键盘修饰状态
/// @param keycode 按下的键值
/// @param cnt 表示按下的键数
static inline void usbkb_report_normal_key(int ctrl_key, u8 *keycode, int cnt){
	//用于判断是否需要发送数据，如果cnt大于0或者ctrl_key不为0，
	//则需要发送数据
	if(cnt > 0 || ctrl_key)
	{	
		//发送普通按键数据
		if(usbkb_hid_report_normal(ctrl_key, keycode)){
			//BM_SE用于设置usbkb_not_released的值，表示未释放
		    BM_SET(usbkb_not_released, KB_NORMAL_RELEASE_MASK);
		}
	}else//不需要发送数据，表示没有按键按下，或者所有按键都已经释放
	{	
		//释放所有普通按键
		usbkb_release_normal_key();
	}
}
/// @brief 该函数用于发送系统按键的数据
/// @param ext_key 表示按下的系统键值
static inline void usbkb_report_sys_key(u8 ext_key)
{	
	//判断是否为系统键值
	if(ext_key >= VK_SYS_START && ext_key < VK_SYS_END)
	{
		//表示系统键值在VK_STS_MAP(系统键盘)上的下标
		int idx = ext_key - VK_SYS_START;

		if(usbmouse_hid_report(USB_HID_KB_SYS, (u8*)(&vk_sys_map[idx]), 1)){	// assert sys key len == 1, check descriptor
			//BM_SE用于设置usbkb_not_released的值，表示未释放
			BM_SET(usbkb_not_released, KB_SYS_RELEASE_MASK);
		}
	}else//不需要发送数据，表示没有按键按下，或者所有按键都已经释放
	{	
		//释放所有普通按键
		usbkb_release_sys_key();
	}
}
/// @brief 该函数用于发送媒体按键的数据
/// @param ext_key 表示按下的媒体键值
static inline void usbkb_report_media_key(u8 ext_key){
	//判断是否为媒体键值
	if(ext_key >= VK_MEDIA_START && ext_key < VK_MEDIA_END){
		STATIC_ASSERT(VK_EXT_LEN <= MOUSE_REPORT_DATA_LEN);
	//媒体键值初始化为0
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
	//媒体键值在vk_media_map(媒体键盘)中的下标
		int idx = ext_key - VK_MEDIA_START;
		//循环把媒体键值赋值给ext_keycode数组
		foreach(i, VK_EXT_LEN){
			ext_keycode[i] = vk_media_map[idx].val[i];
		}

		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//BM_SE用于设置usbkb_not_released的值，表示未释放
			BM_SET(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}else//不需要发送数据，表示没有按键按下，或者所有按键都已经释放
	{	
		//释放所有普通按键
		usbkb_release_media_key();
	}
}

/// @brief 该函数用于发送消费者按键的数据
/// @param consumer_key 表示按下的消费者键值
void usbkb_report_consumer_key(u16 consumer_key)
{	
	//判断是否是消费者键值
	if(consumer_key)
	{
		//赋予消费者键值初值0（初始化）
		u8 ext_keycode[MOUSE_REPORT_DATA_LEN] = {0};
		//循环把消费者键值赋值给ext_keycode数组
		foreach(i, VK_EXT_LEN){
			ext_keycode[i] = consumer_key;
			consumer_key >>=8;
		}

		if(usbmouse_hid_report(USB_HID_KB_MEDIA, ext_keycode, MEDIA_REPORT_DATA_LEN)){
			//按下后表示未释放
			//BM_SE用于设置usbkb_not_released的值，表示未释放
			BM_SET(usbkb_not_released, KB_MEDIA_RELEASE_MASK);
		}
	}else//不需要发送数据，表示没有按键按下，或者所有按键都已经释放
	{	
		//释放所有普通按键
		usbkb_release_media_key();
	}

	//用于记录数据报告的诗句，即键盘数据最后一次发送的时间
    usbkb_data_report_time = clock_time();

}


/// @brief 该函数用于比较两个键盘数据是否相同
/// @param a 键盘数据1的指针
/// @param b 键盘数据2的指针
/// @return 
int kb_is_data_same(kb_data_t *a, kb_data_t *b)
{	
	//如果a、b为空指针
	if(!a || !b){
		return 0;
	}
	//如果两个键盘数据的按键数量或控制键不相同
	if((a->cnt != b->cnt) || (a->ctrl_key != b->ctrl_key))
		return 0;
	//循环用于比较两个键盘数据的每个按键值
	foreach(i, a->cnt)
	{	
		//如果有不相同
		if(a->keycode[i] != b->keycode[i])
		{
			return 0;
		}
	}
	//如果都相同，返回1
	return 1;
}
/// @brief 该函数用于检查键盘按键是否重复，并且保存最近一次的键盘数据
/// @param data 当前键盘数据
/// @return 
static inline int usbkb_check_repeat_and_save(kb_data_t *data){
    //首先assert宏检查dat是否为空指针
	assert(data);
	//定义一个静态变量，用于保存最近一次的键盘数据
	static kb_data_t last_data;
	//判断当前的键盘数据和最近一次的键盘数据是否相同
	int same = kb_is_data_same(&last_data, data);
	//如果不相同
	if(!same)
	{	
		//把数据都拷贝到last_data静态变量中
		STATIC_ASSERT(sizeof(last_data) == 8);
	    ((u32*) (&last_data))[0] = ((u32*) (data))[0];
	    ((u32*) (&last_data))[1] = ((u32*) (data))[1];
	}
	//返回same说明是否重复
	return same;
}

/// @brief 用于处理键盘数据并发送HID报告
/// @param data 表示当前的键盘数据
void usbkb_hid_report(kb_data_t *data){
	//先使用assert检查data是否为空指针
    assert(data);
	//ext_key扩展键数量,normal_key_cnt普通键数量
    u8 ext_key = VK_EXT_END, normal_key_cnt = 0;
	//用于保存正常键码
    u8 normal_keycode[KEYBOARD_REPORT_KEY_MAX] = {0};

	//如果当前键盘数据的数量大于KB_RETURN_KEY_MAX,则直接返回，为了防止错误数据包的影响
	if(data->cnt > KB_RETURN_KEY_MAX){	// must,   in case bad packets
		return;
	}

	/*	http://msdn.microsoft.com/en-us/windows/hardware/gg462991.aspx
		It is also important to notice that any re-triggering of events should be done by software timers in the host system and not by hardware timers in the device itself.
		For example, if the user keeps pressing the Volume Increment button, the device should only generate one input report with this state information
	*/
	//如果当前没有按键按下
	if((data->cnt > 0 || data->ctrl_key) && usbkb_check_repeat_and_save(data)){
		if(usbkb_not_released){
			usbkb_data_report_time = clock_time();
			return;
		}
	}

	//如果当前有按键按下
	if(data->cnt > 0){
		//  keycode分类处理,分为扩展按键和正常按键
	    normal_key_cnt = usbkb_separate_key_types(data->keycode, data->cnt, normal_keycode, &ext_key);
	}
	//发送普通按键数据
	usbkb_report_normal_key(data->ctrl_key, normal_keycode, normal_key_cnt);
	//发送系统按键数据
	usbkb_report_sys_key(ext_key);
	//发送媒体按键数据
	usbkb_report_media_key(ext_key);
	//最后一次发送报告的时间
    usbkb_data_report_time = clock_time();
}

/// @brief 该函数用于对键盘按键进行初始化
void usbkb_init(){
	//ev_on_poll(EV_POLL_KEYBOARD_RELEASE_CHECK, usbkb_release_check);
}

/// @brief 用于处理USB数据报告队列并发送HID报告
/// @param  
/// @return 
int usb_hid_report_fifo_proc(void)
{	
	//如果读指针与写指针相等，则表示队列中没有数据，直接返回
	if(usb_ff_rptr == usb_ff_wptr){
		return 0;
	}
	//指向usb_fifo数组中，读取指针所在的地址
	u8 *pData = (u8 *)&usb_fifo[usb_ff_rptr & (USB_FIFO_NUM - 1)];
	//如果当前数据是键盘数据
	if(pData[0] == DAT_TYPE_KB)
	{
		//调用usbhw_is_ep_busy函数判断是否有端点在传送数据
		if(usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN))
		{
			return 0;
		}
		else{
			//如果没有
			//调用函数发送键盘HID报告
			usbkb_hid_report_normal(pData[1], pData + 2);
			//读指针+1
			usb_ff_rptr ++;

			return 1;
		}
	}
	else if(pData[0] == DAT_TYPE_MOUSE)//如果未鼠标数据
	{	
		//调用usbhw_is_ep_busy函数判断是否有端点在传送数据
		if(usbhw_is_ep_busy(USB_EDP_MOUSE))
		{
			return 0;
		}
		else//没有端点在传送数据
		{
			//调用函数发送键盘HID报告
			usbmouse_hid_report(pData[1], pData + 4, pData[2]);
			//读指针+1
			usb_ff_rptr ++;

			return 1;
		}
	}


	return 0;
}


#endif

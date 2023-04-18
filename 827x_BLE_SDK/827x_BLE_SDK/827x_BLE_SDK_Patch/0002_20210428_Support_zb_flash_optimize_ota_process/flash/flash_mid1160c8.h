/********************************************************************************************************
 * @file	flash_mid1160c8.h
 *
 * @brief	This is the header file for b85m
 *
 * @author	Driver Group
 * @date	2020
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef __MID1160C8_H__
#define __MID1160C8_H__
//关于MID=0x1160c8型号的Flash芯片GD25LD10C的定义。
//其中定义了Flash的保护区域和可修改的状态范围
/*
 * @brief     MID = 0x1160c8 Flash include GD25LD10C.
 */


/**
 * @brief     define the section of the protected memory area which is read-only and unalterable.
 */
typedef enum{
	//表示无保护，整个Flash区域均可读写
	FLASH_LOCK_NONE_MID1160C8		=	0x00,
	//表示保护了Flash的前120k字节（0x000000h-0x01DFFFh)
	//只有该区域以外的部分可以读写
	FLASH_LOCK_LOW_120K_MID1160C8	=	0x04,	//000000h-01DFFFh
	//表示保护了Flash的前112K字节（000000h-01BFFFh）区域
	//这有该区域以外的部分可以读写
	FLASH_LOCK_LOW_112K_MID1160C8	=	0x08,	//000000h-01BFFFh
	//表示保护了Flash的前96K字节（000000h-017FFFh）区域
	//这有该区域以外的部分可以读写
	FLASH_LOCK_LOW_96K_MID1160C8	=	0x0c,	//000000h-017FFFh
	//表示保护了Flash的前64K字节（000000h-00FFFFh）区域
	//这有该区域以外的部分可以读写
	FLASH_LOCK_LOW_64K_MID1160C8	=	0x10,	//000000h-00FFFFh
	//表示保护了Flash的前128K字节（000000h-01FFFFh）区域
	//这有该区域以外的部分可以读写
	FLASH_LOCK_ALL_128K_MID1160C8	=	0x1c,	//000000h-01FFFFh
}mid1160c8_lock_block_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID1160C8	=	0x1c,
	//表示可修改的状态范围，只有该位可修改，其余位不可修改。其中BP(Block Protect)位用于设置保护区域
}mid1160c8_write_status_bit_e;
//以上代码是为了方便MID=0X1160C8型号的flash芯片GD25LD10C进行操作而定义的

/**
 * @brief 		This function reads the status of flash.
 * @return 		the value of status.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
unsigned char flash_read_status_mid1160c8(void);
//这段代码定义了一个函数flash_read_status_mid1160c8，
//其返回值类型为unsigned char。
//函数的作用是读取MID = 0x1160c8型号的Flash芯片GD25LD10C的状态寄存器的值。
//具体实现可能涉及到与硬件的交互，读取Flash的状态寄存器值可能包括多个状态位，需要根据具体情况进行解析。
/**
 * @brief 		This function write the status of flash.
 * @param[in]  	data	- the value of status.
 * @param[in]  	bit		- the range of bits to be modified when writing status.
 * @return 		none.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
void flash_write_status_mid1160c8(unsigned char data, mid1160c8_write_status_bit_e bit);
//这段代码定义了一个函数flash_write_status_mid1160c8，其返回值类型为void。
//函数的作用是向MID = 0x1160c8型号的Flash芯片GD25LD10C的状态寄存器中写入值data，写入的位置由参数bit指定。
//具体实现可能涉及到与硬件的交互，需要根据具体情况将要写入的值data和位置bit转换为硬件可识别的格式，并进行写入操作。
//其中，参数bit是一个枚举类型，表示可修改的状态位范围，具体取值可参考前面的定义。
/**
 * @brief 		This function serves to set the protection area of the flash.
 * @param[in]   data	- refer to the protection area definition in the .h file.
 * @return 		none.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
void flash_lock_mid1160c8(mid1160c8_lock_block_e data);
//这段代码定义了一个函数flash_lock_mid1160c8，其返回值类型为void。
//函数的作用是对MID = 0x1160c8型号的Flash芯片GD25LD10C进行区域保护，保护的区域由参数data指定。
//具体实现可能涉及到与硬件的交互，需要根据具体情况将要保护的区域data转换为硬件可识别的格式，并进行保护操作。
//其中，参数data是一个枚举类型，表示保护的区域范围，具体取值可参考前面的定义。
/**
 * @brief 		This function serves to flash release protection.
 * @return 		none.
 * @note        Attention: Before calling the FLASH function, please check the power supply voltage of the chip.
 *              Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called.
 *              Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater
 *              than the minimum chip operating voltage. For the specific value, please make a reasonable setting according
 *              to the specific application and hardware circuit.
 *
 *              Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply,
 *              there may be a risk of error in the operation of the flash (especially for the write and erase operations.
 *              If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure)
 */
void flash_unlock_mid1160c8(void);
//这段代码定义了一个函数flash_unlock_mid1160c8，其返回值类型为void。
//函数的作用是对MID = 0x1160c8型号的Flash芯片GD25LD10C进行解除保护，即将整个Flash区域设置为可读写状态。
//具体实现可能涉及到与硬件的交互，需要根据具体情况将Flash解除保护的指令发送给硬件，并进行解除保护操作。

#endif


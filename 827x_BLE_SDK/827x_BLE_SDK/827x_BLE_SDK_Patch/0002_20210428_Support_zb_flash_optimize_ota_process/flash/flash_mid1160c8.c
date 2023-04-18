/********************************************************************************************************
 * @file	flash_mid1160c8.c
 *
 * @brief	This is the source file for b85m
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
#include "flash_type.h"

#if FLASH_LOCK_EN
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
//用于读取flash的状态
unsigned char flash_read_status_mid1160c8(void)
{
	return flash_read_status(FLASH_READ_STATUS_CMD_LOWBYTE);
}

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
//用于写入flash的状态,在写入前需要检查芯片的电源电压是否达到安全值
//若电压过低则可能导致flash操作错误，甚至导致产品故障。
//同时，在写入状态时只能修改指定范围的位，以避免对其他位的影响。
void flash_write_status_mid1160c8(unsigned char data, mid1160c8_write_status_bit_e bit)
{
	unsigned char status = flash_read_status(FLASH_READ_STATUS_CMD_LOWBYTE);
	//先读取当前flash的状态，然后通过位运算将要写入的状态数据与当前状态数据合并
	//同时要注意将不需要修改的位清零
	data |= (status & ~(bit));
	flash_write_status(FLASH_TYPE_8BIT_STATUS, data);
	//调用写入将合并后的状态数据写入flash
	//mid1160c8_write_status_bit_e是一个枚举类型，表示要修改的状态位的范围
}

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
//以下代码实现了flash的锁定功能
//1.首先，传入的参数data是一个枚举类型，表示要锁定的flash块的范围。
//2.然后，调用flash_write_status_mid1160c8函数，将要写入的状态数据与当前状态数据合并。
//以修改指定的状态位。
//3.在本例中，要修改的是FLASH_WRITE_STATUS_BP_MID1160C8位。
//它表示flash的写保护状态，将其设置为1就可以锁定flash块
//4.最后，将修改后的状态数据写入flash，以完成锁定操作。
void flash_lock_mid1160c8(mid1160c8_lock_block_e data)
{
	flash_write_status_mid1160c8(data, FLASH_WRITE_STATUS_BP_MID1160C8);
}

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
//这段代码实现了flash的解锁功能，具体实现如下：
//1. 首先，调用`flash_write_status_mid1160c8`函数，将要写入的状态数据与当前状态数据合并，
//以修改指定的状态位。
//2. 在本例中，要修改的是`FLASH_WRITE_STATUS_BP_MID1160C8`位，它表示flash的写保护状态，
//将其设置为0就可以解锁flash块。
//3. 同时，将要写入的状态数据设置为`FLASH_LOCK_NONE_MID1160C8`，表示不锁定任何flash块。
//4. 最后，将修改后的状态数据写入flash，以完成解锁操作。
void flash_unlock_mid1160c8(void)
{
	flash_write_status_mid1160c8(FLASH_LOCK_NONE_MID1160C8, FLASH_WRITE_STATUS_BP_MID1160C8);
}
#endif

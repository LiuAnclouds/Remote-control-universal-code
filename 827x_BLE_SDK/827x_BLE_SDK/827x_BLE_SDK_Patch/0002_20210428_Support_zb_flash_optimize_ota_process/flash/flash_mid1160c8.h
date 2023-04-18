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
//����MID=0x1160c8�ͺŵ�FlashоƬGD25LD10C�Ķ��塣
//���ж�����Flash�ı�������Ϳ��޸ĵ�״̬��Χ
/*
 * @brief     MID = 0x1160c8 Flash include GD25LD10C.
 */


/**
 * @brief     define the section of the protected memory area which is read-only and unalterable.
 */
typedef enum{
	//��ʾ�ޱ���������Flash������ɶ�д
	FLASH_LOCK_NONE_MID1160C8		=	0x00,
	//��ʾ������Flash��ǰ120k�ֽڣ�0x000000h-0x01DFFFh)
	//ֻ�и���������Ĳ��ֿ��Զ�д
	FLASH_LOCK_LOW_120K_MID1160C8	=	0x04,	//000000h-01DFFFh
	//��ʾ������Flash��ǰ112K�ֽڣ�000000h-01BFFFh������
	//���и���������Ĳ��ֿ��Զ�д
	FLASH_LOCK_LOW_112K_MID1160C8	=	0x08,	//000000h-01BFFFh
	//��ʾ������Flash��ǰ96K�ֽڣ�000000h-017FFFh������
	//���и���������Ĳ��ֿ��Զ�д
	FLASH_LOCK_LOW_96K_MID1160C8	=	0x0c,	//000000h-017FFFh
	//��ʾ������Flash��ǰ64K�ֽڣ�000000h-00FFFFh������
	//���и���������Ĳ��ֿ��Զ�д
	FLASH_LOCK_LOW_64K_MID1160C8	=	0x10,	//000000h-00FFFFh
	//��ʾ������Flash��ǰ128K�ֽڣ�000000h-01FFFFh������
	//���и���������Ĳ��ֿ��Զ�д
	FLASH_LOCK_ALL_128K_MID1160C8	=	0x1c,	//000000h-01FFFFh
}mid1160c8_lock_block_e;

/**
 * @brief     the range of bits to be modified when writing status.
 */
typedef enum{
	FLASH_WRITE_STATUS_BP_MID1160C8	=	0x1c,
	//��ʾ���޸ĵ�״̬��Χ��ֻ�и�λ���޸ģ�����λ�����޸ġ�����BP(Block Protect)λ�������ñ�������
}mid1160c8_write_status_bit_e;
//���ϴ�����Ϊ�˷���MID=0X1160C8�ͺŵ�flashоƬGD25LD10C���в����������

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
//��δ��붨����һ������flash_read_status_mid1160c8��
//�䷵��ֵ����Ϊunsigned char��
//�����������Ƕ�ȡMID = 0x1160c8�ͺŵ�FlashоƬGD25LD10C��״̬�Ĵ�����ֵ��
//����ʵ�ֿ����漰����Ӳ���Ľ�������ȡFlash��״̬�Ĵ���ֵ���ܰ������״̬λ����Ҫ���ݾ���������н�����
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
//��δ��붨����һ������flash_write_status_mid1160c8���䷵��ֵ����Ϊvoid��
//��������������MID = 0x1160c8�ͺŵ�FlashоƬGD25LD10C��״̬�Ĵ�����д��ֵdata��д���λ���ɲ���bitָ����
//����ʵ�ֿ����漰����Ӳ���Ľ�������Ҫ���ݾ��������Ҫд���ֵdata��λ��bitת��ΪӲ����ʶ��ĸ�ʽ��������д�������
//���У�����bit��һ��ö�����ͣ���ʾ���޸ĵ�״̬λ��Χ������ȡֵ�ɲο�ǰ��Ķ��塣
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
//��δ��붨����һ������flash_lock_mid1160c8���䷵��ֵ����Ϊvoid��
//�����������Ƕ�MID = 0x1160c8�ͺŵ�FlashоƬGD25LD10C�������򱣻��������������ɲ���dataָ����
//����ʵ�ֿ����漰����Ӳ���Ľ�������Ҫ���ݾ��������Ҫ����������dataת��ΪӲ����ʶ��ĸ�ʽ�������б���������
//���У�����data��һ��ö�����ͣ���ʾ����������Χ������ȡֵ�ɲο�ǰ��Ķ��塣
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
//��δ��붨����һ������flash_unlock_mid1160c8���䷵��ֵ����Ϊvoid��
//�����������Ƕ�MID = 0x1160c8�ͺŵ�FlashоƬGD25LD10C���н����������������Flash��������Ϊ�ɶ�д״̬��
//����ʵ�ֿ����漰����Ӳ���Ľ�������Ҫ���ݾ��������Flash���������ָ��͸�Ӳ���������н������������

#endif


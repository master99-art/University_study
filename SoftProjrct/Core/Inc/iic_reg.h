#ifndef __IIC_REG_H
#define __IIC_REG_H



/**
 * @author guoch
 * @note TMP411AQDGKRQ1
 * @note 这个部分是温度传感器的寄存器的值
 * @note
 */

#define TEMPER_Dev 0X4C
#define TEMPER_Local_H 0x00 // 本地温度高
#define TEMPER_Local_L 0x15 // 本地温度低

#define TEMPER_Remote_H 0x01 // 远程温度高
#define TEMPER_Remote_L 0x10 // 远程温度低

#define TEMPER_Command_W 0x09 // 配置寄存器写
#define TEMPER_Command_R 0x03 // 配置寄存器读
#define TEMPER_PixAddress_WR 0x1A // 温度分辨率配置
#define TEMPER_Pix_00625 0x1f     // 温度分辨率为0.0625

#endif
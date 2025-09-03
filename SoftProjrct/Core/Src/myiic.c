#include "myiic.h"
#include "main.h"
#include "i2c.h"

/**
 * @author guoch
 * @brief 这是IIC1的发送函数
 * @param DevAddress 需要响应的从机地址
 * @param pData 需要发送的数据
 * @param Size 需要发送的数据的长度
 * @note 这是一个底层的IIC数据发送函数，我们在编写这个函数的应用层的时候应该再写一个函数来把这个函数给封装出来
 * 
 */
void IIC_Send(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, 0xf);
}

void IIC_Send_men(uint16_t DevAddress,  
    uint16_t MemAddress, 
    uint8_t *pData
    )
{
    HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress,
                      I2C_MEMADD_SIZE_8BIT, pData, 1, 0x0f);
}

/**
 * @author guoch
 * @brief 这是IIC1的接收函数
 * @param DevAddress 需要响应的从机地址
 * @param redAdr 寄存器地址
 * @param pData 需要接收的数据
 * @param Size 需要接收的数据的长度
 * @note 这是一个底层的IIC数据接收函数，我们在编写这个函数的应用层的时候应该再写一个函数来把这个函数给封装出来
 *
 */
void IIC_Read(uint16_t DevAddress, uint8_t redAdr,uint8_t *pData, uint16_t Size)
{
    uint8_t data = redAdr;
    HAL_I2C_Master_Transmit(&hi2c1, DevAddress, &data, 1, 0xf);
    
    HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, 0xf);
}
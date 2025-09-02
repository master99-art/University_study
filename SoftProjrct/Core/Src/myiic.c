#include "myiic.h"
#include "main.h"
#include "i2c.h"

/**
 * @author guoch
 * @brief IIC_Send
 */
void IIC_Send(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, 0xf);
}


void IIC_Send_men(uint16_t DevAddress,  
    uint16_t MemAddress, 
    uint8_t *pData, 
    )
{
    HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress,
                      I2C_MEMADD_SIZE_8BIT, pData, 1, 0x0f);
}
void IIC_Read(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, 0xf);
}
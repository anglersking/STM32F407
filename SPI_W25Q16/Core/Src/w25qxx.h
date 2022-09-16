#ifndef __W25QXX_H
#define __W25QXX_H			    
#include "stm32f4xx_hal.h"



#define W25X_ManufactDeviceID  0x90  		/* Read identification */
#define sFLASH_CMD_WREN        0x06			/* Write enable instruction */
#define sFLASH_CMD_RDSR        0x05			/* Read Status Register instruction  */
#define sFLASH_CMD_SE          0x20			/* Sector Erase instruction */
#define sFLASH_CMD_WRITE       0x02  		/* Write to Memory instruction */
#define sFLASH_CMD_READ        0x03			/* Read from Memory instruction */	

#define sFLASH_DUMMY_BYTE      0x00
#define sFLASH_BUSY_FLAG        0x01
#define sFLASH_SPI_PAGESIZE       0x100

/* 选中芯片： 拉低片选 */
#define sFLASH_CS_LOW()       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)
/* 释放芯片： 拉高片选 */
#define sFLASH_CS_HIGH()      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)



uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_ReadID(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

#endif

















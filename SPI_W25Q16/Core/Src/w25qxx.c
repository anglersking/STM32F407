#include "w25qxx.h" 
extern SPI_HandleTypeDef hspi1;

/*��дһ���ֽں���*/
uint8_t sFLASH_SendByte(uint8_t byte)
{
	uint8_t TX_DATA = byte;
	uint8_t RX_DATA = 0;
	
	HAL_SPI_TransmitReceive(&hspi1, &TX_DATA ,&RX_DATA , 1, 1000);
	
	return RX_DATA;
}

/*�ȴ���������д�������*/
void sFLASH_WaitForEnd(void)
{
	uint8_t sr_value = 0;
	
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(sFLASH_CMD_RDSR);
	
	do{
		
		sr_value = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
		
	}while( sr_value & sFLASH_BUSY_FLAG);

	
	sFLASH_CS_HIGH();
}

void sFLASH_WriteEnable(void)
{
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(sFLASH_CMD_WREN);
	
	sFLASH_CS_HIGH();
}


uint16_t sFLASH_ReadID(void)
{
	uint16_t FLASH_ID;
	uint8_t temp0,temp1;
	
	
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(W25X_ManufactDeviceID);
	
	sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	
	temp0 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	temp1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	
	sFLASH_CS_HIGH();
	
	FLASH_ID = (temp0 << 8) | temp1;

	return FLASH_ID;
}

void sFLASH_EraseSector(uint32_t SectorAddr)
{
	sFLASH_WriteEnable();  //����дʹ��
	
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(sFLASH_CMD_SE);
	sFLASH_SendByte( (SectorAddr>>16) & 0xff);   //���͸�8λ
	sFLASH_SendByte( (SectorAddr>>8) & 0xff);     //������8λ
	sFLASH_SendByte( (SectorAddr>>0) & 0xff);      //���͵�8λ
	 
	sFLASH_CS_HIGH();
	
	/*�ȴ��������*/
	sFLASH_WaitForEnd();
	
}

void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(sFLASH_CMD_READ);
	sFLASH_SendByte( (ReadAddr>>16) & 0xff);   //���͸�8λ
	sFLASH_SendByte( (ReadAddr>>8) & 0xff);     //������8λ
	sFLASH_SendByte( (ReadAddr>>0) & 0xff);      //���͵�8λ
	
	while(NumByteToRead--)
	{
		* pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
		pBuffer++;
	}

	sFLASH_CS_HIGH();
}

void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	if(NumByteToWrite > sFLASH_SPI_PAGESIZE )
	{
		NumByteToWrite = sFLASH_SPI_PAGESIZE;
		
		printf("д������̫�󣬳���һҳ�Ĵ�С\n");
	}
	
	sFLASH_WriteEnable();  //����дʹ��
	
	sFLASH_CS_LOW();
	
	sFLASH_SendByte(sFLASH_CMD_WRITE);
	sFLASH_SendByte( (WriteAddr>>16) & 0xff);   //���͸�8λ
	sFLASH_SendByte( (WriteAddr>>8) & 0xff);     //������8λ
	sFLASH_SendByte( (WriteAddr>>0) & 0xff);      //���͵�8λ
	
	while(NumByteToWrite--)
	{
		sFLASH_SendByte(* pBuffer);
		pBuffer++;
	}
	
	sFLASH_CS_HIGH();
	
	/*�ȴ�д���*/
	sFLASH_WaitForEnd();
}

void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	uint16_t NumOfPage, NumOfBytes, count, offset;
	
	offset = WriteAddr % sFLASH_SPI_PAGESIZE;
	count = sFLASH_SPI_PAGESIZE - offset;
	
	/* ����ҳ����������*/
	if(offset && (NumByteToWrite > count ))
	{
		sFLASH_WritePage(pBuffer,WriteAddr,count);
		
		NumByteToWrite -= count;
		
		pBuffer += count;
		WriteAddr += count;
	}
	
	
	NumOfPage = NumByteToWrite / sFLASH_SPI_PAGESIZE;
	NumOfBytes = NumByteToWrite % sFLASH_SPI_PAGESIZE;
	
	if(NumOfPage)
	{
			while(NumOfPage--)
			{
				sFLASH_WritePage(pBuffer,WriteAddr,sFLASH_SPI_PAGESIZE);
				pBuffer += sFLASH_SPI_PAGESIZE;
				WriteAddr += sFLASH_SPI_PAGESIZE;
			}
	}
	
	if(NumOfBytes)
	{
		sFLASH_WritePage(pBuffer,WriteAddr,NumOfBytes);
	}
}



























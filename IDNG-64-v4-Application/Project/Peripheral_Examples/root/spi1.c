
#include "stm32f0xx.h"
#include "spi1.h"




/*
void spi1Send (unsigned char data)					
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4); // CS low
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
  SPI_SendData8(SPI1, data);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait finish SPI1sending
	GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high		
}
*/





void spi1Send (unsigned char data)					
{
	//GPIO_ResetBits(GPIOA, GPIO_Pin_4); // CS low
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
  SPI_SendData8(SPI1, data);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait finish SPI1sending
	//GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high		
}






void spi1Write (unsigned char address, unsigned char data)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4); // CS low
		spi1Send(SPI1_WRITE_OP_CODE);		
		spi1Send(address);			
		spi1Send(data);			
		GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high		
}



unsigned char spi1Read(unsigned char address)
{
    unsigned char answer;
		unsigned int i;
		GPIO_ResetBits(GPIOA, GPIO_Pin_4); // CS low	
		spi1Send(SPI1_READ_OP_CODE);		
		spi1Send(address);			
		for (i = 0; i < 50; i++);	
		while (SPI_GetReceptionFIFOStatus(SPI1) != SPI_ReceptionFIFOStatus_Empty)			    // Read receiving FIFO until it is empty
		{	
			SPI_ReceiveData8(SPI1);
		}				
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
		}
    SPI_SendData8(SPI1, 0x00);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		{
		}
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
		}			
	  answer = SPI_ReceiveData8(SPI1);		
		GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high
    return answer;		
}







/*
unsigned char spi1Read(unsigned char address)
{
    unsigned char answer;
	
		
	
		spi1Send(SPI1_READ_OP_CODE);		
		spi1Send(address);			
		while (SPI_GetReceptionFIFOStatus(SPI1) != SPI_ReceptionFIFOStatus_Empty)			    // Read receiving FIFO until it is empty
		{	
			SPI_ReceiveData8(SPI1);
		}		
		GPIO_ResetBits(GPIOA, GPIO_Pin_4); // CS low	
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
		}
    SPI_SendData8(SPI1, 0x00);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		{
		}
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
		}			
		GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high
	  answer = SPI_ReceiveData8(SPI1);		
    return answer;		
}
*/




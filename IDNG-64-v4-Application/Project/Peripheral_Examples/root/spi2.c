		/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
		
		
		void spiIINCHIP_CSoff_m25p (void)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_12); 	// CS high	
			GPIO_ResetBits(GPIOB, GPIO_Pin_11); // CS high		
		}

		void spiIINCHIP_CSon_m25p (void)
		{
				GPIO_SetBits(GPIOB, GPIO_Pin_11); 	// CS high
				GPIO_SetBits(GPIOB, GPIO_Pin_12); 	// CS high	
		}
		
		void spiIINCHIP_CSoff_w5100 (void)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_11); 	// CS high	
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); // CS high		
		}

		void spiIINCHIP_CSon_w5100 (void)
		{
				GPIO_SetBits(GPIOB, GPIO_Pin_12); 	// CS high
				GPIO_SetBits(GPIOB, GPIO_Pin_11); 	// CS high
		}
		
		
		
		void spiIINCHIP_SpiSendData(unsigned char data)
		{
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
			SPI_SendData8(SPI2, data);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET); //wait finish sending			
		}
		
		
		
		
		
		uint8_t spiIINCHIP_SpiRecvData(void)
		{
				unsigned char result;
				unsigned int i;
		
				for (i = 0; i < 50; i++);	
			
				while (SPI_GetReceptionFIFOStatus(SPI2) != SPI_ReceptionFIFOStatus_Empty)			    // Read receiving FIFO until it is empty
				{	
					SPI_ReceiveData8(SPI2);
				}				

				while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
				{
				}

				SPI_SendData8(SPI2, 0x00);
				
				
				while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET)
				{
				}
				while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
				{
				}			
			  result = SPI_ReceiveData8(SPI2);		
				return result;			
		}

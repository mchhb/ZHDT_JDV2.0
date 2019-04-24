#ifndef _SD_CARD_H_
#define _SD_CARD_H_		 

#include "stm32f10x.h"
#include "spi.h"

// SPI CH choose
#define SD_USING_SPI2
//#define SD_USING_SPI1

// SPI SD_CS 
#define SD_CS_RCC RCC_APB2Periph_GPIOB
#define SD_CS_PORT GPIOB
#define SD_CS_PIN  GPIO_Pin_12
#define SD_CS PBout(12) 

// SPI�����ٶ����� 
#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  3
#define SPI_SPEED_32 	4
#define SPI_SPEED_64 	5
#define SPI_SPEED_128 6
#define SPI_SPEED_256 7

// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   






// SD��ָ���  	   
#define CMD0    0       // ����λ ����IDLE
#define CMD1    1
#define CMD8    8       // ����8 ��check sd_type sd_1.x (no_respond), sd_hc , sd_xc  
#define CMD9    9       // ����9 ����CSD����
#define CMD10   10      // ����10����CID����
#define CMD12   12      // ����12��ֹͣ���ݴ���
#define CMD16   16      // ����16������SectorSize Ӧ����0x00
#define CMD17   17      // ����17����sector
#define CMD18   18      // ����18����Multi sector
#define CMD23   23      // ����23�����ö�sectorд��ǰԤ�Ȳ���N��block

#define CMD24   24      // ����24��дsector
#define CMD25   25      // ����25��дMulti sector

#define CMD41   41      // ����41��Ӧ����0x00
#define CMD55   55      // ����55��Ӧ����0x01

#define CMD58   58      // ����58����OCR��Ϣ
#define CMD59   59      // ����59��ʹ��/��ֹCRC��Ӧ����0x00 spi_mode Ĭ�Ϲر�crc
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
//�ⲿ��Ӧ���ݾ�����������޸�!
				    	  


void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
void SD_DisSelect(void);
u8 SD_Select(void);
u8 SD_WaitReady(void);
u8 SD_GetResponse(u8 Response);
u8 SD_RecvData(u8*buf,u16 len);
u8 SD_SendBlock(u8*buf,u8 cmd);		  
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc);													   
u8 SD_GetCID(u8 *cid_data);								   
u8 SD_GetCSD(u8 *csd_data);										  


u32 SD_GetSectorCount(void);
u8 SD_Initialize(void);
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);
#endif





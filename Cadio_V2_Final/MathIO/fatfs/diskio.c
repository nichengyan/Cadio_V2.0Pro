/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "SD.h"
#include "W25QXX.h"



void ReadSector(uint32_t addr,uint8_t *dat){/*ÉÈÇøµØÖ·*/
    W25QXX_ReadPage(addr*4096+0*256,(unsigned char*)(dat+0*256));
    W25QXX_ReadPage(addr*4096+1*256,(unsigned char*)(dat+1*256)); 
    W25QXX_ReadPage(addr*4096+2*256,(unsigned char*)(dat+2*256));
    W25QXX_ReadPage(addr*4096+3*256,(unsigned char*)(dat+3*256)); 
    W25QXX_ReadPage(addr*4096+4*256,(unsigned char*)(dat+4*256));
    W25QXX_ReadPage(addr*4096+5*256,(unsigned char*)(dat+5*256)); 
    W25QXX_ReadPage(addr*4096+6*256,(unsigned char*)(dat+6*256));
    W25QXX_ReadPage(addr*4096+7*256,(unsigned char*)(dat+7*256)); 
    W25QXX_ReadPage(addr*4096+8*256,(unsigned char*)(dat+8*256));
    W25QXX_ReadPage(addr*4096+9*256,(unsigned char*)(dat+9*256)); 
    W25QXX_ReadPage(addr*4096+10*256,(unsigned char*)(dat+10*256));
    W25QXX_ReadPage(addr*4096+11*256,(unsigned char*)(dat+11*256)); 
    W25QXX_ReadPage(addr*4096+12*256,(unsigned char*)(dat+12*256));
    W25QXX_ReadPage(addr*4096+13*256,(unsigned char*)(dat+13*256)); 
    W25QXX_ReadPage(addr*4096+14*256,(unsigned char*)(dat+14*256));
    W25QXX_ReadPage(addr*4096+15*256,(unsigned char*)(dat+15*256));  
}  
/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();

		// translate the reslut code here

		//return stat;

		break;

	case DEV_MMC :
		//result = MMC_disk_status();

		// translate the reslut code here

		//return stat;

		break;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the reslut code here

		//return stat;

		break;
	}

	//return STA_NOINIT;
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_initialize();

		// translate the reslut code here

		//return stat;

		break;

	case DEV_MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		//return stat;

		//if (W25QXX_GetCapacity() != 255) return 0;
        //return SD_Init();
        //HardWareSPI_Init(0x00);
        return 0;
		break;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		//return stat;

		break;
	}

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		//return res;
		
		break;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		//return res;
        //SD_ReadDisk(buff,sector,count);
        while(count--){
            ReadSector(buff,sector);    
            sector++;
        }
        
		return RES_OK;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		//return res;

		break;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		//return res;

		break;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		//return res;

        //SD_WriteDisk(buff,sector,count);
		return RES_OK;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		//return res;

		break;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		//return res;

		break;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		//return res;

		return RES_OK;

	case DEV_USB :

		// Process of the command the USB drive

		//return res;

		break;
	}

	return RES_PARERR;
}


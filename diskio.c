/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "kernel.h"

/* Definitions of physical drive number for each drive */
#define DEV_FD		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_HD		1	/* Example: Map MMC/SD card to physical drive 1 */

DRESULT __bios_errcode_result(
	BYTE result
)
{
	switch(result) {
	// FDD and HDD
	case 0x00:
		return RES_OK;
	case 0x01:
		return RES_PARERR;
	case 0x02: case 0x04: case 0x09: case 0x10: case 0x20: case 0x40: case 0x80:
		return RES_ERROR;
	// FDD only
	case 0x03:
		return RES_WRPRT;
	case 0x06:
		return RES_NOTRDY;
	case 0x08:
		return RES_ERROR;
	// HDD only
	case 0x05:
		return RES_NOTRDY;
	case 0x07:
		return RES_PARERR;
	case 0x11: case 0xbb: case 0xcc: case 0xe0:
		return RES_ERROR;
	case 0xaa:
		return RES_NOTRDY;
	}
	
	return RES_ERROR;
}

DSTATUS __bios_errcode_status(
	BYTE result
)
{
	switch(result) {
	case 0x00:
		return 0;
	case 0x03:
		return STA_PROTECT;
	case 0x06:
		return STA_NODISK;
	default:
		return STA_NOINIT;
	}
}

struct __bios_rtc_bcd
{
	int d_cl,d_ch,d_dl,d_dh;
	int t_cl,t_ch,t_dl,t_dh;
};

struct __bios_rtc_bcd __bios_readrtc(
	void
)
{
	struct __bios_rtc_bcd rtc;
	BYTE ___cl,___ch,___dh,___dl;
	
	__asm "mov ah,0x02"
	__asm "int 0x1a"
	__asm "mov ___cl,cl"
	__asm "mov ___ch,ch"
	__asm "mov ___dh,dh"
	
	rtc.t_cl = ___cl;
	rtc.t_ch = ___ch;
	rtc.t_dh = ___dh;
	
	__asm "mov ah,0x04"
	__asm "int 0x1a"
	__asm "mov ___cl,cl"
	__asm "mov ___ch,ch"
	__asm "mov ___dh,dh"
	__asm "mov ___dl,dl"
	
	rtc.d_cl = ___cl;
	rtc.d_ch = ___ch;
	rtc.d_dh = ___dh;
	rtc.d_dl = ___dl;
	
	return rtc;
}

DWORD __bcd_to_int(
	DWORD bcd
)
{
	DWORD ret = 0;
	int i;
	for(i = 0; i < 8; i++) {
		int d;
		ret *= 10;
		d = (bcd >> ((7 - i) * 4)) & 0x0f;
		ret += d;
	}
	
	return ret;
}

DWORD get_fattime(
	void
)
{
	struct __bios_rtc_bcd rtc = __bios_readrtc();
	
	DWORD year = __bcd_to_int(rtc.d_ch << 8 | rtc.d_cl) - 1980;
	DWORD month = __bcd_to_int(rtc.d_dh);
	DWORD day = __bcd_to_int(rtc.d_dl);
	
	DWORD hour = __bcd_to_int(rtc.d_ch);
	DWORD minute = __bcd_to_int(rtc.d_cl);
	DWORD second = __bcd_to_int(rtc.d_dh) / 2;
	
	DWORD ret = (year << 25) | (month << 21) | (day << 16) | (hour << 11) | (minute << 5) | (second << 0);
	
	return ret;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	BYTE result;

	switch (pdrv) {
	case DEV_FD :
		return 0;

	case DEV_HD :
		return 0;

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	BYTE result;

	switch (pdrv) {
	case DEV_FD :
		result = init_drive(0x00);

		return __bios_errcode_status(result);

	case DEV_HD :
		result = init_drive(0x80);

		return __bios_errcode_status(result);

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	BYTE result;
	int i;

	switch (pdrv) {
	case DEV_FD :
		// translate the arguments here

		result = read_disk(0x00, sector, count, (char far *) 0x18000000);

		for(i = 0; i < count * 512; i++) buff[i] = ((char far *) 0x18000000)[i];

		// translate the reslut code here

		return __bios_errcode_result(result);

	case DEV_HD :
		// translate the arguments here

		result = read_disk(0x80, sector, count, (char far *) 0x18000000);

		for(i = 0; i < count * 512; i++) buff[i] = ((char far *) 0x18000000)[i];

		// translate the reslut code here

		return __bios_errcode_result(result);

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	BYTE result;
	int i;

	switch (pdrv) {
	case DEV_FD :
		// translate the arguments here

		for(i = 0; i < count * 512; i++) ((char far *) 0x18000000)[i] = buff[i];

		result = write_disk(0x00, sector, count, (char far *) 0x18000000);

		// translate the reslut code here

		return __bios_errcode_result(result);

	case DEV_HD :
		// translate the arguments here

		for(i = 0; i < count * 512; i++) ((char far *) 0x18000000)[i] = buff[i];

		result = write_disk(0x80, sector, count, (char far *) 0x18000000);

		// translate the reslut code here

		return __bios_errcode_result(result);

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	BYTE result;

	switch (pdrv) {
	case DEV_FD :

		// Process of the command for the RAM drive

		return res;

	case DEV_HD :

		// Process of the command for the MMC/SD card

		return res;

	}

	return RES_PARERR;
}


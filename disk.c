#include "kernel.h"

disk_addr sect2disk(int sector)
{
	disk_addr disk_address;
	
	disk_address.cyls = sector / 36;
	sector %= 36;
	disk_address.head = sector / 18;
	sector %= 18;
	disk_address.sect = sector + 1;
	
	return disk_address;
}

unsigned char init_drive(char __drive)
{
	unsigned char __result;
	unsigned short __eflags;
	
	__asm "mov ah,0x00"
	__asm "mov dl,__drive"
	__asm "int 0x13"
	
	__asm "push eflags"
	__asm "pop dx"
	__asm "mov __eflags,dx"
	__asm "mov __result,ah"
	
	if((__eflags & 1) == 0) __result = 0;
	
	return __result;
}

int disk_acsess(char drive, char mode, char size, 
	char cyls, char head, char sect,
	int reg_es, int reg_bx)
{
	char ret_val = 0;
	__asm "mov ah,mode"
	__asm "mov al,size"
	__asm "mov ch,cyls"
	__asm "mov cl,sect"
	__asm "mov dh,head"
	__asm "mov dl,drive"
	__asm "push ax"
	__asm "mov ax,reg_es"
	__asm "mov es,ax"
	__asm "pop ax"
	__asm "push bx"
	__asm "mov bx,reg_bx"
	__asm "int 0x13"
	__asm "pop bx"
	__asm "mov ret_val,ah"
	return (int)ret_val;
}

int read_disk(int drv, int sect, int size, char far *buf)
{
	disk_addr disk = sect2disk(sect);
	
	return disk_acsess(drv, 2, size, disk.cyls, disk.head, disk.sect, ((long)buf >> 16), (int)buf);
}

int write_disk(int drv, int sect, int size, char far *buf)
{
	disk_addr disk = sect2disk(sect);
	
	return disk_acsess(drv, 3, size, disk.cyls, disk.head, disk.sect, ((long)buf >> 16), (int)buf);
}

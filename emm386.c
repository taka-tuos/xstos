#include "kernel.h"

void emm386_writeb(u32 __addr, u8 __data)
{
	__asm "mov ecx,__addr"
	__asm "mov al,__data"
	__asm "mov [ecx],al"
}

void emm386_writew(u32 __addr, u16 __data)
{
	__asm "mov ecx,__addr"
	__asm "mov ax,__data"
	__asm "mov [ecx],ax"
}

void emm386_writed(u32 __addr, u32 __data)
{
	__asm "mov ecx,__addr"
	__asm "mov eax,__data"
	__asm "mov [ecx],eax"
}

u8 emm386_readb(u32 __addr)
{
	u8 __data;
	__asm "mov ecx,__addr"
	__asm "mov al,[ecx]"
	__asm "mov __data,al"
	return __data;
}

u16 emm386_readw(u32 __addr)
{
	u16 __data;
	__asm "mov ecx,__addr"
	__asm "mov ax,[ecx]"
	__asm "mov __data,ax"
	return __data;
}

u32 emm386_readd(u32 __addr)
{
	u32 __data;
	__asm "mov ecx,__addr"
	__asm "mov eax,[ecx]"
	__asm "mov __data,eax"
	return __data;
}

void emm386_readdata(void far *__buf, u32 __addr, u16 __size)
{
	u16 i;
	for(i = 0; i < __size; i++) {
		((u8 far *)__buf)[i] = emm386_readb(i + __addr);
	}
}

void emm386_readdata_near(void *__buf, u32 __addr, u16 __size)
{
	u16 i;
	for(i = 0; i < __size; i++) {
		((u8 *)__buf)[i] = emm386_readb(i + __addr);
	}
}

void emm386_writedata(void far *__buf, u32 __addr, u16 __size)
{
	u16 i;
	for(i = 0; i < __size; i++) {
		emm386_writeb(i + __addr,((u8 far *)__buf)[i]);
	}
}

void emm386_writedata_near(void *__buf, u32 __addr, u16 __size)
{
	u16 i;
	for(i = 0; i < __size; i++) {
		emm386_writeb(i + __addr,((u8 *)__buf)[i]);
	}
}


#include "kernel.h"

void farmemcpy(char far *p1, char far *p2, int size)
{
	for(;size != 0; size--) {
		*p1 = *p2;
		p1++;
		p2++;
	}
	
	return;
}

void farmemset(char far *p, int val, int size)
{
	for(;size != 0; size--) {
		*p = val;
		p++;
	}
	
	return;
}

int farstrncmp(char far *str1, char far *str2, int n)
{
	int ret = 0;
	for ( ; n > 0; n--, str1++, str2++)
		if ((ret = *str1 - *str2) || !*str1) break;
	return ret;
}

int farstrcmp(char far *str1, char far *str2)
{
	for (;;)
	{
		int ret = *str1 - *str2;
		if (ret || !*str1) return ret;
		str1++;
		str2++;
	}
}

void farstrcpy(char far *d, char far *s)
{
	char far *tmp = d;
	
	while ((*d++ = *s++) != '\0');
}

char getchar()
{
	char chr = 0;
	__asm "mov ah,0"
	__asm "int 0x16"
	__asm "mov chr,al"
	return chr;
}

char getchar_nowait()
{
	char chr = 0;
	unsigned int flag;
	
	__asm "mov ah,1"
	__asm "int 0x16"
	
	__asm "pushf"
	__asm "pop ax"
	__asm "mov flag,ax"
	if(!(flag & (1 << 6))) {
		__asm "mov ah,0"
		__asm "int 0x16"
		__asm "mov chr,al"
	}
	return chr;
}

void disable(void)
{
	__asm "cli"
}

void enable(void)
{
	__asm "sti"
}

void far *farmalloc(int size)
{
	struct MEMMAN far *memman = (struct MEMMAN far *) MEMMAN_ADDR;
	char far *p = (char far *)MK_FP(memman_alloc(memman, size + 16),0);
	if(p != 0) {
		*((int *) p) = size;
		p += 16;
	}
	return p;
}

void farfree(void far *p)
{
	struct MEMMAN far *memman = (struct MEMMAN far *) MEMMAN_ADDR;
	char far *q = (char far *)p;
	int size;
	if(q != 0) {
		q -= 16;
		size = *((long far *) q);
		memman_free(memman, FP_SEG(q), size + 16);
	}
}

void outp(int port, int data)
{
	__asm "mov cx,port"
	__asm "mov ax,data"
	__asm "out cx,ax"
}

void outb(int port, char data)
{
	__asm "mov cx,port"
	__asm "mov al,data"
	__asm "out cx,al"
}

int inp(int port)
{
	int data;
	__asm "mov cx,port"
	__asm "in ax,cx"
	__asm "mov data,ax"
	return data;
}

char inb(int port)
{
	char data;
	__asm "mov cx,port"
	__asm "in al,cx"
	__asm "mov data,al"
	return data;
}

void setvect(int n, void interrupt far (*func)(void))
{
	*((int far *)MK_FP(0,n*4+0)) = FP_OFF(func);
	*((int far *)MK_FP(0,n*4+2)) = FP_SEG(func);
}

void (far *getvect(int n))(void)
{
	return MK_FP(*((int far *)MK_FP(0,n*4+2)),*((int far *)MK_FP(0,n*4+0)));
}

unsigned long clock(void)
{
	unsigned int h16;
	unsigned int l16;
	__asm "mov ah,0"
	__asm "int 0x1a"
	__asm "mov h16,cx"
	__asm "mov l16,dx"
	return (h16 << 16) | l16;
}

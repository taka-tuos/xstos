#include "kernel.h"

extern struct CONSOLE cons;

void putchar8(int x, int y, char col, int c)
{
	int far *vram = (int far *)0xb8000000;
	
	vram[y * 80 + x] = c | (col << 8) | (0 << 12);
}

int checkline(int *x, int *y)
{
	int ret = 0;
	
	if(*x >= 80) {
		*x = 0;
		*y++;
		ret = 1;
	}
	if(*y >= 25) {
		*y = 24;
		memcpy((char far *) 0xb8000000, (char far *) 0xb80a0000, 80*25*2);
		ret = 1;
	}
	
	return ret;
}

void move_cur(int x, int y)
{
	short xy = y * 80 + x;
	
	__asm "mov ax,xy"
	__asm "push ax"
	__asm "mov dx,0x3d4"
	__asm "mov al,0x0e"
	__asm "out dx,ax"
	__asm "pop ax"
	__asm "mov ah,al"
	__asm "mov al,0x0f"
	__asm "out dx,ax"
}

void cons_putstr(char *str, char col)
{
	int x = cons.x;
	int y = cons.y;
	unsigned char *s = (unsigned char *)str;
	
	for(; *s != 0; s++) {
		if(*s == 0x0a) {
			x = 0;
			y++;
		} else if(*s == 0x0d) {
			continue;
		} else {
			putchar8(x, y, col, *s);
			x++;
		}
		checkline(&x, &y);
	}
	
	move_cur(x, y);
	
	cons.x = x;
	cons.y = y;
}

void cons_putchar(char c, char col)
{
	int x = cons.x;
	int y = cons.y;
	
	if(c == 0x0a) {
		x = 0;
		y++;
	} else if(c != 0x0d) {
		putchar8(x, y, col, c);
		x++;
	}
	checkline(&x, &y);
	
	move_cur(x, y);
	
	cons.x = x;
	cons.y = y;
}

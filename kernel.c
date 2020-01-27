#include "kernel.h"
#include "ff.h"
#include <stdlib.h>

struct CONSOLE cons;

struct MEMMAN far *memman = (struct MEMMAN far *) MEMMAN_ADDR;

void api_proc(void);

char hextbl[] = "0123456789abcdef";

FATFS *fatfs;

void exec_app(void);

void decode_hex(DWORD data, BYTE col);

extern void asm_api_proc();

void xstos_main()
{
	FATFS fs;
	FIL fp;
	FRESULT res;
	
	// <initmem>
	memman_init(memman);
	memman_free(memman, 0x2800, 0xa000 - 0x2800);
	memman_free(memman, 0xb800, 0x10000 - 0xb800);
	// </initmem>
	
	farmemset((char far *)0xb8000000, 0, 80*25*2);
	
	init_drive(0);
	
	cons.x = 0;
	cons.y = 0;
	
	//start_rtc(rtc_routine);
	
	cons_putstr("Xstos -git build date:"__DATE__"\n",7);
	cons_putstr("[kernel]:boot successed\n",2);
	
	__asm "push es"
	__asm "push ax"
	__asm "push bx"
	__asm "mov ax,0"
	__asm "mov es,ax"
	__asm "mov ax,asm_api_proc"
	__asm "mov bx,4*0x40"
	__asm "mov [es:bx],ax"
	__asm "mov ax,cs"
	__asm "mov bx,4*0x40+2"
	__asm "mov [es:bx],ax"
	__asm "pop bx"
	__asm "pop ax"
	__asm "pop es"
	
	cons_putstr("[kernel]:FatFS initalize...",2);
	
	res = f_mount(&fs,"",1);
	
	fatfs = &fs;
	
	if(!res) cons_putstr("done\n",2);
	else cons_putstr("failed\n",2);
	
	f_open(&fp,"FILETEST.TXT",FA_OPEN_ALWAYS | FA_WRITE);
	f_puts("書き込み・タイムスタンプテスト",&fp);
	f_close(&fp);
	
	exec_app();
	
	for(;;) {
		__asm "hlt"
	}
}

void decode_hex(DWORD data, BYTE col)
{
	int i;
	
	for(i = 0; i < 8; i++) cons_putchar(hextbl[(data >> ((7 - i) * 4)) & 0xf],col);
}

void exec_app(void)
{
	BYTE buff[512];
	FIL fp;
	UINT siz;
	UINT ptr = 0;
	int i;
	BYTE far *appseg = 0x60000000;
	
	BYTE r_chksum = 0;
	BYTE w_chksum = 0;
	
	f_open(&fp,"autoexec.bin",FA_OPEN_ALWAYS | FA_READ);
	while(!f_eof(&fp)) {
		f_read(&fp, buff, 512, &siz);
		for(i = 0; i < siz; i++) {
			appseg[ptr + i] = buff[i];
			r_chksum ^= buff[i];
		}
		ptr += siz;
	}
	
	for(i = 0; i < ptr; i++) w_chksum ^= appseg[i];
	
	sprintf(buff,"R=%02X W=%02X\n",r_chksum,w_chksum);
	cons_putstr(buff,2);
	
	f_close(&fp);
	
	__asm "call 0x6000,0x0000"
}

void api_proc(void)
{
	BYTE __ah,__al;
	WORD __bx,__si;
	
	__asm "mov __ah,ah"
	__asm "mov __al,al"
	__asm "mov __bx,bx"
	__asm "mov __si,si"
	
	switch(__ah) {
		case 0x01 : {
			char far *s = MK_FP(0x6000,__si);
			for(;*s;s++) cons_putchar(*s,__bx);
			break;
		}
		case 0x02 : {
			cons_putchar(__al,__bx);
			break;
		}
		case 0x06 : {
			__al = getchar();
			__asm "mov al,__al"
			__asm "mov 20[bp],al"
			break;
		}
	}
}

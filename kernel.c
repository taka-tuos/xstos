#include "kernel.h"
#include "ff.h"
#include <stdlib.h>

struct CONSOLE cons;
struct MEMMAN far *memman = (struct MEMMAN far *) MEMMAN_ADDR;
FATFS *fatfs;
FIL app_fil_hnds[16];

void api_proc(void);
void exec_app(void);
void decode_hex(DWORD data, BYTE col);
extern void asm_api_proc();

struct MEMMAN far *memman_app = (struct MEMMAN far *) MEMMAN_ADDR + sizeof(struct MEMMAN);

void xstos_main()
{
	FATFS fs;
	FIL fp;
	FRESULT res;
	
	// <initmem>
	memman_init(memman);
	memman_free(memman, 0x28000, 0x0a0000 - 0x028000);
	memman_free(memman, 0xb8000, 0x100000 - 0x0b8000);
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
	
	//f_open(&fp,"FILETEST.TXT",FA_OPEN_ALWAYS | FA_WRITE);
	//f_puts("書き込み・タイムスタンプテスト",&fp);
	//f_close(&fp);
	
	memset(app_fil_hnds,0xff,sizeof(app_fil_hnds));
	
	exec_app();
	
	for(;;) {
		__asm "hlt"
	}
}

void decode_hex(DWORD data, BYTE col)
{
	int i;
	
	for(i = 0; i < 8; i++) {
		int h = (data >> ((7 - i) * 4)) & 0xf;
		cons_putchar(h >= 10 ? h-10+'A' : h+'0',col);
	}
	cons_putchar(0x0a,col);
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
	
	__asm "push cx"
	__asm "mov cx,0x800"
	__asm "mov ds,cx"
	__asm "mov ss,cx"
	__asm "pop cx"
	
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
		
		case 0x03 : {
			__al = getchar();
			__asm "mov al,__al"
			__asm "mov 20[bp],al"
			break;
		}
		
		case 0x05 : {
			FIL *fp = &app_fil_hnds[__al];
			char fn[32];
			char far *p = MK_FP(0x6000,__si);
			char *q = fn;
			for(;*p;p++,q++) *q = *p;
			
			f_open(fp,fn,FA_OPEN_ALWAYS | FA_WRITE);
			break;
		}
		
		case 0x06 : {
			FIL *fp = &app_fil_hnds[__al];
			UINT i;
			//char buff[512];
			
			//char far *p = MK_FP(0x6000,__si);
			//char *q = buff;
			//for(;*p;p++,q++) *q = *p;
			
			
			//f_write(fp, "hoge", 4, &i);
			
			//for(;*p;p++) f_putc(*p,fp);
			break;
		}
		
		case 0x07 : {
			FIL *fp = &app_fil_hnds[__al];
			
			f_close(fp);
			break;
		}
	}
}

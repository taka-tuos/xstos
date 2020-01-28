/* kernel header */
#ifndef __OPKRH__
#define __OPKRH__
#define FP_SEG(__p) ((unsigned)((unsigned long)(void __far*)(__p) >> 16))
#define  FP_OFF(__p) ((unsigned)(__p))
#define MK_FP(__s,__o) (((unsigned short)(__s)):>((void __near *)(__o)))

typedef struct {
	short cyls;
	short head;
	short sect;
} disk_addr;

unsigned char init_drive(char __drive);
int disk_acsess(char drive, char mode, char size, 
	char cyls, char head, char sect,
	int reg_es, int reg_bx);
int read_disk(int drv, int sect, int size, char far *buf);
int write_disk(int drv, int sect, int size, char far *buf);

struct CONSOLE {
	int x, y;
};

void init_text(int __seg, int __off);
void move_cur(int x, int y);
void cons_putstr(char *str,char col);
void cons_putchar(char c, char col);

void farmemcpy(char far *p1, char far *p2, int size);
void farmemset(char far *p, int val, int size);
int farstrncmp(char far *str1, char far *str2, int n);
int farstrcmp(char far *str1, char far *str2);
void farstrcpy(char far *d, char far *s);
char getchar();
char getchar_nowait();
void disable(void);
void enable(void);
void far *farmalloc(int size);
void farfree(void far *p);
void outp(int port, int data);
void outb(int port, char data);
int inp(int port);
char inb(int port);
void setvect(int n, void interrupt far (*func)(void));
void (far *getvect(int n))(void);
unsigned long clock(void);

#define MEMMAN_FREES		2048	/* ‚±‚ê‚Å–ñ32KB */
#define MEMMAN_ADDR			0x18000000
struct FREEINFO {	/* ‚ ‚«î•ñ */
	unsigned long addr, size;
};
struct MEMMAN {		/* ƒƒ‚ƒŠŠÇ— */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
void memman_init(struct MEMMAN far *man);
unsigned long memman_total(struct MEMMAN far *man);
unsigned long memman_alloc(struct MEMMAN far *man, unsigned long size);
int memman_free(struct MEMMAN far *man, unsigned long addr, unsigned long size);

#define MEMMAN_FP(__p) MK_FP((__p) >> 4,(__p) & 15)
#define MEMMAN_CUTUP(__s) (((__s) + 15) & 0xfffffff0)

typedef unsigned char u8;
typedef unsigned int  u16;
typedef unsigned long u32;

/*
void emm386_writeb(u32 __addr, u8 __data);
void emm386_writew(u32 __addr, u16 __data);
void emm386_writed(u32 __addr, u32 __data);
u8 emm386_readb(u32 __addr);
u16 emm386_readw(u32 __addr);
u32 emm386_readd(u32 __addr);
void emm386_readdata(void far *__buf, u32 __addr, u16 __size);
void emm386_readdata_near(void *__buf, u32 __addr, u16 __size);
void emm386_writedata(void far *__buf, u32 __addr, u16 __size);
void emm386_writedata_near(void *__buf, u32 __addr, u16 __size);
*/
#endif

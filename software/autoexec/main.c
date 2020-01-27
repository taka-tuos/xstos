void puts(char *s)
{
	__asm "pusha"
	__asm "mov ah,0x01"
	__asm "mov si,s"
	__asm "mov bx,3"
	__asm "int 0x40"
	__asm "popa"
}

void xstos_main(void)
{
	puts("16color test");
	
	for(;;);
}

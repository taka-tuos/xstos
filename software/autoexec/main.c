void puts(char *s, int col)
{
	__asm "pusha"
	__asm "mov ah,0x01"
	__asm "mov si,s"
	__asm "mov bx,col"
	__asm "int 0x40"
	__asm "popa"
}

void open(char *s)
{
	__asm "pusha"
	__asm "mov ah,0x05"
	__asm "mov si,s"
	__asm "mov al,0"
	__asm "int 0x40"
	__asm "popa"
}

void write(char *s)
{
	__asm "pusha"
	__asm "mov ah,0x06"
	__asm "mov si,s"
	__asm "mov al,0"
	__asm "int 0x40"
	__asm "popa"
}

void close()
{
	__asm "pusha"
	__asm "mov ah,0x07"
	__asm "mov al,0"
	__asm "int 0x40"
	__asm "popa"
}

void xstos_main(void)
{
	unsigned char i = 7;
	
	open("test.txt");
	write("hogehogefugafuga");
	close();
	
	while(1) {
		puts("16color test\n",i);
		i++;
		i &= 15;
	}
	
	for(;;);
}

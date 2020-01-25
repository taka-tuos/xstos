; i-dos sample
; tab=4

.8086

_text	segment byte public 'CODE'

		org		0x0000

		;mov		ah,0x05		;int 0x40 ah=0x05:clear screen
		;int		0x40
		
		mov		ah,0x01		;int 0x40 ah=0x01,si=string address:put string
		mov		bx,0x0003
		mov		si,example
		int		0x40

loop_puts:
		mov		ah,0x06		;int 0x40 ah=0x06:get key
		int		0x40
		mov		ah,0x02		;int 0x40 ah=0x02:put char
		int		0x40
		;mov		bx,0x0003
		;hlt
		;mov		ah,0x01		;int 0x40 ah=0x01,si=string address:put string
		;add		bx,0x01
		;and		bx,0x0f
		;mov		si,example
		;int		0x40
		;mov		[ds:bx],byte 0xcc
		add		bx,0x0001
		jmp		loop_puts

example:
		db		"16color test"
		db		0x0d,0x0a

_text	ends
end

_TEXT	segment byte public 'CODE'

public	asm_api_proc_

extrn	api_proc_ : proc

asm_api_proc_ proc
 	pusha
 	
 	mov cx,cs
 	
 	push cx
 	
 	mov cx,0x800
 	mov es,cx
 	mov ss,cx
 	mov ds,cx
 	
	call api_proc_
	
	pop cx
	
	mov es,cx
 	mov ss,cx
 	mov ds,cx
	
	mov bx,sp
	mov [bx+14],ax
	
	popa
	iret
asm_api_proc_ endp

_TEXT	ends

end asm_api_proc_

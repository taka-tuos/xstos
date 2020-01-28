_TEXT	segment byte public 'CODE'

public	asm_api_proc_

extrn	api_proc_ : proc
asm_api_proc_ proc
 	pusha
 	
 	mov bp,sp
 	
 	mov cx,0x800	; change SS to kernel
 	mov ss,cx
 	
 	mov sp,0xfffe
 	
 	push bp			; save SP to kernel stack
 	
 	mov cx,ds
 	push cx			; save DS to kernel stack
 	
 	mov cx,ss
 	mov ds,cx		; change DS to kernel
 	
	call api_proc_
	
	pop cx
	
	pop bp			; restore SP from kernel stack
	
	mov ds,cx		; restore DS from kernel stack
 	mov ss,cx		; restore SS from kernel stack
 	
 	mov sp,bp
	
	mov bx,sp
	mov [bx+14],ax
	
	popa
	iret
asm_api_proc_ endp

_TEXT	ends

end asm_api_proc_

.8086

DGROUP group _TEXT,_DATA,_BSS,_STACK,

_TEXT	segment byte public 'CODE'

public	crtmain
extrn	kernel_main_ : proc
crtmain proc
	assume	ds:_TEXT
	mov		cx,cs
	mov		ds,cx
	
	assume  es:DGROUP
	mov		es,cx
	
	mov		bx,offset DGROUP:_end
	add		bx,0Fh
	and		bl,0F0h
	
	mov		ss,cx
	mov		sp,bx

	call	kernel_main_
crtmain endp

_TEXT	ends

STACK_SIZE equ 2000h

_STACK	segment word public 'STACK'
	db      (STACK_SIZE) dup(?)
	extrn	_end : byte
_STACK	ends

_DATA	segment byte public 'DATA'
_DATA	ends

_BSS	segment word public 'BSS'
_BSS	ends

end crtmain

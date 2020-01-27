.8086

DGROUP group _TEXT,_DATA,_BSS,_STACK

_TEXT	segment byte public 'CODE'

public	crtmain
extrn	xstos_main_ : proc
crtmain proc
	assume	ds:DGROUP
	mov		cx,cs
	mov		ds,cx
	
	assume  es:DGROUP
	mov		es,cx
	
	mov		bx,0xfffe
	
	mov		ss,cx
	mov		sp,bx

	call	xstos_main_
crtmain endp

_TEXT	ends

_STACK	segment word public 'STACK'
_STACK	ends

_DATA	segment byte public 'DATA'
_DATA	ends

_BSS	segment word public 'BSS'
_BSS	ends

end crtmain

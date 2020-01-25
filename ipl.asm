.8086

_text	segment byte public 'CODE'

secs	equ		128				; �ǂ��܂œǂݍ��ނ�

		org		0x7c00			; ���̃v���O�������ǂ��ɓǂݍ��܂��̂�

; �ȉ��͕W���I��fat12�t�H�[�}�b�g�t���b�s�[�f�B�X�N�̂��߂̋L�q

		jmp		entry
		db		0x90
		db		"OPENKRNL"		; �u�[�g�Z�N�^�̖��O�����R�ɏ����Ă悢�i8�o�C�g�j
		dw		512				; 1�Z�N�^�̑傫���i512�ɂ��Ȃ���΂����Ȃ��j
		db		1				; �N���X�^�̑傫���i1�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		dw		1				; FAT���ǂ�����n�܂邩�i���ʂ�1�Z�N�^�ڂ���ɂ���j
		db		2				; FAT�̌��i2�ɂ��Ȃ���΂����Ȃ��j
		dw		224				; ���[�g�f�B���N�g���̈�̑傫���i���ʂ�224�G���g���ɂ���j
		dw		2880			; ���̃h���C�u�̑傫���i2880�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		db		0xf0			; ���f�B�A�̃^�C�v�i0xf0�ɂ��Ȃ���΂����Ȃ��j
		dw		9				; FAT�̈�̒����i9�Z�N�^�ɂ��Ȃ���΂����Ȃ��j
		dw		18				; 1�g���b�N�ɂ����̃Z�N�^�����邩�i18�ɂ��Ȃ���΂����Ȃ��j
		dw		2				; �w�b�h�̐��i2�ɂ��Ȃ���΂����Ȃ��j
		dd		0				; �p�[�e�B�V�������g���ĂȂ��̂ł����͕K��0
		dd		2880			; ���̃h���C�u�傫����������x����
		db		0,0,0x29		; �悭�킩��Ȃ����ǂ��̒l�ɂ��Ă����Ƃ����炵��
		dd		0xffffffff		; ���Ԃ�{�����[���V���A���ԍ�
		db		"OPENKERNEL "	; �f�B�X�N�̖��O�i11�o�C�g�j
		db		"FAT12   "		; �t�H�[�}�b�g�̖��O�i8�o�C�g�j
		;resb	18				; �Ƃ肠����18�o�C�g�����Ă���
		db		18 dup(0)

; �v���O�����{��

entry:
		mov		ax,0			; ���W�X�^������
		mov		ss,ax
		mov		sp,0x7c00
		mov		ds,ax

; �f�B�X�N��ǂ�

		mov		ax,0x0800
		mov		es,ax
		mov		di,0
		mov		ch,0			; �V�����_0
		mov		dh,1			; �w�b�h0
		mov		cl,16			; �Z�N�^15
readloop:
		mov		si,0			; ���s�񐔂𐔂��郌�W�X�^
retry:
		mov		ah,0x02			; ah=0x02 : �f�B�X�N�ǂݍ���
		mov		al,1			; 1�Z�N�^
		mov		bx,0
		mov		dl,0x00			; a�h���C�u
		int		0x13			; �f�B�X�Nbios�Ăяo��
		jnc		next			; �G���[�������Ȃ����next��
		add		si,1			; si��1�𑫂�
		cmp		si,5			; si��5���r
		jae		error			; si >= 5 ��������error��
		mov		ah,0x00
		mov		dl,0x00			; a�h���C�u
		int		0x13			; �h���C�u�̃��Z�b�g
		jmp		retry
next:
		mov		ax,es			; �A�h���X��0x200�i�߂�
		add		ax,0x0020
		mov		es,ax			; add es,0x020 �Ƃ������߂��Ȃ��̂ł������Ă���
		add		di,1
		add		cl,1			; cl��1�𑫂�
		cmp		cl,18			; cl��18���r
		jbe		readloop		; cl <= 18 ��������readloop��
		mov		cl,1
		add		dh,1
		cmp		dh,2
		jb		readloop		; dh < 2 ��������readloop��
		mov		dh,0
		add		ch,1
		cmp		di,secs
		jb		readloop		; ch < cyls ��������readloop��

; �ǂݏI������̂�kernel.sys�����s���I

		jmp		0x0800,0x0000

error:
		mov		si,msg
		mov		bx,0x4
		call	puts_col
		jmp		fin
puts_col:
		mov		ax,ds
		mov		es,ax
		mov		di,si
		xor		cx,cx
		not		cx
		xor		al,al
		cld
		repne scasb
		not		cx
		dec		cx
		push	cx
		
		mov		ax,0x0300
		push	bx
		mov		bx,0x0000
		int		0x10
		pop		bx
		
		pop		cx
		mov		ax,0x1301
		mov		bp,si
		int		0x10
		ret
fin:
		hlt						; ��������܂�cpu���~������
		jmp		fin				; �������[�v
msg:
		db		0x0a, 0x0a		; ���s��2��
		db		"[ipl]Fatal : Cannot read floppy",0x0d,0x0a,"force reboot : Ctrl+Alt+Del"
		db		0

ffaa:
		org		0x7dfe
		db		0x55, 0xaa

_text	ends
end

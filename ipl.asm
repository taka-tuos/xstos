.8086

_text	segment byte public 'CODE'

secs	equ		128				; どこまで読み込むか

		org		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なfat12フォーマットフロッピーディスクのための記述

		jmp		entry
		db		0x90
		db		"OPENKRNL"		; ブートセクタの名前を自由に書いてよい（8バイト）
		dw		512				; 1セクタの大きさ（512にしなければいけない）
		db		1				; クラスタの大きさ（1セクタにしなければいけない）
		dw		1				; FATがどこから始まるか（普通は1セクタ目からにする）
		db		2				; FATの個数（2にしなければいけない）
		dw		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
		dw		2880			; このドライブの大きさ（2880セクタにしなければいけない）
		db		0xf0			; メディアのタイプ（0xf0にしなければいけない）
		dw		9				; FAT領域の長さ（9セクタにしなければいけない）
		dw		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
		dw		2				; ヘッドの数（2にしなければいけない）
		dd		0				; パーティションを使ってないのでここは必ず0
		dd		2880			; このドライブ大きさをもう一度書く
		db		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
		dd		0xffffffff		; たぶんボリュームシリアル番号
		db		"OPENKERNEL "	; ディスクの名前（11バイト）
		db		"FAT12   "		; フォーマットの名前（8バイト）
		;resb	18				; とりあえず18バイトあけておく
		db		18 dup(0)

; プログラム本体

entry:
		mov		ax,0			; レジスタ初期化
		mov		ss,ax
		mov		sp,0x7c00
		mov		ds,ax

; ディスクを読む

		mov		ax,0x0800
		mov		es,ax
		mov		di,0
		mov		ch,0			; シリンダ0
		mov		dh,1			; ヘッド0
		mov		cl,16			; セクタ15
readloop:
		mov		si,0			; 失敗回数を数えるレジスタ
retry:
		mov		ah,0x02			; ah=0x02 : ディスク読み込み
		mov		al,1			; 1セクタ
		mov		bx,0
		mov		dl,0x00			; aドライブ
		int		0x13			; ディスクbios呼び出し
		jnc		next			; エラーがおきなければnextへ
		add		si,1			; siに1を足す
		cmp		si,5			; siと5を比較
		jae		error			; si >= 5 だったらerrorへ
		mov		ah,0x00
		mov		dl,0x00			; aドライブ
		int		0x13			; ドライブのリセット
		jmp		retry
next:
		mov		ax,es			; アドレスを0x200進める
		add		ax,0x0020
		mov		es,ax			; add es,0x020 という命令がないのでこうしている
		add		di,1
		add		cl,1			; clに1を足す
		cmp		cl,18			; clと18を比較
		jbe		readloop		; cl <= 18 だったらreadloopへ
		mov		cl,1
		add		dh,1
		cmp		dh,2
		jb		readloop		; dh < 2 だったらreadloopへ
		mov		dh,0
		add		ch,1
		cmp		di,secs
		jb		readloop		; ch < cyls だったらreadloopへ

; 読み終わったのでkernel.sysを実行だ！

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
		hlt						; 何かあるまでcpuを停止させる
		jmp		fin				; 無限ループ
msg:
		db		0x0a, 0x0a		; 改行を2つ
		db		"[ipl]Fatal : Cannot read floppy",0x0d,0x0a,"force reboot : Ctrl+Alt+Del"
		db		0

ffaa:
		org		0x7dfe
		db		0x55, 0xaa

_text	ends
end

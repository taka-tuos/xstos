MAKE     = make -r
WASM     = wasm
WLINK    = wlink
COPY     = cp
DEL      = rm

TARGET   = autoexec
OBJS     = autoexec.obj

# デフォルト動作

default :
	$(MAKE) bin

# ファイル生成規則

%.obj : %.asm Makefile
	$(WASM) $(ASFLAGS) -fo=$*.obj $*.asm

%.obj : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.obj $*.c

$(TARGET).bin : $(OBJS) Makefile
	$(WLINK) $(patsubst %.obj, FILE %.obj, $(OBJS)) FORMAT RAW BIN NAME $(TARGET).bin LIBRARY ../golibc86/golibc86.lib

# コマンド

bin :
	$(MAKE) $(TARGET).bin

clean :
	-$(DEL) $(OBJS)

src_only :
	$(MAKE) clean
	-$(DEL) $(TARGET).bin

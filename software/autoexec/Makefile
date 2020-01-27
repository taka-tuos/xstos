MAKE     = make -r
WASM     = wasm
WCC      = wcc
WLINK    = wlink
COPY     = cp
DEL      = rm

TARGET   = autoexec
OBJS     = main.o

CFLAGS = -2 -s -wx -d0 -fr -ms -zl -i=../golibc86/include
ASFLAGS = -bt=dos -0r -mt

# デフォルト動作

default :
	$(MAKE) bin

# ファイル生成規則

%.o : %.s Makefile
	$(WASM) $(ASFLAGS) -fo=$*.o $*.s

%.o : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.o $*.c

$(TARGET).bin : $(OBJS) Makefile
	$(MAKE) -C ../icrt
	$(WLINK) FILE ../icrt/icrt0.o $(patsubst %.o, FILE %.o, $(OBJS)) FORMAT DOS COM NAME $(TARGET).bin LIBRARY ../golibc86/golibc86.lib

# コマンド

bin :
	$(MAKE) $(TARGET).bin

clean :
	-$(DEL) $(OBJS)

fclean :
	$(MAKE) clean
	-$(DEL) $(TARGET).bin

OBJS_KERNEL = kernel.o disk.o text.o ansi.o memory.o ff.o diskio.o asmfunc.o
LIB = software/golibc86/golibc86.lib

empty:=
space:= $(empty) $(empty)

MAKE     = make -r
WCC      = wcc
WASM     = wasm
WLINK    = wlink

MFORMAT  = mformat
MCOPY    = mcopy

COPY     = cp
DEL      = rm
DD       = dd

CFLAGS = -bt=dos -2 -s -wx -d0 -fr -ms -zl -i=./software//golibc86/include
ASFLAGS = -bt=dos -2r -mt

# デフォルト動作

default :
	$(MAKE) img

# ファイル生成規則

%.o : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.o $*.c

%.o : %.s Makefile
	$(WASM) $(ASFLAGS) -fo=$*.o $*.s

ipl.bin : ipl.o Makefile
	$(WLINK) FILE ipl.o NAME ipl.raw FORMAT RAW
	$(DD) if=ipl.raw of=ipl.bin skip=62

kernel.sys : $(OBJS_KERNEL) Makefile
	$(WLINK) FILE software/icrt/icrt0.o $(patsubst %.o, FILE %.o, $(OBJS_KERNEL)) FORMAT DOS COM NAME kernel.sys OPTION NODEFAULTLIBS, START=crtmain, MAP=kernel.map LIBRARY $(LIB)

xstos.img : ipl.bin kernel.sys software/autoexec/autoexec.bin Makefile
	$(MFORMAT) -f 1440 -C -B ipl.bin -i $$@ xstos.img
	$(MCOPY) -i xstos.img kernel.sys ::/
	$(MCOPY) -i xstos.img software/autoexec/autoexec.bin ::/

# コマンド

img :
	$(MAKE) -C software/icrt
	$(MAKE) -C software/autoexec
	$(MAKE) -C software/golibc86
	$(MAKE) xstos.img

clean :
	$(MAKE) -C software/icrt clean
	$(MAKE) -C software/autoexec clean
	$(MAKE) -C software/golibc86 clean
	-$(DEL) ipl.bin
	-$(DEL) ipl.raw
	-$(DEL) *.o
	-$(DEL) *.err
	-$(DEL) *.map
	

fclean :
	$(MAKE) -C software/icrt fclean
	$(MAKE) -C software/autoexec fclean
	$(MAKE) -C software/golibc86 fclean
	$(MAKE) clean
	-$(DEL) xstos.img
	-$(DEL) kernel.sys

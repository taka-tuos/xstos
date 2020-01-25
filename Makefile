OBJS_KERNEL = kernel.obj disk.obj text.obj ansi.obj memory.obj emm386.obj ff.obj diskio.obj
LIB = golibc86/golibc86.lib

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

CFLAGS = -bt=dos -3 -s -wx -d0 -fr -ms -zl -i=./golibc86/include
ASFLAGS = -bt=dos -0r -mt

# デフォルト動作

default :
	$(MAKE) img

# ファイル生成規則

%.obj : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.obj $*.c

%.obj : %.asm Makefile
	$(WASM) $(ASFLAGS) -fo=$*.obj $*.asm

ipl.bin : ipl.obj Makefile
	$(WLINK) FILE ipl.obj NAME ipl.raw FORMAT RAW
	$(DD) if=ipl.raw of=ipl.bin skip=62

kernel.sys : icrt0.obj $(OBJS_KERNEL) Makefile
	$(WLINK) FILE icrt0.obj $(patsubst %.obj, FILE %.obj, $(OBJS_KERNEL)) FORMAT DOS COM NAME kernel.sys OPTION NODEFAULTLIBS, START=crtmain, MAP=kernel.map $(subst $(space), LIBRARY , $(LIB))

openkernel.img : ipl.bin kernel.sys Makefile
	$(MFORMAT) -f 1440 -C -B ipl.bin -i $$@ xstos.img
	$(MCOPY) -i xstos.img kernel.sys ::/
	$(MCOPY) -i xstos.img sample/autoexec.bin ::/

# コマンド

img :
	$(MAKE) -C sample
	$(MAKE) -C golibc86
	$(MAKE) openkernel.img

clean :
	$(MAKE) -C sample clean
	$(MAKE) -C golibc86 clean
	-$(DEL) ipl.bin
	-$(DEL) ipl.raw
	-$(DEL) *.obj
	-$(DEL) *.err
	-$(DEL) *.map
	

src_only :
	$(MAKE) -C sample src_only
	$(MAKE) -C golibc86 src_only
	$(MAKE) clean
	-$(DEL) xstos.img
	-$(DEL) kernel.sys

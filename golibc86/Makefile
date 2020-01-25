TOOLPATH = ../../z_tools/
MAKE     = make -r
WCC      = wcc
WASM     = wasm
WLIB     = wlib
COPY     = cp
DEL      = rm

OBJS     = abs.obj atoi.obj errno.obj memchr.obj memcmp.obj memcpy.obj \
           memmove.obj memset.obj qsort.obj rand.obj sprintf.obj strcat.obj \
           strchr.obj strcmp.obj strcpy.obj strcspn.obj strlen.obj strncat.obj \
           strncmp.obj strncpy.obj strpbrk.obj strrchr.obj strspn.obj strstr.obj \
           strtol.obj strtoul.obj strtoul0.obj vsprintf.obj i4d.obj i4m.obj

CFLAGS   = -bt=dos -3 -s -wx -d0 -fr -ms -zl -i=./include
ASFLAGS  = -bt=dos -0r -mt

# デフォルト動作

default :
	$(MAKE) lib

# ファイル生成規則

%.obj : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.obj $*.c

%.obj : %.asm Makefile
	$(WASM) $(ASFLAGS) -fo=$*.obj $*.asm

golibc86.lib : $(OBJS) Makefile
	$(WLIB) golibc86.lib $(patsubst %.obj,+%.obj,$(OBJS))

# コマンド

lib :
	$(MAKE) golibc86.lib

clean :
	-$(DEL) *.obj

src_only :
	$(MAKE) clean
	-$(DEL) golibc86.lib

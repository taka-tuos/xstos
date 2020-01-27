TOOLPATH = ../../z_tools/
MAKE     = make -r
WCC      = wcc
WASM     = wasm
WLIB     = wlib
COPY     = cp
DEL      = rm

OBJS     = abs.o atoi.o errno.o memchr.o memcmp.o memcpy.o \
           memmove.o memset.o qsort.o rand.o sprintf.o strcat.o \
           strchr.o strcmp.o strcpy.o strcspn.o strlen.o strncat.o \
           strncmp.o strncpy.o strpbrk.o strrchr.o strspn.o strstr.o \
           strtol.o strtoul.o strtoul0.o vsprintf.o i4d.o i4m.o

CFLAGS   = -bt=dos -3 -s -wx -d0 -fr -ms -zl -i=./include
ASFLAGS  = -bt=dos -0r -mt -i=./include/asm

# デフォルト動作

default :
	$(MAKE) lib

# ファイル生成規則

%.o : %.c Makefile
	$(WCC) $(CFLAGS) -fo=$*.o $*.c

%.o : %.s Makefile
	$(WASM) $(ASFLAGS) -fo=$*.o $*.s

golibc86.lib : $(OBJS) Makefile
	$(WLIB) golibc86.lib $(patsubst %.o,+%.o,$(OBJS))

# コマンド

lib :
	$(MAKE) golibc86.lib

clean :
	-$(DEL) *.o

fclean :
	$(MAKE) clean
	-$(DEL) golibc86.lib

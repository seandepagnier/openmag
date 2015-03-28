LIBNAME=tinygl
SRCS= api.c list.c vertex.c init.c matrix.c texture.c \
      misc.c clear.c light.c clip.c select.c get.c error.c \
      zbuffer.c zline.c zdither.c ztriangle.c \
      zmath.c image_util.c oscontext.c msghandling.c \
      arrays.c specbuf.c memory.c glu.c

INCPREFIX = ../../

CFLAGS = -I.. -fno-builtin

WARNS = -Wno-endif-labels

NOAVRLIB=1

include ../../make/multilib.mk

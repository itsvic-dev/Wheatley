PROJECT_NAME := Kernel
TARGET_NAME := kernel.elf
SRC := main.c io.c printf.c \
	drivers/fb/fb.c drivers/fb/efifb.c \
	drivers/tty/tty.c drivers/tty/serialtty.c \

PRECOMPILED_OBJ := kernel.ld

# define PROJECT_NAME, SRC and TARGET_NAME before including cproj.mak!!!
include $(BS_ROOT)/cproj.mak

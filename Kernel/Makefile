PROJECT_NAME := Kernel
TARGET_NAME := kernel.elf
SRC := main.c idt.c io.c libk.c panic.c printf.c \
	drivers/fb/fb.c drivers/fb/efifb.c \
	drivers/tty/tty.c \
	drivers/tty/fbtty.c drivers/tty/serialtty.c \
	mm/init.c mm/internal.c mm/mm.c \

CFLAGS := -fstack-protector

PRECOMPILED_OBJ := kernel.ld $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)/Tamsyn8x16r.o

$(OUTPUT_DIRECTORY)/$(PROJECT_NAME)/%.o: %.psf
	@echo -e "OBJCOPY ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@objcopy -O elf64-x86-64 -B i386 -I binary $< $@

# define PROJECT_NAME, SRC and TARGET_NAME before including cproj.mak!!!
include $(BS_ROOT)/cproj.mak

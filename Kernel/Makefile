PROJECT_NAME := Kernel
TARGET_NAME := kernel.elf

LAI_SRC := core/error.c core/eval.c core/exec-operand.c \
	core/exec.c core/libc.c core/ns.c core/object.c \
	core/opregion.c core/os_methods.c core/variable.c \
	core/vsnprintf.c helpers/pc-bios.c helpers/pci.c \
	helpers/resource.c helpers/sci.c helpers/pm.c \
	drivers/ec.c drivers/timer.c \

SRC := main.c libk.c panic.c printf.c \
	fw/acpi.c fw/apic.c fw/laihost.c \
	drivers/fb/fb.c drivers/fb/efifb.c \
	drivers/timer/apic.c drivers/timer/hpet.c \
	drivers/tty/tty.c \
	drivers/tty/fbtty.c drivers/tty/serialtty.c \
	mm/init.c mm/internal.c mm/mm.c \
	sys/cpuid.c sys/gdt.asm sys/gdt.c \
	sys/idt.c sys/io.c sys/isr.asm sys/isr.c sys/pci.c \
	$(addprefix ../../ThirdParty/lai/,$(LAI_SRC)) \

CFLAGS := -fstack-protector -I$(PROJECT_ROOT)/ThirdParty/lai/include -Wno-ignored-attributes

PRECOMPILED_OBJ := kernel.ld $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)/Tamsyn8x16r.o

$(OUTPUT_DIRECTORY)/$(PROJECT_NAME)/%.o: %.psf
	@echo -e "OBJCOPY ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@objcopy -O elf64-x86-64 -B i386 -I binary $< $@

# define PROJECT_NAME, SRC and TARGET_NAME before including cproj.mak!!!
include $(BS_ROOT)/cproj.mak

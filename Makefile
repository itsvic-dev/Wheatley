# ComatOS makefile

PROJECT_ROOT := $(shell pwd)
BS_ROOT := $(PROJECT_ROOT)/BuildSystem

include $(BS_ROOT)/defines.mak
export

all: bootloader kernel
.PHONY: all

bootloader:
	@echo -e "   MAKE (Bootloader)"
	@$(MAKE) -C Bootloader build

kernel:
	@echo -e "   MAKE (Kernel)"
	@$(MAKE) -C Kernel build

clean:
	rm -rf build
.PHONY: clean

include $(BS_ROOT)/qemu.mak

# ComatOS makefile

PROJECT_ROOT := $(shell pwd)
BS_ROOT := $(PROJECT_ROOT)/BuildSystem

include $(BS_ROOT)/defines.mak
export

all: bootloader
.PHONY: all

bootloader:
	@echo -e "MAKE\tBootloader"
	@$(MAKE) -C Bootloader build

clean:
	rm -rf build
.PHONY: clean

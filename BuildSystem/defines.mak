CC := clang
GLOBAL_CFLAGS := -g -Wall -Werror \
	-I$(PROJECT_ROOT)/Includes \
	-ffreestanding -mno-red-zone \
	-nostdlib -MD

GLOBAL_LDFLAGS := -fuse-ld=lld -nostdlib

OUTPUT_DIRECTORY := $(PROJECT_ROOT)/build

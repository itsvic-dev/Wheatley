CC := clang
GLOBAL_CFLAGS := -g -Wall -Werror -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -nostdlib
GLOBAL_LDFLAGS := -fuse-ld=lld -nostdlib

OUTPUT_DIRECTORY := $(PROJECT_ROOT)/build

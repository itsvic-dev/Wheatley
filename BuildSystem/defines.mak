CC := clang
GLOBAL_CFLAGS := -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -nostdlib -fno-pie -fno-pic
GLOBAL_LDFLAGS := -fuse-ld=lld -nostdlib

OUTPUT_DIRECTORY := $(PROJECT_ROOT)/build

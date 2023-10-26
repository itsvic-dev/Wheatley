#!/bin/bash
clang-format -i `find Kernel Bootloader -iname '*.c' -or -iname '*.h'`

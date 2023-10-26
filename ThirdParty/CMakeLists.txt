add_library(efiHeaders INTERFACE)
target_include_directories(
    efiHeaders INTERFACE efi
)

add_library(lai STATIC
    lai/core/error.c lai/core/eval.c lai/core/exec-operand.c
    lai/core/exec.c lai/core/libc.c lai/core/ns.c lai/core/object.c
    lai/core/opregion.c lai/core/os_methods.c lai/core/variable.c
    lai/core/vsnprintf.c lai/helpers/pc-bios.c lai/helpers/pci.c
    lai/helpers/resource.c lai/helpers/sci.c lai/helpers/pm.c
    lai/drivers/ec.c lai/drivers/timer.c
)
target_include_directories(lai PUBLIC lai/include)

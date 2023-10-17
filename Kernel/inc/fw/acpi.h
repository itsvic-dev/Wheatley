#ifndef __KERNEL_FW_ACPI_H
#define __KERNEL_FW_ACPI_H

#include <stdint.h>
#include <acpispec/tables.h> // from LAI

void acpi_init(void);

bool acpi_checksum(acpi_header_t *header);
acpi_header_t *acpi_find_table(char *sig, size_t index);

#endif // __KERNEL_FW_ACPI_H

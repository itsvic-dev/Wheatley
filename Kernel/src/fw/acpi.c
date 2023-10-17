#include <fw/acpi.h>
#include <stdint.h>
#include <libk.h>
#include <printf.h>
#include <panic.h>
#include <lai/core.h>
#include <lai/helpers/sci.h>
#include <assert.h>
#include <sys/pci.h>

bool has_xsdt = false;
acpi_xsdt_t *xsdt = NULL;
uint8_t revision;

void acpi_init(void) {
    char oemID[7];
    memcpy(oemID, g_handoff->rsdp.oemID, 6);
    oemID[6] = 0;
    revision = g_handoff->rsdp.revision;

    printf("acpi: revision %d, OEM: %s\n", g_handoff->rsdp.revision, oemID);
    if (g_handoff->rsdp.revision > 0) {
        has_xsdt = true;
        xsdt = (acpi_xsdt_t *)g_handoff->rsdp.xsdtAddress;
        assert(acpi_checksum(&xsdt->header));
        printf("acpi: using XSDT @ %#llx\n", xsdt);
    } else {
        panic("support for RSDT isn't here yet", 0);
    }

    pci_init();

    lai_set_acpi_revision(revision);
    lai_create_namespace();
    lai_enable_acpi(1);
}

bool acpi_checksum(acpi_header_t *header) {
    uint8_t sum = 0;

    for (int i = 0; i < header->length; i++)
        sum += ((uint8_t *)header)[i];

    return sum == 0;
}

acpi_header_t *acpi_find_table(char *sig, size_t index) {
    if (memcmp(sig, "DSDT", 4) == 0) {
        // get the DSDT from the FADT
        acpi_fadt_t *fadt = (acpi_fadt_t *)acpi_find_table("FACP", 0);
        acpi_header_t *ptr = (acpi_header_t *)fadt->x_dsdt;
        if (acpi_checksum(ptr))
            return ptr;
        return NULL;
    }

    size_t entries = (xsdt->header.length - sizeof(acpi_header_t)) / 8;
    int count = 0;

    for (size_t i = 0; i < entries; i++) {
        acpi_header_t *ptr = (acpi_header_t *)(xsdt->tables[i]);

        if (
            !memcmp(ptr->signature, sig, 4)
            && acpi_checksum(ptr)
            && count++ == index
        ) {
            return ptr;
        }
    }

    return NULL;
}

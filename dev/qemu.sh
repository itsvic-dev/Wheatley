#!/bin/bash
# Runs Wheatley in QEMU.

ROOT="$(realpath $(dirname $0)/..)"
BUILD="$ROOT/build"
IMG="$BUILD/Wheatley.img"

if [ -e "$ROOT/.env" ] ; then
    . "$ROOT/.env"
fi

# ensure the IMG exists
"$ROOT"/dev/fat32.sh

# ensure that OVMF path is set
: "${OVMF_PATH:=/usr/share/edk2-ovmf}"

exec qemu-system-x86_64 \
    -M q35 "${EXTRA_QEMU_ARGS[@]}" \
    -m 128M -cpu host -enable-kvm -serial stdio \
    -smp sockets=1,cores=4,threads=1 \
    -drive if=pflash,readonly=on,file="${OVMF_PATH}/OVMF_CODE.fd" \
    -hda "$IMG" "$@"

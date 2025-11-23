#!/bin/sh
#
# Copyright 2025 Erdem Ersoy (eersoy93)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

# Run erdemOS userspace program using host system's Linux kernel in QEMU

# Find the host's Linux kernel
KERNEL=$(ls /boot/vmlinuz-* 2>/dev/null | sort -V | tail -n1)
if [ -z "$KERNEL" ]; then
    echo "Error: No Linux kernel found in /boot/vmlinuz-*"
    exit 1
fi

echo "Using kernel: $KERNEL"

# Path to initramfs
INITRAMFS="output/initramfs.cpio.gz"
if [ ! -f "$INITRAMFS" ]; then
    echo "Error: Initramfs not found at $INITRAMFS"
    echo "Please run ./build.sh first."
    exit 1
fi

# Launch QEMU with the host kernel and our initramfs
# rdinit=/bin/init tells kernel to use our init from initramfs
# quiet suppresses most kernel boot messages
# vga=791 sets 1024x768 16-bit color mode with Unicode support
sudo qemu-system-x86_64 \
    -kernel "$KERNEL" \
    -initrd "$INITRAMFS" \
    -append "rdinit=/bin/init quiet vga=785" \
    -m 256M

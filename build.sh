#!/usr/bin/env bash
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

set -euo pipefail

# Build erdemOS init as a simple Linux userspace program and create initramfs

ROOT_DIR=$(cd "$(dirname "$0")" && pwd)
cd "$ROOT_DIR"

# Configuration
CC=${CC:-gcc}
SRC_DIR=src
OUTPUT_DIR=output
INITRAMFS_DIR="$OUTPUT_DIR/initramfs"
CFLAGS="-Wall -Wextra -O2 -static"

# Outputs
OUTPUT_INIT="$OUTPUT_DIR/init"
OUTPUT_ERSH="$OUTPUT_DIR/ersh"
OUTPUT_POWEROFF="$OUTPUT_DIR/poweroff"
INITRAMFS_FILE="$OUTPUT_DIR/initramfs.cpio.gz"

# Helper functions
info() { echo "[$(date +%H:%M:%S)] $*"; }
die() { echo "Error: $*" >&2; exit 1; }
require_cmd() { command -v "$1" >/dev/null 2>&1 || die "Required command not found: $1"; }

# Dependency checks
for cmd in "$CC" cpio gzip; do
    require_cmd "$cmd"
done

# Create output directory
mkdir -p "$OUTPUT_DIR"

info "[1/4] Compiling init userspace program (static)"
"$CC" $CFLAGS "$SRC_DIR/init.c" -o "$OUTPUT_INIT"

info "[2/4] Compiling ersh shell (static)"
"$CC" $CFLAGS "$SRC_DIR/ersh.c" -o "$OUTPUT_ERSH"

info "[3/4] Compiling poweroff utility (static)"
"$CC" $CFLAGS "$SRC_DIR/poweroff.c" -o "$OUTPUT_POWEROFF"

info "[4/4] Creating initramfs with all binaries"
# Clean and create initramfs directory
rm -rf "$INITRAMFS_DIR"
mkdir -p "$INITRAMFS_DIR/bin"

# Copy binaries
cp "$OUTPUT_INIT" "$INITRAMFS_DIR/bin/init"
cp "$OUTPUT_ERSH" "$INITRAMFS_DIR/bin/ersh"
cp "$OUTPUT_POWEROFF" "$INITRAMFS_DIR/bin/poweroff"
chmod +x "$INITRAMFS_DIR/bin/init"
chmod +x "$INITRAMFS_DIR/bin/ersh"
chmod +x "$INITRAMFS_DIR/bin/poweroff"

# Package into initramfs
cd "$INITRAMFS_DIR"
find . | cpio -o -H newc 2>/dev/null | gzip > "$ROOT_DIR/$INITRAMFS_FILE"
cd "$ROOT_DIR"

# Clean up temporary initramfs directory
rm -rf "$INITRAMFS_DIR"

info "Build complete:"
info "  Init binary:     $OUTPUT_INIT"
info "  Ersh binary:     $OUTPUT_ERSH"
info "  Poweroff binary: $OUTPUT_POWEROFF"
info "  Initramfs:       $INITRAMFS_FILE"

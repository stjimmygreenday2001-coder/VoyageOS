#!/bin/bash
# VoyageOS Build Script
# A helper script to build the OS and manage development

set -e

VESION="0.1.0"
BUILD_DIR="build"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Helper functions
info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

success() {
    echo -e "${GREEN}✓${NC} $1"
}

error() {
    echo -e "${RED}✗${NC} $1"
    exit 1
}

warn() {
    echo -e "${YELLOW}⚠${NC} $1"
}

# Check dependencies
check_deps() {
    info "Checking dependencies..."
    
    for cmd in nasm gcc ld; do
        if ! command -v "$cmd" &> /dev/null; then
            error "Required tool not found: $cmd"
        fi
    done
    
    success "All dependencies found"
}

# Clean build
do_clean() {
    info "Cleaning build artifacts..."
    rm -rf "$BUILD_DIR"
    success "Clean complete"
}

# Build bootloader
do_bootloader() {
    info "Building bootloader..."
    
    mkdir -p "$BUILD_DIR"
    nasm -f bin bootloader/bootloader.asm -o "$BUILD_DIR/boot.bin"
    
    success "Bootloader built: $BUILD_DIR/boot.bin"
}

# Build kernel
do_kernel() {
    info "Building kernel..."
    
    mkdir -p "$BUILD_DIR"
    
    # Compile C kernel
    gcc -ffreestanding -fno-stack-protector -fno-builtin -m64 \
        -Wall -Wextra -c kernel/kernel.c -o "$BUILD_DIR/kernel.o"
    
    # Link kernel
    ld -nostdlib -Ttext 0x10000 "$BUILD_DIR/kernel.o" -o "$BUILD_DIR/kernel.bin"
    
    success "Kernel built: $BUILD_DIR/kernel.bin"
}

# Build IDE
do_ide() {
    info "Building VoyageIDE..."
    
    mkdir -p "$BUILD_DIR"
    
    gcc -ffreestanding -fno-stack-protector -fno-builtin -m64 \
        -Wall -Wextra -c voyageide/ide.c -o "$BUILD_DIR/ide.o"
    
    success "VoyageIDE built: $BUILD_DIR/ide.o"
}

# Build compiler
do_compiler() {
    info "Building VoyageC compiler..."
    
    mkdir -p "$BUILD_DIR"
    
    gcc -Wall -Wextra -O2 compiler/voyagec.c -o "$BUILD_DIR/voyagec"
    
    success "VoyageC compiler built: $BUILD_DIR/voyagec"
}

# Full build
do_all() {
    info "Building VoyageOS v$VERSION..."
    do_bootloader
    do_kernel
    do_ide
    do_compiler
    success "VoyageOS build complete!"
}

# Run in QEMU
do_run() {
    if [ ! -f "$BUILD_DIR/boot.bin" ]; then
        warn "Bootloader not built, building now..."
        do_bootloader
    fi
    
    info "Launching VoyageOS in QEMU..."
    qemu-system-x86_64 -fda "$BUILD_DIR/boot.bin" -monitor stdio
}

# Show help
show_help() {
    cat << EOF
VoyageOS Build System v$VERSION

Usage: $0 [command]

Commands:
  all          Build entire VoyageOS
  bootloader   Build bootloader only
  kernel       Build kernel only
  ide          Build VoyageIDE
  compiler     Build VoyageC compiler
  run          Build and run in QEMU
  clean        Remove build artifacts
  check        Check dependencies
  help         Show this help message

Examples:
  $0 all              # Build everything
  $0 run              # Build and run in QEMU
  $0 clean            # Clean build

EOF
}

# Main
main() {
    case "${1:-all}" in
        all)
            check_deps
            do_all
            ;;
        bootloader)
            check_deps
            do_bootloader
            ;;
        kernel)
            check_deps
            do_kernel
            ;;
        ide)
            check_deps
            do_ide
            ;;
        compiler)
            check_deps
            do_compiler
            ;;
        run)
            check_deps
            do_all
            do_run
            ;;
        clean)
            do_clean
            ;;
        check)
            check_deps
            ;;
        help)
            show_help
            ;;
        *)
            error "Unknown command: $1"
            show_help
            ;;
    esac
}

main "$@"

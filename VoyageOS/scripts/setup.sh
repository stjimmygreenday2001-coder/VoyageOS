#!/bin/bash
# VoyageOS Quick Start Script
# Get a new VoyageOS developer up and running

set -e

echo "╔════════════════════════════════════════════════╗"
echo "║     VoyageOS Development Environment Setup     ║"
echo "╚════════════════════════════════════════════════╝"
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
    echo "✓ Detected: Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
    echo "✓ Detected: macOS"
else
    echo "✗ Unsupported OS: $OSTYPE"
    exit 1
fi

# Check for package manager
if [ "$OS" = "linux" ]; then
    if command -v apt-get &> /dev/null; then
        PKG_MGR="apt-get"
        echo "✓ Detected package manager: apt-get"
    elif command -v dnf &> /dev/null; then
        PKG_MGR="dnf"
        echo "✓ Detected package manager: dnf"
    else
        echo "✗ No supported package manager found"
        exit 1
    fi
elif [ "$OS" = "macos" ]; then
    if command -v brew &> /dev/null; then
        PKG_MGR="brew"
        echo "✓ Detected package manager: brew"
    else
        echo "✗ Homebrew not installed. Please install from https://brew.sh"
        exit 1
    fi
fi

echo ""
echo "Installing dependencies..."
echo ""

# Install dependencies
if [ "$OS" = "linux" ]; then
    if [ "$PKG_MGR" = "apt-get" ]; then
        echo "Running: sudo apt-get install -y nasm gcc binutils make qemu-system-x86"
        sudo apt-get update
        sudo apt-get install -y nasm gcc binutils make qemu-system-x86 2>/dev/null || {
            echo "Note: Some packages may already be installed"
        }
    elif [ "$PKG_MGR" = "dnf" ]; then
        echo "Running: sudo dnf install -y nasm gcc binutils make qemu-kvm"
        sudo dnf install -y nasm gcc binutils make qemu-kvm 2>/dev/null || {
            echo "Note: Some packages may already be installed"
        }
    fi
elif [ "$OS" = "macos" ]; then
    echo "Running: brew install nasm gcc binutils make qemu"
    brew install nasm gcc binutils make qemu 2>/dev/null || {
        echo "Note: Some packages may already be installed"
    }
fi

echo ""
echo "Verifying installation..."
echo ""

# Verify tools
for tool in nasm gcc ld make; do
    if command -v "$tool" &> /dev/null; then
        VERSION=$($tool --version 2>&1 | head -n1)
        echo "✓ $tool - $VERSION"
    else
        echo "✗ $tool - NOT FOUND"
    fi
done

echo ""
echo "╔════════════════════════════════════════════════╗"
echo "║          Setup Complete! Ready to Build        ║"
echo "╚════════════════════════════════════════════════╝"
echo ""
echo "Next steps:"
echo ""
echo "  1. Navigate to VoyageOS directory:"
echo "     cd /home/jasmine/Desktop/VoyageOS"
echo ""
echo "  2. Build the OS:"
echo "     make"
echo "     or"
echo "     bash scripts/build.sh all"
echo ""
echo "  3. Run in QEMU:"
echo "     make run"
echo "     or"
echo "     bash scripts/build.sh run"
echo ""
echo "  4. Read documentation:"
echo "     cat docs/DEVELOPMENT.md"
echo "     cat docs/ARCHITECTURE.md"
echo ""
echo "Happy coding! 🚀"

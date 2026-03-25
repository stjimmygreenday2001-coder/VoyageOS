#!/bin/bash
# VoyageOS Run Script
# Launch VoyageOS in QEMU with separate window and audio support

set -e

BUILD_DIR="build"

# Check if build exists
if [ ! -f "$BUILD_DIR/floppy.img" ]; then
    echo "Error: $BUILD_DIR/floppy.img not found. Please run 'make floppy' first."
    exit 1
fi

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║             Launching VoyageOS in QEMU                         ║"
echo "║          (Separate window with audio enabled)                 ║"
echo "║          Press Ctrl+Alt+Q or close window to quit             ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""
echo "Configuration:"
echo "  - Display: Separate GTK/SDL window"
echo "  - Audio: PulseAudio (QEMU PC Speaker)"
echo "  - Memory: 256 MB"
echo "  - Boot: Floppy disk"
echo ""
echo "Booting..."
echo ""

# Try different display/audio combinations
# 1. GTK display with PulseAudio (best on Linux with audio)
echo "Attempting: GTK display with PulseAudio..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    -display gtk \
    -audiodev pa,id=audio0 \
    -machine pcspk-audiodev=audio0 \
    -name "VoyageOS" \
    2>/dev/null && exit 0

# 2. SDL display with PulseAudio
echo "Attempting: SDL display with PulseAudio..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    -display sdl \
    -audiodev pulse,id=audio0 \
    -machine pcspk-audiodev=audio0 \
    -name "VoyageOS" \
    2>/dev/null && exit 0

# 3. GTK display with ALSA audio
echo "Attempting: GTK display with ALSA audio..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    -display gtk \
    -audiodev alsa,id=audio0 \
    -machine pcspk-audiodev=audio0 \
    -name "VoyageOS" \
    2>/dev/null && exit 0

# 4. GTK display without audio
echo "Attempting: GTK display (audio disabled)..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    -display gtk \
    -audiodev none,id=noaudia \
    -machine pcspk-audiodev=noaudia \
    -name "VoyageOS" \
    2>/dev/null && exit 0

# 5. SDL display without audio
echo "Attempting: SDL display (audio disabled)..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    -display sdl \
    2>/dev/null && exit 0

# 6. Basic fallback
echo "Using basic display mode..."
qemu-system-x86_64 \
    -fda "$BUILD_DIR/floppy.img" \
    -m 256 \
    2>/dev/null || echo "QEMU failed to start"

exit 0

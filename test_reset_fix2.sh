#!/bin/bash
# Test script for improved OpenOCD reset sequence

set -e

WORKSPACE="/home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux"
cd "$WORKSPACE"

echo "================================================"
echo "Testing Improved OpenOCD Reset Sequence"
echo "================================================"
echo ""

# Check if ELF file exists
if [ ! -f "build/GD32F303RC.elf" ]; then
    echo "ERROR: ELF file not found. Run 'cmake --build build' first."
    exit 1
fi

echo "[Step 1] Starting OpenOCD with improved config..."
openocd -f gd32f303_flash.cfg > /tmp/openocd.log 2>&1 &
OPENOCD_PID=$!
echo "OpenOCD PID: $OPENOCD_PID"
sleep 3

echo "✓ OpenOCD started (PID: $OPENOCD_PID)"
echo ""

echo "[Step 2] Running GDB test sequence..."
echo ""

# Run GDB with complete test sequence
arm-none-eabi-gdb -batch -ex "set pagination off" -ex "set architecture arm" -ex "target remote localhost:3333" \
  -ex "monitor reset init" \
  -ex "file build/GD32F303RC.elf" \
  -ex "load" \
  -ex "info registers pc" \
  -ex "break main" \
  -ex "info break" \
  -ex "info registers pc" \
  -ex "c" \
  -ex "quit" 2>&1 | tee /tmp/gdb_result.log

echo ""
echo "[Step 3] Cleanup..."
kill $OPENOCD_PID 2>/dev/null || true
sleep 1

echo ""
echo "================================================"
echo "Test Complete"
echo "================================================"
echo ""
echo "Full GDB output saved to: /tmp/gdb_result.log"
echo ""
echo "KEY OBSERVATIONS:"
echo "1. Check if 'continue' command hit the breakpoint at main"
echo "2. Look for 'Breakpoint 1, main' message"
echo "3. If PC stays at 0x8007xxx after reset = SUCCESS (Flash execution)"
echo "4. If PC goes to 0x1ffff = FAILED (still in bootloader)"
echo ""

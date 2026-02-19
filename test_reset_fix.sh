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
sleep 2

# Check if OpenOCD started successfully
if ! kill -0 $OPENOCD_PID 2>/dev/null; then
    echo "ERROR: OpenOCD failed to start. Log:"
    cat /tmp/openocd.log | head -20
    exit 1
fi
echo "✓ OpenOCD started (PID: $OPENOCD_PID)"
echo ""

# GDB test script
GDB_SCRIPT=$(cat <<'EOF'
set pagination off
set architecture arm

target remote localhost:3333
monitor reset init

echo \n=== Vector Table Check ===\n
x/4xw 0x8000000

echo \n=== Flash Load Check ===\n
file build/GD32F303RC.elf
load

echo \n=== PC After Load ===\n
info registers pc

echo \n=== Setting Breakpoint ===\n
break main
info break

echo \n=== PC Status ===\n
info registers pc

echo \n=== Running Program ===\n
continue
echo "Program should hit breakpoint now..."

quit
EOF
)

echo "[Step 2] Running GDB test sequence..."
echo "GDB commands:"
echo "  target remote localhost:3333"
echo "  monitor reset init"
echo "  file build/GD32F303RC.elf"
echo "  load"
echo "  break main"
echo "  continue"
echo ""

# Run GDB
arm-none-eabi-gdb -batch -ex "set pagination off" -ex "set architecture arm" <<'GDBEOF'
target remote localhost:3333
monitor reset init

echo
echo === Vector Table Check ===
x/4xw 0x8000000

echo

echo === Loading Program ===
file build/GD32F303RC.elf
load

echo

echo === PC After Load ===
info registers pc

echo

echo === Setting Breakpoint at main ===
break main
info break

echo

echo === Current PC ===
info registers pc

echo

echo === Attempting Continue ===
continue

echo

quit
GDBEOF

echo ""
echo "[Step 3] Cleanup..."
kill $OPENOCD_PID 2>/dev/null || true
sleep 1

echo ""
echo "================================================"
echo "Test Complete"
echo "================================================"
echo ""
echo "KEY OBSERVATIONS:"
echo "1. After 'reset init': Check if PC is in Flash (0x08xxx) or ROM (0x1ffff)"
echo "2. Vector table should show Reset_Handler at 0x08007971"
echo "3. Breakpoint should be set at main (around 0x8007336)"
echo "4. 'continue' should run the program to main()"
echo ""

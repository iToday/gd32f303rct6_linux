# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

GD32F303RCT6 embedded development project using CMake on Linux for cross-compilation to ARM Cortex-M4.

- **MCU**: GD32F303RCT6 (ARM Cortex-M4F, 256KB Flash, 48KB RAM)
- **Build System**: CMake with cross-compilation to arm-none-eabi
- **RTOS**: FreeRTOS (heap_4 memory management, 1000Hz tick rate)
- **GUI**: LVGL with stress demo
- **Debug**: OpenOCD + J-Link SWD

## Build Commands

```bash
# Configure for Debug build
cmake -DCMAKE_BUILD_TYPE=Debug -B build

# Build all targets
cmake --build build --target all

# Clean and rebuild
rm -rf build && cmake -DCMAKE_BUILD_TYPE=Debug -B build && cmake --build build

# Flash to device via OpenOCD
openocd -f gd32f303_flash.cfg -c "program build/GD32F303RC.bin verify reset 0x08000000" -c "shutdown"
```

VS Code tasks provide shortcuts:
- `Ctrl+Shift+B` → Build Project (CMake)
- `Build & Flash` → Build and flash in one step

## Build Types

| Type | Flags | Description |
|------|-------|-------------|
| Debug | `-Og -g3 -DDEBUG` | Minimal optimization, full debug symbols |
| Release | `-O3` | Maximum speed optimization |
| RelWithDebInfo | `-O2 -g` | Optimized with debug info |
| MinSizeRel | `-Os` | Size optimization |

## Project Structure

```
App/                  # Application layer - main entry and task definitions
├── main.c           # System initialization, calls lvgl_demo()
├── lvgl_demo.c      # FreeRTOS task setup (Start, LV Demo, LED tasks)
├── FreeRTOSConfig.h # FreeRTOS configuration (10KB heap, 32 priorities)
└── sys/, delay/, usart/  # System utilities

CMSIS/                # ARM Cortex-M CMSIS files

Drivers/              # Hardware abstraction layer
├── LCD/             # Display driver
├── TOUCH/           # Touch controller
├── IIC/             # I2C bus
├── 24CXX/           # EEPROM driver
└── KEY/, LED/, TIMER/

FWLIB/                # GD32F30x standard peripheral library

Middlewares/
├── FreeRTOS/        # RTOS (portable/GCC/ARM_CM4F, heap_4.c)
└── LVGL/            # GUI library with porting examples

Startup/              # Official GD32 startup files
gcc_startup/          # GCC-specific startup (startup_gd32f30x_hd.S)

build/                # Build output (GD32F303RC.elf/.hex/.bin)
```

## FreeRTOS Task Architecture

Tasks are created in `App/lvgl_demo.c`:

1. **Start Task** (Priority 1, 128 words) - Creates other tasks, then deletes itself
2. **LV Demo Task** (Priority 3, 1024 words) - Runs `lv_demo_stress()` and `lv_timer_handler()` every 5 ticks
3. **LED Task** (Priority 4, 128 words) - Toggles LED every 1000 ticks

The start task is created first, scheduler started, then start task creates the LV and LED tasks.

## Key Configuration Files

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Build configuration, compiler flags, source file selection |
| `GD32F303RC.ld` | Linker script (256K Flash @ 0x08000000, 48K RAM @ 0x20000000) |
| `gd32f303_flash.cfg` | OpenOCD config (J-Link SWD, 4MHz adapter speed) |
| `App/FreeRTOSConfig.h` | RTOS configuration |
| `.vscode/launch.json` | Debug configuration (auto-build, flash, stop at main) |

## Debugging

- Press `F5` in VS Code to start debugging
- Uses "OpenOCD Debug (GD32F303RCT6)" configuration
- Automatically builds and flashes, then stops at `main()`
- SWD pins: SWDIO (PA13), SWCLK (PA14)

## CMake Build Details

The build uses several exclusions to avoid duplicate compilation:
- `lcd_ex.c` is excluded because it's `#include`'d from `lcd.c`
- `portable/RVDS/*` excluded (Keil-specific, not GCC compatible)
- `MemMang/heap_[^4].c` excluded (only heap_4.c is used)

Hardware FPU is enabled: `-mfpu=fpv4-sp-d16 -mfloat-abi=hard`

## Memory Layout

- Flash: 0x08000000, 256KB total
- RAM: 0x20000000, 48KB total
- Stack: 4KB (configurable via `__stack_size` in linker script)
- System heap: 4KB minimum
- FreeRTOS heap: 10KB (configTOTAL_HEAP_SIZE in FreeRTOSConfig.h)

## Important Notes

- Source files are added via GLOB_RECURSE, not explicitly listed
- Duplicate files like `Drivers/LED/*.*` are commented out to avoid conflicts with `App/led.c`
- The project generates `compile_commands.json` for IDE IntelliSense
- Section garbage collection is enabled (`-Wl,-gc-sections`)
- Memory usage is printed during build

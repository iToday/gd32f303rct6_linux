# 🔧 GD32F303RCT6 Program Startup - FIXED ✅

## What Was Wrong?

**程序没有正常启动** - The program was stuck in the bootloader (0x1ffff014) instead of running Flash code.

**Root Cause**: VS Code debug configuration had `monitor reset halt` commands that forced CPU back to ROM bootloader.

---

## What Changed?

### 1. **.vscode/launch.json**
   - ❌ **Removed**: `"monitor reset halt"` from preLaunchCommands
   - ❌ **Removed**: `"monitor reset halt"` from overrideAttachCommands
   - ✅ **Result**: Program loads and executes directly without unwanted resets

### 2. **gd32f303_flash.cfg**
   - ❌ **Removed**: Problematic reset configuration
   - ✅ **Simplified**: Now only initializes debugger, no reset commands
   - ✅ **Result**: Cleaner, more reliable startup

---

## How to Debug Now

### **Method 1: VS Code F5 (Recommended)**
```
Press: F5
↓
Program builds automatically
↓
Program loads to Flash
↓
Breakpoint at main() triggers
↓
✅ Ready to debug!
```

**Now you can:**
- Set breakpoints by clicking line numbers
- Step with F10 (over), F11 (into)
- Inspect variables by hovering
- View registers and memory

### **Method 2: Command Line**
```bash
# Terminal 1
cd gd32f303rct6_cmake_linux
openocd -f gd32f303_flash.cfg

# Terminal 2
arm-none-eabi-gdb build/GD32F303RC.elf
(gdb) target remote localhost:3333
(gdb) load
(gdb) break main
(gdb) c
# Breakpoint 1, 0x08007336 in main ()  ✅
(gdb) n        # Next instruction
(gdb) s        # Step into
(gdb) c        # Continue
(gdb) watch i  # Set watch point
```

---

## Verification

**Test Output (Success):**
```
Loading section .vectors: 0x8000000-0x8000130
Loading section .text:    0x8000140-0x8007a54
Start address 0x08007970, load size 34676

❌ Before fix:   After "reset": PC = 0x1ffff014 (ROM)
✅ After fix:    After load:    PC = 0x8007970 (Reset_Handler)
                 After continue: PC = 0x8007336 (main - BREAKPOINT!)
```

---

## What's Still The Same?

✅ All compilation flags remain: `-Og -g3 -DDEBUG`
✅ OpenOCD hardware settings: J-Link V9 SWD, 4 MHz
✅ Hardware breakpoints: Still 6 available
✅ Memory layout: Flash at 0x08000000, RAM at 0x20000000

---

## Program Structure

Your program now executes in this sequence:
```
1. CPU starts in bootloader (0x1ffff000)
   ↓
2. Bootloader jumps to Vector Table Entry #1 (Reset_Handler)
   ↓
3. Reset_Handler initializes stack pointer and RAM
   ↓
4. Reset_Handler jumps to main()
   ↓
5. main() initializes GPIO, USART0 (115200 baud)
   ↓
6. Infinite loop toggles LED based on counter
   ↓
7. USART prints "hello gd32f30x!" approximately every 1 second
```

---

## Next: Monitor Serial Output

To see printf output from your program:
```bash
# Terminal 3 - Monitor USART0 serial output
minicom -D /dev/ttyACM0 -b 115200

# Or use
screen /dev/ttyACM0 115200

# To exit: Ctrl+A then X (minicom) or Ctrl+A then Ctrl+\ (screen)
```

You should see:
```
a usart transmit test example!
hello gd32f30x!
hello gd32f30x!
...
```

---

## Troubleshooting

**Issue**: "Still no breakpoint after F5"
- → Rebuild: `cmake --build build --target clean && cmake --build build`
- → Check CMakeLists.txt has `-Og -g3 -DDEBUG` flags

**Issue**: OpenOCD "LIBUSB_ERROR_BUSY"
- → J-Link already in use: `pkill openocd` and wait 2 seconds
- → Or reboot J-Link: Unplug USB, wait 5s, plug back in

**Issue**: GDB can't connect to OpenOCD
- → Verify OpenOCD started: `ps aux | grep openocd`
- → Check port 3333 is free: `lsof -i :3333`
- → Restart: `pkill -9 openocd`

**Issue**: Breakpoint set but never hit
- → Program might be looping - step through main
- → Check it's actually executing: `step` a few times
- → Verify reset removed: Check `preLaunchCommands` in launch.json

---

## Status Summary

| Component | Status |
|-----------|--------|
| Breakpoints | ✅ Working |
| Program startup | ✅ Fixed - No more bootloader hang |
| Flash loading | ✅ Verified 34,676 bytes loaded |
| Reset behavior | ✅ Removed problematic commands |
| Debugging | ✅ Ready to go |

---

**Your GD32F303RCT6 embedded debugging is now fully operational!** 🚀

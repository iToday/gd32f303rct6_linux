# GD32F303RCT6 OpenOCD 在线调试配置指南

## 环境要求

- ✅ OpenOCD 已安装 (version 0.11.0)
- ✅ Cortex-Debug VS Code 扩展已安装
- ✅ arm-none-eabi-gcc 工具链已安装
- ✅ CMake 已配置

## 支持的调试器

项目已配置 **J-Link** 调试器，配置文件：`gd32f303_flash.cfg`

### 支持的接口

- **SWD** (推荐) - 占用引脚少，调试速度快
- **JTAG** - 传统调试接口

### 更换调试器

如需使用其他调试器，编辑 `gd32f303_flash.cfg`：

```cfg
# J-Link (当前配置)
source [find interface/jlink.cfg]

# ST-Link V2/V3
# source [find interface/stlink.cfg]

# CMSIS-DAP
# source [find interface/cmsis-dap.cfg]

# Raspberry Pi GPIO (开源调试方案)
# source [find interface/raspberrypi-gpio.cfg]
```

## 硬件连接

### SWD 连接（推荐）

```
J-Link          GD32F303RCT6
─────────────────────────────
SWDIO    <-->   PA13 (JTMS)
SWCLK    <-->   PA14 (JTCK)
GND      <-->   GND
VCC      <-->   VCC (3.3V)
```

### JTAG 连接

```
J-Link          GD32F303RCT6
─────────────────────────────
TCO      <-->   PA15 (JTDI)
TCI      <-->   PB3  (JTDO)
TMS      <-->   PA13 (JTMS)
TCK      <-->   PA14 (JTCK)
GND      <-->   GND
VCC      <-->   VCC (3.3V)
```

## 在 VS Code 中调试

### 方式 1: 启动调试会话

1. 使用快捷键 **F5** 或点击 Run > Start Debugging
2. 选择 **"OpenOCD Debug (GD32F303RCT6)"**
3. 调试会自动构建项目并启动调试

### 方式 2: 附加到运行的设备

1. 确保设备已通过 OpenOCD 连接
2. 使用快捷键 **F5** 并选择 **"OpenOCD Debug (Attach)"**
3. 这将附加到已运行的程序

## 可用任务

在终端 (Ctrl+`) 执行以下任务：

| 任务 | 命令 | 说明 |
|------|------|------|
| Build Project (CMake) | `Build Project (CMake)` | 构建项目（默认任务） |
| CMake: Configure Debug | Debug 配置编译 | 配置 Debug 版本 |
| CMake: Configure Release | Release 配置编译 | 配置 Release 版本 |
| Clean Build | 清理构建目录 | 删除 build 文件夹 |
| Flash Program (OpenOCD) | 使用 OpenOCD 烧写 | 直接烧写到程序存储区 |

### 快速命令

```bash
# 构建项目
Press Ctrl+Shift+B

# 启动调试
Press F5

# 手动编译和烧写
cd /path/to/project
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build --target all
openocd -f gd32f303_flash.cfg -c "program build/GD32F303RC.bin verify reset 0x08000000" -c "shutdown"
```

## 调试技巧

### 断点

- **设置断点**: 在代码行边距点击
- **条件断点**: 右键点击断点 > Edit Breakpoint
- **日志点**: 用于记录变量而不暂停执行

### 查看

- **变量**: 在 Debug 面板的 Variables 标签页查看
- **内存**: 使用 Cortex-Debug Memory View (插件提供)
- **寄存器**: 在 Debug 面板查看 CPU 寄存器状态
- **反汇编**: 在 Disassembly 视图查看汇编代码

### 调试速度

如果调试太慢，在 `gd32f303_flash.cfg` 中调整速度：

```cfg
adapter speed 4000      # 当前: 4 MHz
adapter speed 1000      # 更保守: 1 MHz
adapter speed 10000     # 更快: 10 MHz (需要良好的硬件连接)
```

## 常见问题

### 问题: "信息: Unable to match any debug components"

**解决方案:**
1. 检查硬件连接（SWD/JTAG 线）
2. 检查电源连接（VCC 和 GND）
3. 在 `gd32f303_flash.cfg` 中尝试切换 SWD/JTAG 协议
4. 降低适配器速度

### 问题: "Error: Flash operation failed"

**解决方案:**
1. 确保目标设备已连接
2. 检查 Flash 地址是否正确 (0x08000000)
3. 尝试使用 `flash erase_sector` 命令
4. 检查 Flash 是否被保护

### 问题: "Timeout during connect"

**解决方案:**
1. 检查 USB 连接
2. 重启 OpenOCD
3. 检查调试器固件版本
4. 尝试 `reset_config`

## OpenOCD 命令行参考

### 手动启动 OpenOCD

```bash
# 启动 OpenOCD 服务器
openocd -f gd32f303_flash.cfg

# 在另一个终端连接并执行命令
telnet localhost 4444

# 常用 OpenOCD 命令
reset halt           # 复位并停止
step                 # 单步执行
continue             # 继续执行
flash list           # 列出 Flash 信息
flash erase_all      # 擦除所有 Flash
flash banks          # 列出 Flash 库
```

## GDB 远程连接

如果不使用 VS Code，可以用 GDB 远程连接：

```bash
# 启动 OpenOCD
openocd -f gd32f303_flash.cfg

# 另一个终端中启动 GDB
arm-none-eabi-gdb build/GD32F303RC.elf

# 在 GDB 中连接
(gdb) target remote localhost:3333
(gdb) load
(gdb) continue
(gdb) step
```

## 更多资源

- [OpenOCD 官方文档](http://openocd.org/doc/html/)
- [Cortex-Debug 项目](https://github.com/Marus/cortex-debug)
- [GD32F303 数据手册](https://www.gigadevice.com/)
- [STM32 OpenOCD 配置](https://github.com/openocd-org/openocd/tree/master/tcl/target)

## 配置文件说明

### `.vscode/launch.json`
- **OpenOCD Debug (GD32F303RCT6)**: 启动调试会话，自动编译
- **OpenOCD Debug (Attach)**: 附加到已运行的程序

### `.vscode/tasks.json`
包含所有可用的构建和调试任务

### `gd32f303_flash.cfg`
OpenOCD 配置文件，指定调试器接口、传输协议和目标芯片

### `.vscode/c_cpp_properties.json`
IntelliSense 配置，包含头文件路径和编译器设置

# GD32F303RCT6 OpenOCD 在线调试配置指南

## 📋 环境配置状态

✅ **已完成的配置：**
- OpenOCD 配置文件 (`gd32f303_flash.cfg`)
- VS Code Cortex-Debug 扩展
- CMake 调试符号生成 (`-g3` 标志)
- `compile_commands.json` 符号链接
- C/C++ IntelliSense 配置

---

## 🔧 软件依赖检查

### 必需工具
```bash
# 检查 ARM 工具链
arm-none-eabi-gcc --version

# 检查 OpenOCD
openocd --version

# 检查 Python (可选，用于 OpenOCD 脚本)
python3 --version
```

### VS Code 扩展
- ✅ C/C++ (ms-vscode.cpptools)
- ✅ CMake Tools (ms-vscode.cmake-tools)
- ✅ Cortex-Debug (mcu-debug.cortex-debug)

---

## 🎯 快速开始调试

### 1️⃣ **编译项目**
```bash
# 方式 1: 使用 VS Code 任务
Ctrl+Shift+B  # 执行默认构建任务

# 方式 2: 命令行
cmake --build build --target all
```

### 2️⃣ **启动調試**

**选项 A：从头开始调试（重新烧写程序）**
1. 按 `F5` 或点击运行按钮
2. 选择 **"OpenOCD Debug (GD32F303RCT6)"**
3. 程序将自动：
   - 编译最新代码
   - 重新烧写到芯片
   - 在 `main()` 函数处停止

**选项 B：连接到已运行的程序**
1. 按 `F5` → 选择 **"OpenOCD Debug (Attach)"**
2. 连接到正在运行的程序，不重新烧写

---

## 🐛 调试操作

### 基本快捷键
| 功能 | 快捷键 | 说明 |
|------|--------|------|
| 继续执行 | `F5` | 运行至下一个断点或程序结束 |
| 暂停 | `F6` | 暂停程序执行 |
| 单步执行 | `F10` | 执行单条语句（不进入函数） |
| 单步进入 | `F11` | 执行单条语句（进入函数） |
| 单步退出 | `Shift+F11` | 执行至函数返回 |
| 重启调试 | `Ctrl+Shift+F5` | 重新开始调试会话 |

### 设置断点
- **点击设置**：在代码行号前左键点击红点
- **快捷键**：`F9` 在当前行切换断点
- **条件断点**：右键断点 → "编辑断点" → 设置条件

### 查看变量
- **变量窗口**：侧边栏 → "变量" 面板
- **悬停查看**：鼠标悬停在变量上
- **监视表达式**：在 "监视" 面板添加自定义表达式

---

## 📌 调试配置详解

### launch.json 配置

**Launch 模式（推荐用于新调试）**
```jsonc
{
  "name": "OpenOCD Debug (GD32F303RCT6)",
  "type": "cortex-debug",
  "request": "launch",           // 启动新的调试会话
  "executable": "build/GD32F303RC.elf",
  "servertype": "openocd",
  "preLaunchTask": "Build Project (CMake)",  // 自动编译
  "runToMain": true,             // 运行到 main() 函数
  "stopOnEntry": false           // 不在程序入口处停止
}
```

**Attach 模式（用于连接到运行程序）**
```jsonc
{
  "name": "OpenOCD Debug (Attach)",
  "type": "cortex-debug",
  "request": "attach",           // 连接到运行中的程序
  "executable": "build/GD32F303RC.elf"
}
```

---

## 🔌 硬件连接

### J-Link 调试器连接
```
J-Link 引脚          GD32F303RCT6 引脚
─────────────────────────────────────
VCC (20) ────────→  3.3V
GND (4,6,8,10)  →  GND
TCK (9) ────────→  PA14 (TCK/SWCLK)
TDI (5) ────────→  PA15 (TDI)
TDO (13) ───────→  PB3 (TDO)
TMS (7) ────────→  PA13 (TMS/SWDIO)
RESET (15) ─────→  NRST
```

### OpenOCD 配置文件位置
- **文件位置**：`gd32f303_flash.cfg`
- **支持的调试器**：
  - J-Link (当前配置)
  - ST-Link V2/V3
  - CMSIS-DAP
  - 树莓派 GPIO

**切换调试器**（编辑 `gd32f303_flash.cfg`）：
```cfg
# 当前使用 J-Link
source [find interface/jlink.cfg]

# 或切换为 ST-Link
# source [find interface/stlink.cfg]

# 或切换为 CMSIS-DAP
# source [find interface/cmsis-dap.cfg]
```

---

## 📊 常用调试场景

### 场景 1：查看 USART 输出
```
1. 设置断点在 printf() 调用处
2. 单步执行观察硬件状态
3. 在 "变量" 面板查看 USART 寄存器值
```

### 场景 2：调试 LED 闪烁逻辑
```
1. 在 main.c 的 LED 控制代码设置断点
2. F5 启动调试
3. 查看 i 变量值和 LED 状态
4. F10 单步执行看 LED 状态变化
```

### 场景 3：检查初始化顺序
```
1. Ctrl+Shift+D 打开调试视图
2. 设置断点在每个初始化函数处
3. F5 启动，观察调用栈和变量初始化过程
```

---

## 🚨 常见问题排查

### 问题：连接失败 "Unable to open remote"

**解决方案：**
```bash
# 1. 检查 OpenOCD 服务是否正常启动
lsof -i :3333  # 检查端口 3333 是否被占用

# 2. 检查硬件连接
usb-devices | grep -i jlink

# 3. 检查权限
ls -la /dev/ttyUSB*

# 4. 重新插拔调试器
# 或运行 OpenOCD 测试
openocd -f gd32f303_flash.cfg
```

### 问题：符号读取失败 "No debugging symbols found"

**解决方案：**
```bash
# 检查 ELF 文件是否包含调试符号
arm-none-eabi-objdump -h build/GD32F303RC.elf | grep debug

# 如果没有，重新配置构建（Debug 模式）
cmake -DCMAKE_BUILD_TYPE=Debug -B build
```

### 问题："sys.h" 文件找不到

**解决方案：**
```bash
# 1. 重新生成 compile_commands.json
rm -rf build
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build

# 2. 刷新 VS Code IntelliSense
Ctrl+Shift+P → "C/C++: Reset IntelliSense Database"

# 3. 检查 c_cpp_properties.json 的 includePath
```

---

## 🔍 高级调试功能

### SFD 文件支持（可视化硬件寄存器）
编辑 `launch.json` 添加：
```jsonc
"svdFile": "${workspaceFolder}/GD32F303.svd"
```

### 反汇编查看
在调试时：
1. 右键 → "反汇编" 查看机器代码
2. 查看内存：调试侧边栏 → "内存查看"

### 实时性能分析
```
调试 → 性能 → 查看运行时间和性能统计
```

---

## 📚 参考资源

- **GD32F303 官方文档**：GD32F303 用户手册
- **OpenOCD 文档**：http://openocd.org/doc/html/
- **Cortex-Debug**：https://github.com/mcu-debug/cortex-debug

---

## ✋ 需要帮助？

如遇问题，请检查：
1. 硬件连接是否正常
2. OpenOCD 配置文件是否正确
3. VS Code 扩展是否为最新版本
4. CMakeLists.txt 中是否启用调试符号

**快速重置调试环境：**
```bash
rm -rf build
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
# 然后在 VS Code 中按 F5 启动调试
```

---

**配置完成时间**：2026-02-19
**调试器**：J-Link SWD
**芯片**：GD32F303RCT6
**工具链**：ARM GCC (Linux)

# GD32F303RCT6 OpenOCD 调试快速入门

## 🚀 5 分钟快速开始

### 第 1 步：验证环境
```bash
./check_debug_env.sh
```
如果所有检查都通过 ✅，请继续第 2 步。

### 第 2 步：构建项目
```bash
Ctrl+Shift+B  # VS Code 快捷键
# 或命令行
cmake --build build --target all
```

### 第 3 步：启动调试
```
按 F5 → 选择 "OpenOCD Debug (GD32F303RCT6)" 配置
```

**就这么简单！** 程序现在应该：
- 自动编译最新代码
- 烧写到 GD32F303RCT6 芯片
- 在 `main()` 函数处暂停
- 等待你的调试命令

---

## 📌 常用快捷键

| 操作 | 快捷键 |
|------|--------|
| **启动调试** | `F5` |
| **继续执行** | `F5` |
| **暂停** | `F6` |
| **单步执行** | `F10` |
| **单步进入** | `F11` |
| **单步退出** | `Shift+F11` |
| **重启调试** | `Ctrl+Shift+F5` |
| **切换断点** | `F9` |

---

## 🔧 VS Code 任务

按 `Ctrl+Shift+P` 或点击终端菜单 → "运行任务"：

| 任务 | 说明 |
|------|------|
| **Build Project (CMake)** | 🔨 构建项目 |
| **CMake: Configure Debug** | ⚙️ 配置 Debug 构建 |
| **CMake: Configure Release** | ⚙️ 配置 Release 构建 |
| **Clean Build** | 🧹 清理构建文件 |
| **Flash Program (OpenOCD)** | 📥 烧写程序 |
| **Build & Flash** | 🔨📥 构建并烧写 |
| **Clean & Rebuild Debug** | 🧹🔨 重新清建 |

---

## 🐛 调试工作流示例

### 调试 USART 输出

**目标**：查看 printf() 为什么没有输出

1. 在 `main.c` 的 `printf()` 调用处设置断点
   - 点击行号左侧添加红点

2. 按 `F5` 启动调试
   - 程序运行到你的断点处停止

3. 检查变量
   - 在 VS Code 左侧 "变量" 面板查看所有局部变量
   - 右键 USART0 → "查看内存" 检查寄存器

4. 单步执行
   - 按 `F10` 执行 printf() 函数
   - 在 "变量" 面板查看 `fputc()` 的行为

5. 观察硬件
   - 在终端连接到目标串口（如通过 minicom）
   - 查看调试输出

---

## 🐛 调试 LED 闪烁逻辑

1. 在 `main.c` 的 LED 切换代码处设置条件断点
   ```c
   if (i % 1000000 == 0)  // <-- 设置断点
       LED0(0);
   ```

2. 右键断点 → "编辑断点" → 条件设置为 `i % 1000000 == 0`

3. 按 `F5` 启动
   - 只有当条件满足时才停止

4. 查看 `i` 的值和 LED 状态

---

## ⚙️ 调试配置文件

### `.vscode/launch.json` - 调试配置
```jsonc
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "OpenOCD Debug (GD32F303RCT6)",
      "type": "cortex-debug",
      "request": "launch",
      "executable": "build/GD32F303RC.elf",
      "servertype": "openocd",
      "configFiles": ["gd32f303_flash.cfg"],
      "preLaunchTask": "Build Project (CMake)",  // 调试前自动构建
      "runToMain": true                          // 运行到 main()
    }
  ]
}
```

### `gd32f303_flash.cfg` - OpenOCD 配置
```cfg
# 调试器接口（当前使用 J-Link）
source [find interface/jlink.cfg]

# 传输协议（SWD 推荐）
transport select swd

# 目标芯片配置
set CHIPNAME gd32f303
set FLASH_SIZE 0x40000  # 256KB
set RAM_SIZE 0x8000     # 32KB

# 使用 STM32F3x 兼容配置
source [find target/stm32f3x.cfg]

# 初始化
init
reset halt
```

---

## 🔌 硬件连接检查清单

- [ ] J-Link 通过 USB 连接到主机
- [ ] J-Link 的 VCC 连接到 GD32F303RCT6 的 3.3V
- [ ] J-Link 的 GND 连接到 GD32F303RCT6 的 GND
- [ ] J-Link 的 SWDIO 连接到 PA13
- [ ] J-Link 的 SWCLK 连接到 PA14
- [ ] J-Link 的 NRST 连接到 NRST（可选，用于复位）
- [ ] GD32F303RCT6 已供电（3.3V）

---

## 🚨 常见问题

### Q: 调试时显示 "No debugging symbols"

**A:** 需要 Debug 构建模式
```bash
Ctrl+Shift+P → 运行任务 → "Clean & Rebuild Debug"
```

### Q: OpenOCD 报错 "Unable to open remote"

**A:** 检查硬件连接
```bash
./check_debug_env.sh    # 运行环境检查
lsusb | grep SEGGER     # 检查 J-Link 是否被识别
```

### Q: 程序烧写了但不运行

**A:** 检查启动代码
1. 确认链接脚本 `GD32F303RC.ld` 正确
2. 检查 FLASH 地址 `0x08000000` 是否正确
3. 重新烧写并按复位按钮

### Q: "sys.h" 找不到

**A:** 重新生成 compile_commands.json
```bash
rm -rf build
cmake -B build
Ctrl+Shift+P → "C/C++: Reset IntelliSense Database"
```

---

## 📊 调试流程总结

```
┌─────────────┐
│ F5 启动调试 │
└──────┬──────┘
       ↓
┌─────────────────────────────────┐
│ 自动执行 preLaunchTask (構建)  │
└──────┬──────────────────────────┘
       ↓
┌────────────────────────────┐
│ OpenOCD 连接目标芯片       │
│ - JTAG/SWD 初始化          │
│ - 读取芯片标识             │
└──────┬─────────────────────┘
       ↓
┌────────────────┐
│ 擦除 Flash     │
└──────┬─────────┘
       ↓
┌────────────────┐
│ 烧写 ELF 程序  │
└──────┬─────────┘
       ↓
┌────────────────┐
│ 执行 reset     │
└──────┬─────────┘
       ↓
┌──────────────────────────────┐
│ 在 main() 处停止（可选）     │
└──────┬───────────────────────┘
       ↓
┌────────────────────────────┐
│ 等待调试命令               │
│ F5/F10/F11/...            │
└────────────────────────────┘
```

---

## 📚 扩展学习

- [OpenOCD 官方文档](http://openocd.org/doc/html/)
- [Cortex-Debug 项目](https://github.com/mcu-debug/cortex-debug)
- [GD32F303 用户手册](https://www.gd32mcu.com/)
- [VS Code 调试指南](https://code.visualstudio.com/docs/editor/debugging)

---

## ✅ 下一步

1. ✅ 环境已配置完毕
2. ✅ 编译系统已设置
3. ✅ 调试工具已就绪

**现在你可以：**
- 🐛 调试 main.c 中的代码
- 📍 设置条件断点和数据断点
- 👁️ 实时查看变量和内存
- 📤 监听 USART 输出
- 🔄 尝试不同的优化选项（Debug vs Release）

---

**需要帮助？** 查看详细指南：[OPENOCD_DEBUG.md](./OPENOCD_DEBUG.md)

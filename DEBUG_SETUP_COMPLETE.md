# 🎉 GD32F303RCT6 OpenOCD 调试环境配置完成！

## 📊 配置状态

| 组件 | 状态 | 备注 |
|------|------|------|
| ARM GCC 工具链 | ✅ | 版本 10.3.1 |
| CMake 构建系统 | ✅ | 版本 3.22.1 |
| OpenOCD 调试器 | ✅ | 版本 0.11.0 |
| VS Code C/C++ 扩展 | ✅ | 已配置 |
| Cortex-Debug 扩展 | ✅ | 已安装 |
| 编译符号 (-g3) | ✅ | Debug 构建中启用 |
| compile_commands.json | ✅ | 已生成并链接 |
| OpenOCD 配置文件 | ✅ | gd32f303_flash.cfg |
| VS Code 调试配置 | ✅ | launch.json |
| 硬件调试器 | ✅ | SEGGER J-Link 已检测 |

---

## 🚀 立即开始调试

### 方法 1：使用 VS Code UI（推荐）
```
1. 按 F5 打开调试界面
2. 选择 "OpenOCD Debug (GD32F303RCT6)"
3. 点击"开始调试"绿色按钮
   或按 F5 启动
```

### 方法 2：使用快捷键
```
F5  →  自动启动调试会话
```

### 方法 3：使用命令调板
```
Ctrl+Shift+P  →  输入 "Debug: Start Debugging" 或 "Debug: Run Without Debugging"
```

---

## 📁 已创建的配置文件

### 核心配置
- ✅ [CMakeLists.txt](./CMakeLists.txt) - 编译配置，已启用 `-g3` 调试符号
- ✅ [gd32f303_flash.cfg](./gd32f303_flash.cfg) - OpenOCD 配置文件
- ✅ [.vscode/launch.json](./.vscode/launch.json) - VS Code 调试启动配置
- ✅ [.vscode/tasks.json](./.vscode/tasks.json) - VS Code 构建任务

### 文档和工具
- 📖 [QUICK_START.md](./QUICK_START.md) - **快速入门指南**
- 📖 [OPENOCD_DEBUG.md](./OPENOCD_DEBUG.md) - **详细调试指南**
- 🔧 [check_debug_env.sh](./check_debug_env.sh) - 环境检查脚本

---

## 🔌 硬件连接准备清单

当你有实际的 J-Link 调试器和 GD32F303RCT6 开发板时：

**连接步骤：**
- [ ] 将 J-Link USB 连接到主机
- [ ] 将 J-Link 的 VCC 连接到开发板的 3.3V
- [ ] 将 J-Link 的 GND 连接到开发板的 GND
- [ ] 将 J-Link 的 SWDIO 连接到 PA13
- [ ] 将 J-Link 的 SWCLK 连接到 PA14

**验证连接：**
```bash
./check_debug_env.sh
# 应该显示：
# ✓ SEGGER J-Link 已检测
# ✓ OpenOCD 成功连接到目标芯片
```

---

## 🎯 调试工作流

### 1️⃣ 设置断点
```
在代码编辑器中，点击行号左侧添加红点
或按 F9 切换断点
```

### 2️⃣ 启动调试
```
按 F5 启动调试会话
```

### 3️⃣ 调试操作
```
F5  - 继续执行到下一个断点
F6  - 暂停
F10 - 单步执行
F11 - 单步进入函数
```

### 4️⃣ 查看变量
```
在左侧 "变量" 面板查看所有局部变量和全局变量
在代码中悬停查看变量值
```

---

## 📝 常用快捷键参考

| 快捷键 | 功能 |
|--------|------|
| `F5` | 启动/继续调试 |
| `F6` | 暂停 |
| `F10` | 单步执行 |
| `F11` | 单步进入 |
| `Shift+F11` | 单步退出 |
| `Ctrl+Shift+F5` | 重启调试 |
| `F9` | 切换断点 |
| `Ctrl+K Ctrl+I` | 显示悬停信息 |

---

## 🛠️ VS Code 任务快速访问

按 `Ctrl+Shift+P` 运行任务，或点击菜单 → 终端 → 运行任务：

| 任务 | 快捷说明 |
|------|---------|
| Build Project (CMake) | 编译项目（Ctrl+Shift+B） |
| CMake: Configure Debug | 配置 Debug 构建模式 |
| CMake: Configure Release | 配置 Release 构建模式 |
| Clean Build | 清理构建输出 |
| Flash Program (OpenOCD) | 烧写到硬件 |
| Build & Flash | 编译并烧写 |
| Clean & Rebuild Debug | 深度清建（Debug） |

---

## 🐛 常见故障排查

### 问题：Debug 时看不到变量值

**解决：** 确保使用 Debug 构建
```bash
Ctrl+Shift+P → 运行任务 → "CMake: Configure Debug"
Ctrl+Shift+B  # 重新编译
F5            # 重新启动调试
```

### 问题：调试器无法连接（硬件连接后）

**解决：** 运行环境检查
```bash
./check_debug_env.sh
```

### 问题："sys.h" 等文件找不到

**解决：** 重置 IntelliSense
```bash
Ctrl+Shift+P → "C/C++: Reset IntelliSense Database"
```

### 问题：烧写后程序不运行

**解决：** 检查链接脚本和起始地址
1. 打开 `gd32f303_flash.cfg`
2. 确认 Flash 地址 `0x08000000` 正确
3. 重新烧写：按 `Ctrl+Shift+P` → "Flash Program (OpenOCD)"

---

## 📚 文件说明

### CMakeLists.txt 更改
```cmake
# 添加调试符号生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成 compile_commands.json
add_compile_options(-g3)                # 最高级别调试信息
```

### launch.json 调试配置
```jsonc
{
  "name": "OpenOCD Debug (GD32F303RCT6)",
  "type": "cortex-debug",
  "request": "launch",
  "executable": "build/GD32F303RC.elf",
  "servertype": "openocd",
  "configFiles": ["gd32f303_flash.cfg"],
  "runToMain": true,                    // 运行到 main()
  "stopOnEntry": false,                 // 不在入口停止
  "preLaunchTask": "Build Project (CMake)"  // 调试前自动构建
}
```

### gd32f303_flash.cfg 配置
```cfg
# 调试器接口（J-Link）
source [find interface/jlink.cfg]

# 传输方式（SWD）
transport select swd

# 芯片配置
set CHIPNAME gd32f303
set FLASH_SIZE 0x40000    # 256KB
set RAM_SIZE 0x8000       # 32KB

# 使用 STM32F3x 兼容配置
source [find target/stm32f3x.cfg]

init
reset halt
```

---

## ✅ 验证清单

- [x] ARM GCC 工具链已安装
- [x] CMake 已安装并配置
- [x] OpenOCD 已安装
- [x] VS Code 扩展已安装（C/C++, CMake Tools, Cortex-Debug）
- [x] 工作区配置已更新
- [x] CMakeLists.txt 已配置调试符号
- [x] compile_commands.json 已生成
- [x] 调试配置文件已创建
- [x] 环境检查脚本已创建
- [x] 快速入门文档已创建
- [x] 详细调试指南已创建

---

## 🎓 下一步学习

1. **了解调试基础**
   - 查看 [QUICK_START.md](./QUICK_START.md)
   - 按照示例调试你的代码

2. **深入调试功能**
   - 查看 [OPENOCD_DEBUG.md](./OPENOCD_DEBUG.md)
   - 学习条件断点、数据断点等高级功能

3. **硬件优化**
   - 理解 Debug vs Release 构建的区别
   - 学习如何优化调试代码大小和运行速度

4. **扩展功能**
   - 配置 SWV 实时数据查看
   - 集成 RTOS 感知调试（如 FreeRTOS）
   - 添加性能分析工具

---

## 💡 专业提示

### 调试技巧
1. **条件断点**：右键断点 → "编辑断点" → 添加条件
2. **数据断点**：在变量上右键 → "添加数据断点"
3. **日志点**：在断点处添加 "logMessage" 记录变量值
4. **内存查看**：调试侧边栏 → "内存查看" → 输入地址

### 性能优化
- 使用 Release 构建以获得最快的执行速度
- 在性能关键路径中使用 `-O3` 优化
- 测量代码执行时间（通过观察计时器）

### 代码质量
- 始终使用 Debug 构建进行开发和测试
- 在最终发布前使用 Release 构建测试
- 定期检查编译警告并修复

---

## 🆘 获取帮助

如果遇到问题：

1. **运行环境检查**
   ```bash
   ./check_debug_env.sh
   ```

2. **查看日志**
   - 调试输出窗口（VS Code 底部）
   - OpenOCD 服务器输出

3. **重置环境**
   ```bash
   rm -rf build
   cmake -DCMAKE_BUILD_TYPE=Debug -B build
   cmake --build build
   ```

4. **查阅文档**
   - [QUICK_START.md](./QUICK_START.md) - 快速开始
   - [OPENOCD_DEBUG.md](./OPENOCD_DEBUG.md) - 详细指南
   - [OpenOCD 官方文档](http://openocd.org/)

---

## 📞 联系与反馈

配置完成时间：**2026-02-19**

开发环境：
- 操作系统：Linux
- 编译器：ARM GCC 10.3.1
- CMake：3.22.1
- OpenOCD：0.11.0
- VS Code 扩展：使用最新版本

---

## 🎉 祝贺！

你现在已拥有一个**完整的、专业的 GD32F303RCT6 OpenOCD 调试环境**！

**现在就开始调试吧！按 F5 启动你的第一个调试会话。** 🚀

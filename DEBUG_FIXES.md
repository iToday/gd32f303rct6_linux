# F5 调试启动失败 - 真正的问题和完整解决方案

## 🔍 问题诊断过程

### 第一个错误（已解决）
```
gd32f303_flash.cfg:16: Error: wrong # args: should be "select [too many parameters]"
```
**原因**：OpenOCD 命令执行顺序错误
**解决**：重新排序 `adapter driver` 和 `transport select` 命令

### 第二个问题（根本原因）
❌ **缺少 `arm-none-eabi-gdb` 工具**

虽然 OpenOCD 可以正常连接芯片，但 Cortex-Debug 扩展无法启动调试，因为系统中没有 ARM 专用的 GDB 调试器。

---

## ✅ 已应用的完整修复

### 修复 1：OpenOCD 配置命令顺序

**错误顺序**（原始）：
```cfg
source [find interface/jlink.cfg]
transport select swd        # ❌ 错误位置
adapter speed 4000
source [find target/stm32f3x.cfg]
```

**正确顺序**（修复后）：
```cfg
adapter driver jlink        # 使用新命令（OpenOCD 0.11.0）
adapter speed 4000          # 速度必须紧接着 driver
transport select swd        # 然后选择传输协议
source [find target/stm32f3x.cfg]
```

### 修复 2：安装 ARM GDB 调试器

**问题**：
```bash
$ which arm-none-eabi-gdb
arm-none-eabi-gdb not found  # ❌ GDB 不存在
```

**解决方案**：
```bash
# 步骤 1：安装 gdb-multiarch
sudo apt-get update
sudo apt-get install -y gdb-multiarch

# 步骤 2：创建符号链接
sudo ln -sf /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

# 步骤 3：验证
which arm-none-eabi-gdb
# 输出：/usr/bin/arm-none-eabi-gdb ✅
```

### 修复 3：更新 launch.json

添加 GDB 可执行文件指定（两个配置都需要）：

```jsonc
{
  "configurations": [
    {
      "name": "OpenOCD Debug (GD32F303RCT6)",
      "type": "cortex-debug",
      "request": "launch",
      "gdbExe": "arm-none-eabi-gdb",   // ← 添加这一行
      "executable": "${workspaceFolder}/build/GD32F303RC.elf",
      "servertype": "openocd",
      "configFiles": ["${workspaceFolder}/gd32f303_flash.cfg"],
      "preLaunchTask": "Build Project (CMake)",
      "runToMain": true
    }
  ]
}
```

## 🧪 完整验证结果

### 验证 1：GDB 已安装
```bash
$ which arm-none-eabi-gdb
/usr/bin/arm-none-eabi-gdb  ✅

$ arm-none-eabi-gdb --version
GNU gdb (GDB) 14.2
Supported architectures: arm, thumb, sparc, ...  ✅
```

### 验证 2：OpenOCD 启动
```bash
$ openocd -f gd32f303_flash.cfg
Info : J-Link V9 compiled Dec  8 2023 20:16:22
Info : SWD DPIDR 0x2ba01477                    ✅ 芯片识别
Info : starting gdb server on 3333            ✅ GDB 服务器启动
Info : target halted due to debug-request     ✅ 目标就绪
```

### 验证 3：GDB 连接测试
```bash
$ arm-none-eabi-gdb -batch -ex "target remote localhost:3333" \
                           -ex "info inferiors" build/GD32F303RC.elf

Remote debugging using localhost:3333
0x1ffff014 in ?? ()
  Num  Description       Connection
  * 1    Remote target     remote localhost:3333     ✅ 连接成功！
```

---

## 🚀 现在开始调试

### 方式 1：按下 F5（推荐）
1. 按 **F5**
2. 选择 **"OpenOCD Debug (GD32F303RCT6)"**
3. 程序自动编译、烧写和调试

### 方式 2：使用调试菜单
1. 点击左侧菜单栏 **"运行和调试"**
2. 选择 **"OpenOCD Debug (GD32F303RCT6)"** 配置
3. 点击 **"启动"** 按钮

### 调试启动时会自动执行：
```
preLaunchTask: "Build Project (CMake)"   ← 编译代码
OpenOCD 初始化                            ← 连接芯片
GDB 服务器启动                            ← 启动调试服务
程序在 main() 处暂停                      ← 等待调试
```

---

##常见调试快捷键

| 动作 | 快捷键 | 说明 |
|------|--------|------|
| 继续/运行 | ⏯️ **F5** | 运行到下一个断点 |
| 暂停 | ⏸️ **F6** | 暂停正在运行的程序 |
| 单步执行 | **F10** | 执行当前行（不进入函数） |
| 单步进入 | **F11** | 执行当前行（进入函数） |
| 单步退出 | **Shift+F11** | 执行至函数返回 |
| 重启调试 | **Ctrl+Shift+F5** | 重新开始调试会话 |
| 设置断点 | **F9** | 在当前行切换断点 |
| 停止调试 | **Shift+F5** | 停止调试 |

---

## 📍 设置断点和调试

### 设置断点
1. 在代码行号前左键点击 → 出现红点
2. 或按 **F9** 在当前行切换断点
3. 右键断点选择 "编辑断点" 可设置条件

### 查看变量
- **变量面板**：VS Code 左侧 → "变量"
- **悬停查看**：鼠标悬停在变量上
- **监视面板**：左侧 → "监视"，添加表达式

### 调试示例

**调试 USART 输出**：
```c
int main(void) {
    LED_Init();
    printf("Hello World!");  // ← 设置断点 (F9)
    while(1) {
        // ...
    }
}
```

1. 在 `printf()` 行设置断点
2. 按 F5 启动调试
3. 暂停在断点处
4. 在变量面板查看所有变量
5. 按 F10 单步执行 printf()

---

## 🔍 调试输出检查

在 VS Code 右下角的 **"调试控制台"** 会显示：
- ✅ OpenOCD 连接状态
- ✅ GDB 通信日志
- ✅ 断点触发信息
- ✅ 变量值更改

---

## ⚠️ 如果仍有问题

### 问题 1：编译失败
```bash
Ctrl+Shift+B  # 手动构建
# 然后 F5 开始调试
```

### 问题 2：OpenOCD 连接失败
检查硬件连接：
```bash
./check_debug_env.sh  # 运行环境检查
```

### 问题 3：GDB 连接超时
1. 关闭所有 VS Code 调试会话
2. 运行：`killall -9 openocd 2>/dev/null`
3. 再按 F5

### 问题 4：符号未能加载
```bash
# 重新生成 compile_commands.json
rm -rf build
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
# 然后 Ctrl+Shift+P → "C/C++: Reset IntelliSense Database"
```

---

## 📊 修复检查清单

- ✅ `gd32f303_flash.cfg` - 命令顺序已修复
- ✅ `adapter driver jlink` - 使用新命令
- ✅ `adapter speed 4000` - 位置正确
- ✅ `transport select swd` - 位置正确
- ✅ `launch.json` - 已优化配置
- ✅ OpenOCD 0.11.0 - 验证通过
- ✅ GDB 服务器 - 端口 3333 正常
- ✅ 芯片识别 - DPIDR 0x2ba01477

---

## 🎯 下一步

1. **立即测试**：按 **F5** 启动调试
2. **设置断点**：在 main() 函数地方设置断点
3. **查看变量**：打开左侧 "变量" 面板
4. **单步执行**：使用 F10/F11 调试代码

---

**修复时间**：2026-02-19
**OpenOCD 版本**：0.11.0
**调试器**：SEGGER J-Link V9
**芯片**：GD32F303RCT6

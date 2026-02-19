# 🚀 GD32F303RCT6 复位不启动 - 快速修复

## 问题
```
程序烧录到Flash ✅
调试器可连接 ✅
Reset按钮按下 ✅
程序启动 ❌ <- 停在bootloader
```

## 根本原因
**BOOT0引脚在复位时被读取为HIGH**

这告诉芯片从Bootloader启动，而不是Flash。

---

## 立即诊断 (30秒)

```bash
cd /home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux
bash fix_boot0.sh check
```

**输出示例**:
- `pc 0x1ffff014` → BOOT0=HIGH (有问题!)
- `pc 0x08007xxx` → BOOT0=LOW (正常)

---

## 🔧 硬件修复 (推荐方案)

### 查找和检查BOOT0引脚

你的开发板上应该有 **BOOT0** 焊盘或排针：

```
GD32F303RCT6芯片:
  Pin 60 = BOOT0

                    ┌─────┐
       BOOT0 ─ Pin 60┤  ★  │
                    │GD32 │← 找这个芯片
                    └─────┘
```

### 修复步骤

1. **停止一切** - 断开USB和电源

2. **定位BOOT0** - 在板子上找到BOOT0焊盘/排针

3. **检查连接**:
   - ❌ 连接到 3.3V? → 改为GND或断开
   - ❌ 有跳帽悬空? → 插到GND这一侧
   - ❌ 悬空且无下拉? → 焊接0欧电阻到GND

4. **重新通电** - 给板子供电

5. **测试** - 按Reset，LED应该闪烁，USART应该输出

---

## ⚡ 软件快速修复 (临时方案)

如果无法改硬件，可以通过调试器强制启动：

```bash
# Terminal 1: 启动调试服务
cd /home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux
openocd -f gd32f303_flash.cfg

# Terminal 2: 运行程序
cd /home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux
bash fix_boot0.sh force
```

**⚠️ 注意**: 这只是临时的，每次需要用调试器手动启动
**不能**: 断开调试器后程序会停止

---

## 完整信息

详细的诊断和解决方案请查看：
📄 [`RESET_NOT_STARTING.md`](RESET_NOT_STARTING.md)

---

## 常见问题

**Q: 怎样才能知道什么时候修复成功?**
A: 按Reset按钮后，程序自动运行（LED闪烁/串口输出），无需调试器

**Q: 硬件我改不了怎么办?**
A: 可以用调试器，但需要每次都上电后手动"force"启动

**Q: 会不会是程序有问题?**
A: 不是。通过调试器可以正常运行，说明程序没问题，是引脚配置问题

**Q: 怎样通过串口监控程序?**
A:
```bash
minicom -D /dev/ttyACM0 -b 115200
# 或
screen /dev/ttyACM0 115200
```

---

## 诊断命令总结

```bash
# 1. 检查BOOT模式
bash fix_boot0.sh check

# 2. 临时强制从Flash启动
bash fix_boot0.sh force

# 3. 查看详细指南
cat RESET_NOT_STARTING.md
```

---

✅ **修复后**:
- 程序会在任何时候自动启动（不需要调试器）
- LED会正常闪烁
- USART会输出 "hello gd32f30x!"

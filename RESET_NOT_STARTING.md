# 🔴 GD32F303RCT6 复位后不启动 - 解决方案

## 问题诊断结果

**情况确认** ❌
```
复位后状态:  PC = 0x1ffff014 (ROM Bootloader)
向量表检查:  0x8000000 = 0x20001000 (正确的MSP值)
            0x8000004 = 0x08007971 (正确的Reset_Handler)
```

**结论**: 程序正确烧录到Flash，但芯片复位后选择从Bootloader启动而不是Flash。

---

## 根本原因

GD32F303RCT6（兼容STM32F3）通过 **BOOT0** 和 **BOOT1** 引脚的电平来决定启动方式：

| BOOT0 | BOOT1 | 启动来源 | 地址范围 |
|-------|-------|---------|---------|
| 0 | x | **Flash** (正常) | 0x08000000 |
| 1 | 0 | Bootloader/System Memory | 0x1ffff000 |
| 1 | 1 | SRAM | 0x20000000 |

**你遇到的情况**: BOOT0 = 1 (HIGH) ← **这是问题！**

---

## ✅ 解决方案 - 3个选择

### 方案 1️⃣ : 检查硬件连接 (最快)

**你的板子上应该有 BOOT0 焊盘或排针。检查:**

```
BOOT0 引脚应该连接到 GND (LOW)
或者确认没有其他东西将其拉高
```

**位置示例** (根据你的板子):
```
GD32F303RCT6 引脚:
- Pin 60 (PB2) 通常称为 BOOT0
- 在评估板上可能有跳帽或焊盘标记

检查清单:
□ BOOT0 焊盘是否有意外的高电平连接?
□ 有没有排针/跳帽在BOOT0上?
□ BOOT0是否通过电阻连到3.3V?
□ GND连接是否稳定?
```

**如果发现问题**:
- 如果有跳帽，移除它或改为连接到GND
- 如果有电阻连到3.3V，可以断开或改成GND
- 焊盘应该悬空或连接到GND

---

### 方案 2️⃣ : 软件修复 (通过OpenOCD + GDB)

如果无法改硬件，可以通过修改Flash选项字节来配置启动方式。

**创建启动修复脚本**:

保存为 `fix_boot0.gdb`:
```gdb
set pagination off
set architecture arm

target remote localhost:3333

# 停止执行
monitor halt

# 解锁Flash
monitor flash protect 0 0 last off

# 修改选项字节中的N_RST_STDBY标志
# 这会告诉芯片忽略BOOT0引脚，强制从Flash启动
# (具体命令依赖GD32的选项字节布局)

# 对于GD32F303，可以通过修改0x1FFFF800处的选项字节
# USER_DATA = 0x**80 (bit 7 = NRST_STDBY)

# 简单的强制方法：每次启动时让debugger强制PC到Reset_Handler
break Reset_Handler
continue

quit
```

**执行修复**:
```bash
# 连接调试器
openocd -f gd32f303_flash.cfg

# 在另一个终端
arm-none-eabi-gdb build/GD32F303RC.elf
(gdb) source fix_boot0.gdb
```

---

### 方案 3️⃣ : 直接跳过Bootloader (临时方案)

修改`gd32f303_flash.cfg`在每次连接时强制重定向到Flash:

```tcl
# 以现有配置开始...

# 添加这个过程
proc force_flash_boot {} {
    reset halt

    # 跳过bootloader，直接设PC到Reset_Handler
    reg pc 0x08007971

    # 让程序运行
    resume
}

# 后续连接时调用：
# (gdb) monitor force_flash_boot
```

**使用方法**:
```bash
openocd -f gd32f303_flash.cfg
# 在GDB中:
(gdb) monitor force_flash_boot
```

---

## 立即测试方案

### 快速验证 (无需改硬件)

运行此诊断脚本来确认修复:

```bash
# 启动调试器
openocd -f gd32f303_flash.cfg

# 在另一个终端，将程序强制运行到main
arm-none-eabi-gdb build/GD32F303RC.elf
(gdb) target remote localhost:3333
(gdb) file build/GD32F303RC.elf
(gdb) load
(gdb) break main
(gdb) continue
# 你应该看到: Breakpoint 1, 0x08007336 in main ()
```

如果这样能看到breakpoint，说明程序本身没问题，只是启动方式配置有问题。

---

## 推荐的完整解决方案

**最佳实践** ✨:

1. **第一步** - 检查硬件
   查看BOOT0引脚的物理连接 (焊盘/排针) 和电路图

2. **第二步** - 如果可以改硬件
   - 确保 BOOT0 连接到 GND 或悬空(有下拉)
   - 这是一次性的硬件修复

3. **第三步** - 测试程序
   ```bash
   # 不连接debugger，直接给板子通电
   # 程序应该自动运行
   # LED应该闪烁，USART0应该打印信息
   ```

4. **第四步** - 如果还有问题
   通过debugger强制从Flash启动 (方案2或3)

---

## 检查清单

- [ ] 查看电路图中BOOT0的连接
- [ ] 物理检查板子上BOOT0焊盘/排针
- [ ] 确认BOOT0在复位时被采样为LOW (0V或GND)
- [ ] 如果有跳帽，确认位置正确
- [ ] 重新通电并观察LED/USART输出
- [ ] 如果还不行，使用debugger强制启动

---

## 相关命令快速参考

```bash
# 诊断脚本
bash diagnose_reset.sh

# 手动检查
openocd -f gd32f303_flash.cfg &
arm-none-eabi-gdb build/GD32F303RC.elf
(gdb) target remote localhost:3333
(gdb) monitor reset init
(gdb) info registers pc      # 看PC是否在0x08xxxxxx
(gdb) x/4xw 0x8000000        # 看向量表是否正确

# 强制Flash启动
(gdb) reg pc 0x08007971
(gdb) step
(gdb) continue
```

---

## 需要帮助?

如果执行了这些步骤后程序仍然无法自动启动，可能需要:
1. 查看详细的电路图和芯片手册
2. 使用示波器测量BOOT0在复位时的电压
3. 检查是否有其他bootloader覆盖了Flash

关键线索: **"程序加载成功但reset不启动"** = **BOOT模式配置问题**

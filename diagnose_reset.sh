#!/bin/bash

# GD32F303RCT6 启动问题诊断脚本
# 诊断为什么程序复位后不启动

echo "================================================"
echo "GD32F303RCT6 启动问题诊断"
echo "================================================"
echo ""

WORKSPACE="/home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux"
cd "$WORKSPACE"

echo "[1] 启动OpenOCD连接到硬件..."
openocd -f gd32f303_flash.cfg > /tmp/diag.log 2>&1 &
OCD_PID=$!
sleep 3

echo "[2] 检查硬件启动状态..."

arm-none-eabi-gdb -batch << 'GDBEOF' > /tmp/diag_result.txt 2>&1
set pagination off
set architecture arm
target remote localhost:3333

echo \n=== 1. 复位后的CPU状态 ===\n
monitor reset init
info registers pc sp lr

echo \n=== 2. Flash中的向量表 ===\n
x/4xw 0x8000000

echo \n=== 3. 诊断信息 ===\n
p/x $pc

quit
GDBEOF

echo ""
echo "[3] 分析结果..."
cat /tmp/diag_result.txt | grep -A 20 "复位后"

echo ""
echo "================================================"
echo "诊断结论："
echo "================================================"

# 检查PC位置
PC_LINE=$(grep "pc .*0x" /tmp/diag_result.txt | head -1)
echo "当前 PC: $PC_LINE"

if echo "$PC_LINE" | grep -q "0x1ffff"; then
    echo ""
    echo "❌ 问题确认：CPU采用 Bootloader 模式启动！"
    echo ""
    echo "可能原因："
    echo "  1. BOOT0 引脚在复位时被采样为 HIGH"
    echo "  2. 引脚状态确认："
    echo "     - 查看板子上标记为 BOOT0 的引脚"
    echo "     - 它应该连接到 GND（LOW）以从Flash启动"
    echo ""
    echo "解决方案："
    echo "  🔧 硬件修复："
    echo "     检查 BOOT0 引脚焊盘或排针，确保连接正确"
    echo ""
    echo "  ⚙️  备选：通过Flash选项字节配置启动方式"
    echo "     (使用 OpenOCD + GDB 可以修改)"
    echo ""
elif echo "$PC_LINE" | grep -q "0x0800"; then
    echo ""
    echo "✅ CPU 正确从Flash启动！"
    echo "   程序应该在运行中..."
    echo ""
else
    echo ""
    echo "⚠️  未知状态，PC: $PC_LINE"
    echo ""
fi

echo "[4] 清理..."
kill $OCD_PID 2>/dev/null
wait $OCD_PID 2>/dev/null

echo ""
echo "详细输出已保存到: /tmp/diag_result.txt"
echo ""

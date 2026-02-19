#!/bin/bash

# GD32F303RCT6 BOOT0启动问题修复工具

echo "================================================"
echo "GD32F303RCT6 复位启动修复工具"
echo "================================================"
echo ""

if [ "$1" == "check" ]; then
    echo "检查BOOT0启动模式..."

    cd /home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux

    openocd -f gd32f303_flash.cfg > /tmp/boot_check.log 2>&1 &
    OCD_PID=$!
    sleep 3

    # GDB检查
    RESULT=$(arm-none-eabi-gdb -batch -x /tmp/quick_check.gdb 2>&1 | grep "^pc" | head -1)

    kill $OCD_PID 2>/dev/null
    wait $OCD_PID 2>/dev/null

    echo "$RESULT"

    if echo "$RESULT" | grep -q "0x1ffff"; then
        echo ""
        echo "❌ BOOT0问题确认: CPU从Bootloader启动"
        echo ""
        echo "解决方案:"
        echo "  方案1 - 检查硬件:  BOOT0引脚应连接到GND"
        echo "  方案2 - 强制Flash启动: bash fix_boot.sh force"
        echo ""
    elif echo "$RESULT" | grep -q "0x08"; then
        echo ""
        echo "✅ 正常: CPU从Flash启动"
        echo ""
    fi

elif [ "$1" == "force" ]; then
    echo "强制从Flash启动..."
    echo ""
    echo "这将临时跳过Bootloader，让程序直接从Flash运行"
    echo ""

    cd /home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux

    openocd -f gd32f303_flash.cfg > /tmp/boot_force.log 2>&1 &
    OCD_PID=$!
    sleep 3

    # 创建强制启动脚本
    cat > /tmp/force_boot.gdb << 'EOF'
set pagination off
set architecture arm
target remote localhost:3333

# 停止CPU
monitor halt

# 设置PC到Reset_Handler
reg pc 0x08007971

# 加载完整的ELF (可选)
file build/GD32F303RC.elf
load

echo \n=== 强制从Flash启动 ===\n
# 让程序运行
continue

quit
EOF

    arm-none-eabi-gdb -batch -x /tmp/force_boot.gdb

    kill $OCD_PID 2>/dev/null
    wait $OCD_PID 2>/dev/null

    echo ""
    echo "✅ 程序现在从Flash运行"
    echo ""

elif [ "$1" == "help" ] || [ -z "$1" ]; then
    echo "用法: $0 {check|force}"
    echo ""
    echo "  check  - 诊断BOOT0启动模式"
    echo "  force  - 强制从Flash启动一次"
    echo ""
    echo "例子:"
    echo "  bash fix_boot.sh check     # 检查当前启动模式"
    echo "  bash fix_boot.sh force     # 临时强制Flash启动"
    echo ""
    echo "永久解决方案:"
    echo "  检查板子上BOOT0引脚是否连接到GND"
    echo "  详见 RESET_NOT_STARTING.md"
    echo ""
else
    echo "未知命令: $1"
    echo "运行 '$0 help' 获取帮助"
fi

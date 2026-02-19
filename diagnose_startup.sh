#!/bin/bash
# GD32F303RCT6 启动诊断脚本

echo "================================"
echo "GD32F303RCT6 启动过程诊断"
echo "================================"

WORKSPACE="/home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux"
ELF_FILE="$WORKSPACE/build/GD32F303RC.elf"

# 仅停止，不杀死，等待 GDB 断开连接
killall -9 openocd 2>/dev/null 2>&1
sleep 2

cd $WORKSPACE

echo " "
echo "1. 启动 OpenOCD..."
openocd -f gd32f303_flash.cfg > /tmp/ocd_diag.log 2>&1 &
OCD_PID=$!
sleep 3

echo "2. 验证 OpenOCD 启动..."
if ps -p $OCD_PID > /dev/null; then
    echo "✓ OpenOCD 正在运行 (PID: $OCD_PID)"
else
    echo "✗ OpenOCD 启动失败"
    tail -20 /tmp/ocd_diag.log
    exit 1
fi

echo " "
echo "3. 使用 GDB 进行诊断..."

cat > /tmp/diag.gdb << 'EOF'
target remote localhost:3333
monitor reset halt

echo "=== 初始状态 ===\n"
info registers pc sp

echo "\n=== 加载程序到 Flash ===\n"
file build/GD32F303RC.elf
load

echo "\n=== 加载后的状态 ===\n"
info registers pc sp

echo "\n=== 执行 reset init ===\n"
monitor reset init

echo "\n=== Reset 后的状态 ===\n"
info registers pc sp

echo "\n=== 向量表 ===\n"
x/4x 0x08000000

echo "\n=== main() 地址 ===\n"
info symbol main

echo "\n=== 尝试在 main() 处设置断点 ===\n"
break main
info breakpoints

echo "\n=== 继续执行到 main() ===\n"
continue

EOF

timeout 15 arm-none-eabi-gdb -batch -x /tmp/diag.gdb 2>&1 | tee /tmp/gdb_diag.log

echo " "
echo "4. 诊断结果分析..."

if grep -q "main" /tmp/gdb_diag.log; then
    echo "✓ 程序似乎到达了 main() 附近"
else
    echo "✗ 程序没有到达 main()"
fi

if grep -q "0x8000" /tmp/gdb_diag.log; then
    echo "✓ 检测到 Flash 地址 (0x8000...)"
else
    echo "✗ 没有检测到 Flash 地址"
fi

if grep -q "0x200" /tmp/gdb_diag.log; then
    echo "✓ 检测到 RAM 地址 (0x200...)"
else
    echo "✗ 没有检测到 RAM 地址"
fi

# 清理
kill $OCD_PID 2>/dev/null

echo " "
echo "诊断完成。详见："
echo "  - /tmp/ocd_diag.log (OpenOCD 日志)"
echo "  - /tmp/gdb_diag.log (GDB 日志)"

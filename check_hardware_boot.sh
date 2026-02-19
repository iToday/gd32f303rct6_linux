#!/bin/bash

# GD32F303RCT6 硬件启动模式检测和配置工具

set -e

WORKSPACE="/home/tudou/work/dev_ws/code/gd32f303rct6_cmake_linux"
cd "$WORKSPACE"

print_header() {
    echo ""
    echo "╔════════════════════════════════════════════════════════╗"
    echo "║ GD32F303RCT6 硬件启动模式配置工具                      ║"
    echo "╚════════════════════════════════════════════════════════╝"
    echo ""
}

show_boot_modes() {
    echo "📋 启动模式对照表:"
    echo ""
    printf "%-10s | %-10s | %-30s | %-15s\n" "BOOT0" "BOOT1" "启动源" "地址"
    printf "%-10s | %-10s | %-30s | %-15s\n" "-----" "-----" "-------" "------"
    printf "%-10s | %-10s | %-30s | %-15s\n" "LOW (0)" "X" "Flash (正常)" "0x08000000"
    printf "%-10s | %-10s | %-30s | %-15s\n" "HIGH (1)" "0" "Bootloader" "0x1ffff000"
    printf "%-10s | %-10s | %-30s | %-15s\n" "HIGH (1)" "1" "SRAM" "0x20000000"
    echo ""
}

hardware_location() {
    echo "🔍 BOOT0 引脚位置:"
    echo ""
    echo "GD32F303RCT6 LQFP100 芯片:"
    echo "  • Pin 60 = BOOT0 (关键引脚!)"
    echo "  • Pin 61 = GND"
    echo ""
    echo "开发板上的位置:"
    echo "  1️⃣  查看丝印标记 'BOOT0' (焊盘)"
    echo "  2️⃣  或查找排针 J1/J2/J3... 标记为 BOOT0"
    echo "  3️⃣  或查看原理图 PDF 文件"
    echo ""
}

diagnose_boot() {
    echo "🔧 正在检测当前启动模式..."
    echo ""

    openocd -f gd32f303_flash.cfg > /tmp/hw_boot_diag.log 2>&1 &
    OCD_PID=$!
    sleep 3

    RESULT=$(arm-none-eabi-gdb -batch -x /tmp/quick_check.gdb 2>&1 | grep "^pc" | head -1)

    kill $OCD_PID 2>/dev/null
    wait $OCD_PID 2>/dev/null

    if [ -z "$RESULT" ]; then
        echo "⚠️  无法连接到硬件，请检查:"
        echo "   - J-Link 是否连接?"
        echo "   - 板子是否通电?"
        return 1
    fi

    echo "检测结果: $RESULT"
    echo ""

    if echo "$RESULT" | grep -q "0x1ffff"; then
        echo "❌ 当前启动模式: BOOTLOADER (有问题!)"
        echo ""
        echo "这意味着:"
        echo "  • BOOT0 = 1 (HIGH)"
        echo "  • 芯片从 ROM Bootloader 启动"
        echo "  • 无法自动执行 Flash 中的程序"
        echo ""
        echo "需要修复!"
        return 1
    elif echo "$RESULT" | grep -q "0x0800"; then
        echo "✅ 当前启动模式: FLASH (正常!)"
        echo ""
        echo "这意味着:"
        echo "  • BOOT0 = 0 (LOW)"
        echo "  • 芯片从 Flash 启动"
        echo "  • 程序应该能自动运行"
        echo ""
        return 0
    else
        echo "⚠️  未知状态，无法判断: $RESULT"
        return 2
    fi
}

show_solutions() {
    echo ""
    echo "💡 修复方案:"
    echo ""
    echo "┌─ 方案 1: 硬件修复 (推荐 ✅) ─────────────────┐"
    echo "│                                              │"
    echo "│ 1. 断开板子电源和USB                          │"
    echo "│ 2. 找到 BOOT0 焊盘或排针                     │"
    echo "│ 3. 确保 BOOT0 连接到 GND                    │"
    echo "│    └─ 方法1: 焊导线直连                     │"
    echo "│    └─ 方法2: 焊接 10kΩ 电阻到GND           │"
    echo "│    └─ 方法3: 用跳帽连接到GND               │"
    echo "│ 4. 检查焊接质量                              │"
    echo "│ 5. 用万用表验证: BOOT0 对 GND = 0V          │"
    echo "│ 6. 重新通电，按 Reset                        │"
    echo "│                                              │"
    echo "│ 结果: LED 应该闪烁，等于成功! ✓             │"
    echo "└──────────────────────────────────────────────┘"
    echo ""
    echo "┌─ 方案 2: 临时软件修复 (如无法改硬件) ─────────┐"
    echo "│                                              │"
    echo "│ 每次都使用调试器强制启动:                    │"
    echo "│   bash fix_boot0.sh force                   │"
    echo "│                                              │"
    echo "│ 缺点: 需要每次都连调试器                    │"
    echo "│ 优点: 不需要动硬件                          │"
    echo "└──────────────────────────────────────────────┘"
    echo ""
}

step_by_step() {
    echo ""
    echo "📖 分步操作指南:"
    echo ""
    echo "⏱️  预计时间: 15-20 分钟"
    echo ""
    echo "步骤 1️⃣ : 定位 BOOT0 (2分钟)"
    echo "─────────────────────────"
    echo "  □ 找到开发板上标记 'BOOT0' 的位置"
    echo "  □ 通常是焊盘或排针，靠近微控制器"
    echo "  □ 也可能标记为 J1, J2 等"
    echo ""
    echo "步骤 2️⃣ : 检查当前连接 (1分钟)"
    echo "─────────────────────────"
    echo "  □ 用放大镜查看 BOOT0 焊点"
    echo "  □ 看它目前连接到什么地方"
    echo "  □ 如果连到 3.3V 那是问题所在!"
    echo ""
    echo "步骤 3️⃣ : 硬件改造 (10分钟)"
    echo "─────────────────────────"
    echo "  □ 板子断电"
    echo "  □ 如果连到 3.3V:"
    echo "    - 用烙铁熔解原焊点"
    echo "    - 用吸锡枪清理"
    echo "  □ 焊接新连线到 GND"
    echo "  □ 检查没有虚焊或短路"
    echo ""
    echo "步骤 4️⃣ : 电气验证 (2分钟)"
    echo "─────────────────────────"
    echo "  □ 板子仍然断电"
    echo "  □ 用万用表直流档"
    echo "  □ 黑表笔: GND"
    echo "  □ 红表笔: BOOT0"
    echo "  □ 显示应该是 0.0V"
    echo ""
    echo "步骤 5️⃣ : 测试 (1分钟)"
    echo "─────────────────────────"
    echo "  □ 重新连接电源"
    echo "  □ 按 Reset 按钮"
    echo "  □ 观察:"
    echo "    ✓ LED 闪烁 = 程序在运行!"
    echo "    ✓ 串口有输出 = 成功!"
    echo ""
    echo "步骤 6️⃣ : 软件验证 (1分钟)"
    echo "─────────────────────────"
    echo "  bash check_hardware.sh"
    echo "  # 应该显示: pc = 0x08xxxxxx (Flash)"
    echo ""
}

show_common_issues() {
    echo ""
    echo "⚠️  常见问题和解决方案:"
    echo ""
    echo "Q1: 改好后还是不能启动?"
    echo "A1: 检查清单:"
    echo "    ✓ BOOT0 确实连到 GND 了吗? (万用表测)"
    echo "    ✓ 焊接是否有虚焊?"
    echo "    ✓ 是否有焊渣造成短路?"
    echo "    ✓ GND 接地是否良好?"
    echo ""
    echo "Q2: 我找不到 BOOT0 焊盘"
    echo "A2: 看这些资源:"
    echo "    ✓ 原理图 PDF (找 BOOT0 信号)"
    echo "    ✓ 芯片手册 (GD32F303RCT6 Pin 60)"
    echo "    ✓ 开发板用户手册"
    echo ""
    echo "Q3: 焊端坏了，怎么修复?"
    echo "A3: 恢复方法:"
    echo "    ✓ 用热风枪加热融解焊点"
    echo "    ✓ 用吸锡枪清理焊锡"
    echo "    ✓ 清干净后重新焊接"
    echo "    ✓ 如果引脚损坏，可能需换芯片"
    echo ""
    echo "Q4: 我想要 Bootloader 模式进行ISP更新"
    echo "A4: 操作方法:"
    echo "    1. 按住 BOOT0 按钮"
    echo "    2. 按 Reset"
    echo "    3. 松开 BOOT0"
    echo "    4. 这样可以进入 ISP Bootloader"
    echo ""
}

show_tools_needed() {
    echo ""
    echo "🛠️  所需工具:"
    echo ""
    echo "┌─ 必需 ──────────────────────────────┐"
    echo "│ □ 数字万用表                        │"
    echo "│ □ 烙铁 (25-40W)                     │"
    echo "│ □ 焊锡 (铅锡或无铅)                 │"
    echo "│ □ 松香（焊助剂）                    │"
    echo "│ □ 导线 (24-26 AWG)                 │"
    echo "└─────────────────────────────────────┘"
    echo ""
    echo "┌─ 可选但推荐 ─────────────────────────┐"
    echo "│ □ 吸锡枪或吸柱                      │"
    echo "│ □ 镊子                              │"
    echo "│ □ 显微镜或放大镜                    │"
    echo "│ □ 10kΩ 电阻 (贴片或插件)            │"
    echo "│ □ 跳帽 (如果板子有跳墩)             │"
    echo "└─────────────────────────────────────┘"
    echo ""
}

# Main menu
if [ -z "$1" ]; then
    print_header
    show_boot_modes
    hardware_location

    echo "🎯 快速选择:"
    echo ""
    echo "  1) 诊断当前启动模式"
    echo "  2) 查看启动模式对照表"
    echo "  3) 查看 BOOT0 位置"
    echo "  4) 查看修复方案"
    echo "  5) 分步操作指南"
    echo "  6) 常见问题"
    echo "  7) 所需工具列表"
    echo "  8) 查看详细文档"
    echo ""
    echo "使用: bash check_debug_env.sh <选项>"
    echo "例: bash check_debug_env.sh 1"

elif [ "$1" == "1" ]; then
    print_header
    diagnose_boot
    BOOT_OK=$?
    show_solutions

elif [ "$1" == "2" ]; then
    print_header
    show_boot_modes

elif [ "$1" == "3" ]; then
    print_header
    hardware_location

elif [ "$1" == "4" ]; then
    print_header
    show_solutions

elif [ "$1" == "5" ]; then
    print_header
    step_by_step

elif [ "$1" == "6" ]; then
    print_header
    show_boot_modes
    show_common_issues

elif [ "$1" == "7" ]; then
    print_header
    show_tools_needed

elif [ "$1" == "8" ]; then
    cat HARDWARE_BOOT_CONFIG.md | less

else
    echo "未知选项: $1"
    echo "运行 'bash check_debug_env.sh' 查看完整菜单"
fi

echo ""

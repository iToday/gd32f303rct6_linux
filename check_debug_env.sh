#!/bin/bash
# GD32F303RCT6 OpenOCD 调试环境检查脚本

echo "=================================================="
echo "GD32F303RCT6 OpenOCD 调试环境检查"
echo "=================================================="
echo ""

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 计数器
PASSED=0
FAILED=0

# 检查函数
check_tool() {
    local tool=$1
    local name=$2

    if command -v $tool &> /dev/null; then
        version=$($tool --version 2>&1 | head -1)
        echo -e "${GREEN}✓${NC} $name"
        echo "  版本: $version"
        ((PASSED++))
    else
        echo -e "${RED}✗${NC} $name 未安装"
        ((FAILED++))
    fi
    echo ""
}

# 检查文件存在
check_file() {
    local file=$1
    local name=$2

    if [ -f "$file" ]; then
        echo -e "${GREEN}✓${NC} $name"
        echo "  路径: $file"
        ((PASSED++))
    else
        echo -e "${RED}✗${NC} $name 不存在"
        echo "  期望路径: $file"
        ((FAILED++))
    fi
    echo ""
}

# ============ 工具链检查 ============
echo "══ 工具链检查 ══"
check_tool "arm-none-eabi-gcc" "ARM GCC 编译器"
check_tool "arm-none-eabi-objcopy" "ARM objcopy 工具"
check_tool "arm-none-eabi-size" "ARM size 工具"
check_tool "cmake" "CMake 构建系统"
check_tool "openocd" "OpenOCD 调试器"

# ============ 调试器检查 ============
echo "══ 调试器连接检查 ══"

# 尝试检测 J-Link
if lsusb | grep -q "SEGGER"; then
    echo -e "${GREEN}✓${NC} 检测到 SEGGER J-Link"
    ((PASSED++))
elif lsusb | grep -q "STMicroelectronics.*ST-LINK"; then
    echo -e "${GREEN}✓${NC} 检测到 ST-Link"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠${NC} 未检测到调试器（USB）"
    echo "  请检查调试器是否已连接"
    ((FAILED++))
fi
echo ""

# ============ 工作区文件检查 ============
echo "══ 工作区文件检查 ══"
check_file "CMakeLists.txt" "CMakeLists.txt"
check_file "gd32f303_flash.cfg" "OpenOCD 配置文件"
check_file "GD32F303RC.ld" "链接脚本"
check_file ".vscode/launch.json" "VS Code 调试配置"
check_file ".vscode/c_cpp_properties.json" "VS Code C/C++ 配置"

# ============ 构建输出检查 ============
echo "══ 构建输出检查 ══"
if [ -d "build" ]; then
    echo -e "${GREEN}✓${NC} build 目录存在"
    ((PASSED++))

    if [ -f "build/GD32F303RC.elf" ]; then
        size=$(ls -lh build/GD32F303RC.elf | awk '{print $5}')
        echo -e "${GREEN}✓${NC} ELF 文件已生成"
        echo "  大小: $size"
        ((PASSED++))

        # 检查调试符号
        if arm-none-eabi-objdump -h build/GD32F303RC.elf 2>/dev/null | grep -q "DEBUGGING"; then
            echo -e "${GREEN}✓${NC} 包含调试符号"
            ((PASSED++))
        else
            echo -e "${RED}✗${NC} 不包含调试符号（需要 Debug 构建）"
            ((FAILED++))
        fi
    else
        echo -e "${YELLOW}⚠${NC} ELF 文件未生成（需要先构建）"
        ((FAILED++))
    fi

    if [ -f "build/GD32F303RC.bin" ]; then
        size=$(ls -lh build/GD32F303RC.bin | awk '{print $5}')
        echo -e "${GREEN}✓${NC} BIN 文件已生成"
        echo "  大小: $size"
        ((PASSED++))
    else
        echo -e "${YELLOW}⚠${NC} BIN 文件未生成"
        ((FAILED++))
    fi

    if [ -f "build/compile_commands.json" ]; then
        echo -e "${GREEN}✓${NC} compile_commands.json 已生成"
        ((PASSED++))
    else
        echo -e "${RED}✗${NC} compile_commands.json 未生成"
        ((FAILED++))
    fi
else
    echo -e "${YELLOW}⚠${NC} build 目录不存在（需要先构建）"
    ((FAILED++))
fi
echo ""

# ============ 权限检查 ============
echo "══ 权限检查 ══"
if [ -r "/dev/ttyUSB0" ] 2>/dev/null; then
    echo -e "${GREEN}✓${NC} 有读取 /dev/ttyUSB0 的权限"
    ((PASSED++))
elif groups $USER | grep -q dialout; then
    echo -e "${GREEN}✓${NC} 用户在 dialout 组中"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠${NC} 可能没有 USB 设备访问权限"
    echo "  运行: sudo usermod -aG dialout $USER"
    ((FAILED++))
fi
echo ""

# ============ OpenOCD 连接测试 ============
echo "══ OpenOCD 连接测试 ══"
if openocd -f gd32f303_flash.cfg -c "init; reset halt; exit" &>/dev/null; then
    echo -e "${GREEN}✓${NC} OpenOCD 成功连接到目标芯片"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠${NC} OpenOCD 连接测试失败"
    echo "  请检查："
    echo "  1. 调试器是否已连接"
    echo "  2. 目标芯片是否已供电"
    echo "  3. gd32f303_flash.cfg 配置是否正确"
    ((FAILED++))
fi
echo ""

# ============ 总结 ============
echo "=================================================="
echo "检查结果总结"
echo "=================================================="
echo -e "${GREEN}通过: $PASSED${NC}"
echo -e "${RED}失败: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ 环境检查完全通过！可以开始调试。${NC}"
    echo ""
    echo "快速开始："
    echo "  1. 按 F5 启动调试"
    echo "  2. 选择 'OpenOCD Debug (GD32F303RCT6)'"
    echo "  3. 程序将自动编译、烧写和调试"
    exit 0
else
    echo -e "${RED}✗ 环境检查发现问题，请根据上面的提示修复。${NC}"
    exit 1
fi

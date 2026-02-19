# INFO

Ubuntu 22.04 vscode + openOCD + arm-none-eabi + cmake 编译、烧录及调试；

# Build

```
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build --target all
```

# Load

openocd -f ../gd32f303_flash.cfg -c "program GD32F303RC.elf verify reset exit"

# Run

启动 OpenOCD 并保持连接（不加 `-c shutdown`）：

1. **bash**

   ```
   openocd -f ../gd32f303_flash.cfg
   ```
2. 另开终端，用 telnet 连接到 OpenOCD（默认端口 4444）：
   **bash**

   ```
   telnet localhost 4444
   ```

   3.通过调试器强制运行程序（验证程序是否正常）

halt
reg sp 0x20001000          # 设置栈指针（来自向量表第一个字）
reg pc 0x08007971           # 设置 PC 为复位向量（注意最低位为1表示Thumb）
resume

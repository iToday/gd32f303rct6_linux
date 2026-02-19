set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 生成 compile_commands.json 用于 IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(GD32F303RC C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options(-g3)  # 添加调试符号

add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

########################################################################################
#                                        编译选项                                        #
########################################################################################
# 设置Debug编译标志 - 必须在add_compile_options之前
set(CMAKE_C_FLAGS_DEBUG "-Og -g3 -DDEBUG" CACHE STRING "C Debug flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3 -DDEBUG" CACHE STRING "C++ Debug flags" FORCE)

SET(CMAKE_C_FLAGS_RELEASE "-DRELEASE")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-O3)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-O2 -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og)  # -g3 已在上面设置
endif ()

#请在此处添加头文件路径
include_directories(
    FWLIB/Include
    CMSIS/
    App/
    App/sys/
    App/delay
    App/usart
    Drivers/LCD/
    Drivers/IIC/
    Drivers/KEY/
    Drivers/LED/
    Drivers/TIMER/
    Drivers/TOUCH/
    Drivers/24CXX/
    Middlewares/FreeRTOS/include
    Middlewares/FreeRTOS/portable/GCC/ARM_CM4F
    Middlewares/FreeRTOS/portable/MemMang
    Middlewares/LVGL/GUI
    Middlewares/LVGL/GUI/lvgl/src
    Middlewares/LVGL/GUI/lvgl
    Middlewares/LVGL/GUI/lvgl/examples/porting
    Middlewares/LVGL/GUI_APP/demos/stress
    Middlewares/MALLOC
)

add_definitions(-DDEBUG -DGD32F3 -DGD32F30x -DGD32F303 -DGD32F30X_HD -DUSE_STDPERIPH_DRIVER)

#请在此处此处添加.c文件目录
file(GLOB_RECURSE SOURCES
    "App/*.*"
    "CMSIS/*.*"
    "FWLIB/*.*"
    "Startup/*.*"
    "Drivers/LCD/*.*"
    "Drivers/IIC/*.*"
    "Drivers/KEY/*.*"
    #"Drivers/LED/*.*"  # 与 App/led.c 重复，已排除
    "Drivers/TIMER/*.*"
    "Drivers/TOUCH/*.*"
    "Drivers/24CXX/*.*"
    "Middlewares/FreeRTOS/*.*"
    "Middlewares/FreeRTOS/portable/GCC/ARM_CM4F/*.*"
    "Middlewares/FreeRTOS/portable/MemMang/heap_4.c"
    "Middlewares/LVGL/GUI/lvgl/src/*.*"
    "Middlewares/LVGL/GUI/lvgl/*.*"
    "Middlewares/LVGL/GUI_APP/demos/stress/*.*"
    "Middlewares/MALLOC/*.*"
)

# lcd.c 中通过 #include "lcd_ex.c" 包含了 lcd_ex.c，排除单独编译避免重复定义
list(FILTER SOURCES EXCLUDE REGEX ".*lcd_ex\\.c$")
# RVDS port 是 Keil 专用，GCC 下不能编译
list(FILTER SOURCES EXCLUDE REGEX ".*/portable/RVDS/.*")
# FreeRTOS 只保留 heap_4，排除其他 heap 实现
list(FILTER SOURCES EXCLUDE REGEX ".*/MemMang/heap_[^4].*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/GD32F303RC.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-specs=nano.specs -lc -lgcc)
#add_link_options(-lc -lgcc)
add_link_options(-specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

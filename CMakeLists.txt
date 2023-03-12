cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

# Pull in the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(controller C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Set up our includes
include_directories(
        src/
        )

# Our source files
file(GLOB sourceFiles
        src/*
        src/network/*
        src/logging/*
        /Users/april/pico/pico-sdk/lib/lwip/src/apps/*
        /Users/april/pico/pico-sdk/lib/lwip/src/apps/mdns/*
        /Users/april/pico/pico-sdk/lib/lwip/src/apps/mqtt/*
        )

add_executable(solar-monitor
        ${sourceFiles}
        FreeRTOSConfig.h
        )


target_include_directories(solar-monitor PRIVATE
        .
        ${CMAKE_CURRENT_LIST_DIR}
        )


# pull in common dependencies
target_link_libraries(solar-monitor
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        hardware_dma
        hardware_gpio
        hardware_pio
        hardware_pwm
        hardware_uart
        pico_stdlib
        pico_unique_id
        pico_cyw43_arch_lwip_threadsafe_background
        )

# Enable the shell over UART (which is the picoprobe!)
pico_enable_stdio_usb(solar-monitor 0)
pico_enable_stdio_uart(solar-monitor 1)

# Create all of the files
pico_add_extra_outputs(solar-monitor)

cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME hello)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# 1. 定义 LVGL 源码根目录
set(LVGL_ROOT_DIR ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/LVGL/GUI/lvgl)
# 2. 递归查找 LVGL src/ 目录下所有 .c 文件（核心！覆盖所有依赖）
file(GLOB_RECURSE LVGL_ALL_SOURCES
        ${LVGL_ROOT_DIR}/src/*.c
        ${LVGL_ROOT_DIR}/examples/*.c
)
#hardware里的文件
set(hardware_ROOT_DIR ${CMAKE_SOURCE_DIR}/hardware)
file(GLOB_RECURSE hardware_ALL_SOURCES
        ${hardware_ROOT_DIR}/LCD/*.c
        ${hardware_ROOT_DIR}/TOUCH/*.c
)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}

        # LVGL 所有核心 .c 文件（批量添加，解决所有依赖）
        ${LVGL_ALL_SOURCES}
        ${hardware_ALL_SOURCES}

        #Core/Src/music.c
        #Core/Src/oled.c
        #Core/Src/font.c
        #Core/Src/game1.c
        Core/Src/lvgl_demo.c
        # ========== 补充 lv_tick.c 依赖的核心文件 ==========
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/lv_init.c          # lv_global 实现核心
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/tick/lv_tick.c

        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_group.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_class.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_pos.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_draw.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_event.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_id_builtin.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_property.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_scroll.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_style.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_obj_tree.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core/lv_refr.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/draw/lv_draw.c
        #接口函数
        Middlewares/Third_Party/LVGL/GUI/lvgl/examples/porting/lv_port_disp_template.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/examples/porting/lv_port_fs_template.c
        Middlewares/Third_Party/LVGL/GUI_APP/demos/lv_demos.c
        Middlewares/Third_Party/LVGL/GUI_APP/demos/stress/lv_demo_stress.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/examples/porting/osal/lv_example_osal.c
        Middlewares/Third_Party/LVGL/GUI/lvgl/examples/porting/osal/lv_example_osal.h
        hardware/SPI/softSPI.c
        hardware/SPI/softSPI.h
        # lv_tick_inc 实现
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        Core
        Core/Src
        hardware/LCD
        hardware/SPI
        hardware/TOUCH


        Middlewares/Third_Party/LVGL/GUI

        Middlewares/Third_Party/LVGL/GUI/lvgl
        Middlewares/Third_Party/LVGL/GUI/lvgl/src
        Middlewares/Third_Party/LVGL/GUI/lvgl/src/core
        Middlewares/Third_Party/LVGL/GUI/lvgl/examples/porting


        #lvgl的demo文件
        Middlewares/Third_Party/LVGL/GUI_APP/demos/stress
        Middlewares/Third_Party/LVGL/GUI_APP/demos/widgets
        Middlewares/Third_Party/LVGL/GUI_APP/demos/widgets/assets
        Middlewares/Third_Party/LVGL/GUI_APP/demos/vector_graphic
        Middlewares/Third_Party/LVGL/GUI_APP/demos/vector_graphic/assets
        Middlewares/Third_Party/LVGL/GUI_APP/demos/render
        Middlewares/Third_Party/LVGL/GUI_APP/demos/benchmark
        Middlewares/Third_Party/LVGL/GUI_APP/demos/gltf
        Middlewares/Third_Party/LVGL/GUI_APP/demos/keypad_encoder
        Middlewares/Third_Party/LVGL/GUI_APP/demos/music
        Middlewares/Third_Party/LVGL/GUI_APP/demos/render

)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries


)

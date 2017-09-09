
## Mini-ver for cmake
cmake_minimum_required (VERSION 3.3)

## project name
project (stm32f103zet6demo)

## language
enable_language(C ASM)

## platform
set (CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING TRUE) 

## compile
if (NOT (DEFINED "ARM_PREFIX"))
  set (ARM_PREFIX "arm-unknown-eabi-")
endif (NOT (DEFINED "ARM_PREFIX"))

set (CMAKE_C_COMPILER "${ARM_PREFIX}gcc")
set (CMAKE_CXX_COMPILER "${ARM_PREFIX}g++")
set (CMAKE_C_FLAGS "$ENV{CCFLAGS} -mcpu=cortex-m3 -mthumb")
set (CMAKE_ASM_COMPILER "${ARM_PREFIX}gcc")

## for platform
if (WIN32)
  set (EXE_POSTFIX ".exe")
endif(WIN32)

## setting up the baisc file for stm32f103zet6
### basic path
set (BASIC_PATH "${PROJECT_SOURCE_DIR}/stm32f103zet6")
### for source
set (BASIC_SRC_PATH "${BASIC_PATH}/source")
set (BASIC_SRC_FILE ${BASIC_SRC_FILE} "${BASIC_SRC_PATH}/core_cm3.c" "${BASIC_SRC_PATH}/startup.s" "${BASIC_SRC_PATH}/system_stm32f10x.c")
### for include
set (BASIC_INC_PATH "${BASIC_PATH}/include")
### for linker
set (CMAKE_EXE_LINKER_FLAGS " -T ${BASIC_PATH}/stm32f103ze.ld")

## macro
add_definitions ("-D STM32F10X_HD")

## include path
include_directories (${BASIC_INC_PATH})


# sub
add_subdirectory(demo1)

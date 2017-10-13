
## platform
set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_SYSTEM_PROCESSOR arm)
set (CMAKE_CROSSCOMPILING TRUE) 

## Mini-ver for cmake
cmake_minimum_required (VERSION 3.3)

## project name
project (stm32f103zet6demo)

## language
enable_language(C ASM)

## compile
if (NOT (DEFINED "ARM_PREFIX"))
  set (ARM_PREFIX "arm-unknown-eabi-")
endif (NOT (DEFINED "ARM_PREFIX"))

set (CMAKE_C_COMPILER "${ARM_PREFIX}gcc")
set (CMAKE_CXX_COMPILER "${ARM_PREFIX}g++")
set (CMAKE_C_FLAGS "$ENV{CCFLAGS} -mcpu=cortex-m3 -mthumb")
set (CMAKE_ASM_COMPILER "${ARM_PREFIX}gcc")

## for platform
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
  if (NOT (DEFINED "EXE_IGNORE"))
    set (EXE_POSTFIX ".exe") 
  endif(NOT (DEFINED "EXE_IGNORE"))
endif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

# use aiko and firmware
set(
  LIBRARY_A
  " -laiko -lstm32f10x -L${PROJECT_BINARY_DIR}/root/lib"
)


### for include
set (FIRMWARE_INC_PATH "${FIRMWARE_PATH}/include")
### for linker
set (
  CMAKE_EXE_LINKER_FLAGS
  " -T ${PROJECT_BINARY_DIR}/root/share/aiko/aiko.ld -nostdlib ${LIBRARY_A}" 
  )


## macro
add_definitions (" -g -D STM32F10X_HD -D USE_STDPERIPH_DRIVER")

## include path
include_directories(${PROJECT_BINARY_DIR}/root/include)
## library path
link_directories(${PROJECT_BINARY_DIR}/root/lib)


# sub program
add_subdirectory(aiko-demo)
#add_subdirectory(demol1)
#add_subdirectory(or-demo)

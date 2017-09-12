## demo-3 for enable the GPIO
set (DEMO_3_PATH "${PROJECT_SOURCE_DIR}/demo3")

## config for beep
GPIO_option("BEEP_GPIO_SET" "GPIOB" "BEEP_GPIO_PORT" "8" "BEEP_GPIO_CLK" "3")
if (NOT (DEFINED "BEEP_BUF_SIZE"))
  set (BEEP_BUF_SIZE 64)
endif (NOT (DEFINED "BEEP_BUF_SIZE"))

## configure file
configure_file (
  "${DEMO_3_PATH}/config.h.in"
  "${PROJECT_BINARY_DIR}/demo3/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}/demo3")

## target
add_executable (demo3.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo3.hex ALL
  DEPENDS demo3.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo3.elf${EXE_POSTFIX} demo3.hex
  )
  

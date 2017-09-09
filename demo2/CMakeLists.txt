## demo-2 for enable the GPIO
set (DEMO_2_PATH "${PROJECT_SOURCE_DIR}/demo2")

GPIO_option("LED_1_GPIO_SET" "GPIOB" "LED_1_GPIO_PORT" "0" "LED_1_GPIO_CLK" "3")
GPIO_option("LED_2_GPIO_SET" "GPIOF" "LED_2_GPIO_PORT" "7" "LED_2_GPIO_CLK" "7")

## configure file
configure_file (
  "${DEMO_2_PATH}/config.h.in"
  "${PROJECT_BINARY_DIR}/demo2/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}/demo2")

## target
add_executable (demo2.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo2.hex ALL
  DEPENDS demo2.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo2.elf${EXE_POSTFIX} demo2.hex
  )
  

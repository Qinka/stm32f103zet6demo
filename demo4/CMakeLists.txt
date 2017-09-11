## demo-4 for enable the GPIO
set (DEMO_4_PATH "${PROJECT_SOURCE_DIR}/demo4")

## config for LED
GPIO_option("LED_GPIO_SET" "GPIOB" "LED_GPIO_PORT" "5" "LED_GPIO_CLK" "3")
def_option("PWM_TIMx"  3)
def_option("PWM_CHx"   2)
def_option("PWM_REMAP" 2)
def_option("PWM_ARR"   899)
def_option("PWM_LVL_MAX" 300)



## configure file
configure_file (
  "${DEMO_4_PATH}/config.h.in"
  "${PROJECT_BINARY_DIR}/demo4/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}/demo4")

## target
add_executable (demo4.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo4.hex ALL
  DEPENDS demo4.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo4.elf${EXE_POSTFIX} demo4.hex
  )
  

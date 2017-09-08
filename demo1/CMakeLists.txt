macro (GPIO_option set_var set_def port_var port_def clk_var clk_def)
  # for GPIO set
  if (NOT (DEFINED "${ARGV0}"))
    set ("${ARGV0}" "${ARGV1}")
  endif (NOT (DEFINED "${ARGV0}"))
  # for port
  if (NOT (DEFINED "${ARGV2}"))
    set ("${ARGV2}" "${ARGV3}")
  endif (NOT (DEFINED "${ARGV2}"))
  # for clock
  if (NOT (DEFINED "${ARGV4}"))
    set ("${ARGV4}" "${ARGV5}")
  endif (NOT (DEFINED "${ARGV4}"))
endmacro(GPIO_option)
      


## demo-1 for enable the GPIO
set (DEMO_1_PATH "${PROJECT_SOURCE_DIR}/demo1")

GPIO_option("LED_1_GPIO_SET" "GPIOB" "LED_1_GPIO_PORT" "0" "LED_1_GPIO_CLK" "3")
GPIO_option("LED_2_GPIO_SET" "GPIOF" "LED_2_GPIO_PORT" "7" "LED_2_GPIO_CLK" "7")
GPIO_option("KEY_1_GPIO_SET" "GPIOC" "KEY_1_GPIO_PORT" "13" "KEY_1_GPIO_CLK" "4")
## configure file
configure_file (
  "${DEMO_1_PATH}/config.h.in"
  "${PROJECT_BINARY_DIR}/demo1/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}/demo1")

## target
add_executable (demo1.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo1.hex ALL
  DEPENDS demo1.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo1.elf${EXE_POSTFIX} demo1.hex
  )
    

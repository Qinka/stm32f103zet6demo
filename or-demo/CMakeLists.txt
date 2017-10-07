## for the demo of oled

set(OR_DEMO_PATH "${PROJECT_SOURCE_DIR}/oled-demo")

ON_RCC()
ON_GPIO()
ON_TIM()
ON_USART()
ON_SPI()

use_peripheral("${PROJECT_BINARY_DIR}/or-demo")

set (OR_SRC_FILE "hardware/oled/oled.c" ${OR_SRC_FILE})
set (OR_SRC_FILE "hardware/oled/codetab.c" ${OR_SRC_FILE})
set (OR_SRC_FILE "hardware/irda/irda.c" ${OR_SRC_FILE})
set (OR_SRC_FILE "main.c" ${OR_SRC_FILE})
set (OR_SRC_FILE "delay.c" ${OR_SRC_FILE})

include_directories ("hardware/oled")
include_directories ("hardware/irda")

add_executable(or-demo.elf ${OR_SRC_FILE} ${FIRMWARE_SRC_FILE})

## for demo(lib)1

set(DEMOL_1_PATH "${PROJECT_SOURCE_DIR}/demol1}")

ON_RCC()
ON_GPIO()
ON_TIM()
ON_USART()

use_peripheral("${PROJECT_BINARY_DIR}/demol1")

add_executable(demol1.elf main.c "${FIRMWARE_SRC_FILE}")

add_custom_target(demol1.hex ALL
  DEPENDS demol1.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demol1.elf${EXE_POSTFIX} demol1.hex
  )

set (DEMO_10_PATH "${PROJECT_SOURCE_DIR}/demo10")

## target
add_executable (demo10.elf main.c ${BASIC_SRC_FILE})
add_custom_target(demo10.hex ALL
  DEPENDS demo10.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo10.elf${EXE_POSTFIX} demo10.hex
  )

## demo-5 for enable the GPIO
set (DEMO_5_PATH "${PROJECT_SOURCE_DIR}/demo5")

## target
add_executable (demo5.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo5.hex ALL
  DEPENDS demo5.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo5.elf${EXE_POSTFIX} demo5.hex
  )
  

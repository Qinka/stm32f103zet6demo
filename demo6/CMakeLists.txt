set (DEMO_6_PATH "${PROJECT_SOURCE_DIR}/demo6")


## target
add_executable (demo6.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo6.hex ALL
  DEPENDS demo6.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo6.elf${EXE_POSTFIX} demo6.hex
  )
  

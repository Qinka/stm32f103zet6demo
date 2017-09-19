set (DEMO_9_PATH "${PROJECT_SOURCE_DIR}/demo9")


## target
add_executable (demo9.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo9.hex ALL
  DEPENDS demo9.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo9.elf${EXE_POSTFIX} demo9.hex
  )

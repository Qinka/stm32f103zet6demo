set (DEMO_7_PATH "${PROJECT_SOURCE_DIR}/demo7")


## target
add_executable (demo7.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo7.hex ALL
  DEPENDS demo7.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo7.elf${EXE_POSTFIX} demo7.hex
  )
  

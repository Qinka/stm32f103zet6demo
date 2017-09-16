set (DEMO_8_PATH "${PROJECT_SOURCE_DIR}/demo8")


## target
add_executable (demo8.media.elf media.c "${BASIC_SRC_FILE}")
add_executable (demo8.source.elf source.c "${BASIC_SRC_FILE}")
add_custom_target(demo8.media.hex ALL
  DEPENDS demo8.media.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo8.media.elf${EXE_POSTFIX} demo8.media.hex
  )  
add_custom_target(demo8.source.hex ALL
  DEPENDS demo8.source.elf
  COMMAND ${ARM_PREFIX}objcopy -O ihex demo8.source.elf${EXE_POSTFIX} demo8.source.hex
  )
  

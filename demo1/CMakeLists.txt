## demo-1 for enable the GPIO
set (DEMO_1_PATH "${PROJECT_SOURCE_DIR}/demo1")
add_executable (demo1.elf main.c "${BASIC_SRC_FILE}")
add_custom_target(demo1.hex ALL
  DEPENDS demo1.elf
  COMMAND arm-unknown-eabi-objcopy -O ihex demo1.elf demo1.hex
  )

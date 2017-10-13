

## add the source
include_directories(include)
variable_watch(AIKO_DEMO_SRC_FILE)
aux_source_directory(source/hardware AIKO_DEMO_SRC_FILE)
aux_source_directory(source/initialzation AIKO_DEMO_SRC_FILE)

add_executable(aiko-demo.elf 
  ${AIKO_DEMO_SRC_FILE})
target_link_libraries(aiko-demo.elf aiko)
target_link_libraries(aiko-demo.elf stm32f10x)
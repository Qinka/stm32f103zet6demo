## option firmware

macro(use_peripheral path)
  ## for adc
  if(DEFINED "USE_ADC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_adc.c")                                                                              
  endif(DEFINED "USE_ADC")
  ## for bkp
  if(DEFINED "USE_BKP")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_bkp.c")                                                                              
  endif(DEFINED "USE_BKP")
  ## for can
  if(DEFINED "USE_CAN")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_can.c")                                                                              
  endif(DEFINED "USE_CAN")
  ## for cec
  if(DEFINED "USE_CEC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_cec.c")                                                                              
  endif(DEFINED "USE_CEC")
  ## for crc
  if(DEFINED "USE_CRC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_crc.c")                                                                              
  endif(DEFINED "USE_CRC")
  ## for dac
  if(DEFINED "USE_DAC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_dac.c")                                                                              
  endif(DEFINED "USE_DAC")
  ## for dbgmcu
  if(DEFINED "USE_DBGMCU")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_dbgmcu.c")                                                                           
  endif(DEFINED "USE_DBGMCU")
  ## for dma
  if(DEFINED "USE_DMA")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_dma.c")                                                                              
  endif(DEFINED "USE_DMA")
  ## for exti
  if(DEFINED "USE_EXTI")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_exti.c")                                                                             
  endif(DEFINED "USE_EXTI")
  ## for flash
  if(DEFINED "USE_FLASH")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_flash.c")                                                                            
  endif(DEFINED "USE_FLASH")
  ## for fsmc
  if(DEFINED "USE_FSMC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_fsmc.c")                                                                             
  endif(DEFINED "USE_FSMC")
  ## for gpio
  if(DEFINED "USE_GPIO")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_gpio.c")                                                                             
  endif(DEFINED "USE_GPIO")
  ## for i2c
  if(DEFINED "USE_I2C")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_i2c.c")                                                                              
  endif(DEFINED "USE_I2C")
  ## for iwdg
  if(DEFINED "USE_IWDG")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_iwdg.c")                                                                             
  endif(DEFINED "USE_IWDG")
  ## for pwr
  if(DEFINED "USE_PWR")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_pwr.c")                                                                              
  endif(DEFINED "USE_PWR")
  ## for rcc
  if(DEFINED "USE_RCC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_rcc.c")                                                                              
  endif(DEFINED "USE_RCC")
  ## for rtc
  if(DEFINED "USE_RTC")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_rtc.c")                                                                              
  endif(DEFINED "USE_RTC")
  ## for sdio
  if(DEFINED "USE_SDIO")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_sdio.c")                                                                             
  endif(DEFINED "USE_SDIO")
  ## for spi
  if(DEFINED "USE_SPI")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_spi.c")                                                                              
  endif(DEFINED "USE_SPI")
  ## for tim
  if(DEFINED "USE_TIM")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_tim.c")                                                                              
  endif(DEFINED "USE_TIM")
  ## for usart
  if(DEFINED "USE_USART")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_usart.c")                                                                            
  endif(DEFINED "USE_USART")
  ## for wwdg
  if(DEFINED "USE_WWDG")
    set (FIRMWARE_SRC_FILE ${FIRMWARE_SRC_FILE} "${FIRMWARE_SRC_PATH}/stm32f10x_wwdg.c")                                                                             
  endif(DEFINED "USE_WWDG")
  ## config file
  configure_file(
    "${FIRMWARE_PATH}/stm32f10x_conf.h.in"
    "${ARGV0}/stm32f10x_conf.h"
    )
  include_directories("${ARGV0}") 
endmacro(use_peripheral)

macro(ON_ADC)
  set (USE_ADC "#include \"stm32f10x_adc.h\"")
endmacro(ON_ADC)
macro(ON_BKP)
  set (USE_BKP "#include \"stm32f10x_bkp.h\"")
endmacro(ON_BKP)
macro(ON_CAN)
  set (USE_CAN "#include \"stm32f10x_can.h\"")
endmacro(ON_CAN)
macro(ON_CEC)
  set (USE_CEC "#include \"stm32f10x_cec.h\"")
endmacro(ON_CEC)
macro(ON_CRC)
  set (USE_CRC "#include \"stm32f10x_crc.h\"")
endmacro(ON_CRC)
macro(ON_DAC)
  set (USE_DAC "#include \"stm32f10x_dac.h\"")
endmacro(ON_DAC)
macro(ON_DBGMCU)
  set (USE_DBGMCU "#include \"stm32f10x_dbgmcu.h\"")
endmacro(ON_DBGMCU)
macro(ON_DMA)
  set (USE_DMA "#include \"stm32f10x_dma.h\"")
endmacro(ON_DMA)
macro(ON_EXTI)
  set (USE_EXTI "#include \"stm32f10x_exti.h\"")
endmacro(ON_EXTI)
macro(ON_FLASH)
  set (USE_FLASH "#include \"stm32f10x_flash.h\"")
endmacro(ON_FLASH)
macro(ON_FSMC)
  set (USE_FSMC "#include \"stm32f10x_fsmc.h\"")
endmacro(ON_FSMC)
macro(ON_GPIO)
  set (USE_GPIO "#include \"stm32f10x_gpio.h\"")
endmacro(ON_GPIO)
macro(ON_I2C)
  set (USE_I2C "#include \"stm32f10x_i2c.h\"")
endmacro(ON_I2C)
macro(ON_IWDG)
  set (USE_IWDG "#include \"stm32f10x_iwdg.h\"")
endmacro(ON_IWDG)
macro(ON_PWR)
  set (USE_PWR "#include \"stm32f10x_pwr.h\"")
endmacro(ON_PWR)
macro(ON_RCC)
  set (USE_RCC "#include \"stm32f10x_rcc.h\"")
endmacro(ON_RCC)
macro(ON_RTC)
  set (USE_RTC "#include \"stm32f10x_rtc.h\"")
endmacro(ON_RTC)
macro(ON_SDIO)
  set (USE_SDIO "#include \"stm32f10x_sdio.h\"")
endmacro(ON_SDIO)
macro(ON_SPI)
  set (USE_SPI "#include \"stm32f10x_spi.h\"")
endmacro(ON_SPI)
macro(ON_TIM)
  set (USE_TIM "#include \"stm32f10x_tim.h\"")
endmacro(ON_TIM)
macro(ON_USART)
  set (USE_USART "#include \"stm32f10x_usart.h\"")
endmacro(ON_USART)
macro(ON_WWDG)
  set (USE_WWDG "#include \"stm32f10x_wwdg.h\"")
endmacro(ON_WWDG)

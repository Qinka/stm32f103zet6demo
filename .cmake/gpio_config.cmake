macro (GPIO_option set_var set_def port_var port_def clk_var clk_def)
# for GPIO set
if (NOT (DEFINED "${ARGV0}"))
  set ("${ARGV0}" "${ARGV1}")
endif (NOT (DEFINED "${ARGV0}"))
# for port
if (NOT (DEFINED "${ARGV2}"))
  set ("${ARGV2}" "${ARGV3}")
endif (NOT (DEFINED "${ARGV2}"))
# for clock
if (NOT (DEFINED "${ARGV4}"))
  set ("${ARGV4}" "${ARGV5}")
endif (NOT (DEFINED "${ARGV4}"))
endmacro(GPIO_option)
    
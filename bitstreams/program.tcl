open_hw
connect_hw_server -url localhost:3121
open_hw_target
set_property PROGRAM.FILE {design_top_wrapper.bit} [get_hw_devices xcu280-fsvh2892-2L-e]
program_hw_device [get_hw_devices xcu280-fsvh2892-2L-e]
close_hw_target
exit
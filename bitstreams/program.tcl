open_hw
connect_hw_server -url localhost:3121
open_hw_target
set_property PROGRAM.FILE {design_top_wrapper.bit} [get_hw_devices xcu280_u55c_0]
program_hw_device [get_hw_devices xcu280_u55c_0]
close_hw_target
exit
# move the configuration in here for ctp7, copy the mp7 project which just serializes the inputs

# open the project, don't forget to reset
open_project -reset "Layer2_test_v0"
set_top test_PFHT
add_files layer2/simple_layer2algo_hw.cpp -cflags "-DTESTCTP7 -I./"
## Test main
add_files -tb layer2/simple_layer2algo_test.cpp  -cflags "-DTESTCTP7 -I./"
add_files -tb layer2/simple_layer2algo_ref.cpp -cflags "-DTESTCTP7 -I./"
##
add_files -tb layer2/pattern_reader.cpp  -cflags "-DTESTCTP7 -I./"
add_files -tb pattern_serializer.cpp -cflags "-DTESTCTP7  -std=c++0x"
add_files -tb firmware/simple_fullpfalgo.cpp -cflags "-DTESTCTP7 -I./"
# input data
add_files -tb proj3-ctp7-full/solution/csim/build/ctp7_output_patterns_nomux.txt

# reset the solution
open_solution -reset "solutionL2"
set_part {xc7vx690tffg1927-2}
create_clock -period 4.16667 -name default
set_clock_uncertainty 1.5

#config_interface -trim_dangling_port
# do stuff
csim_design
csynth_design
#cosim_design -trace_level all
#export_design -format ip_catalog -vendor "cern-cms" -version ${l1pfIPVersion} -description "${l1pfTopFunc}"

# exit Vivado HLS
exit


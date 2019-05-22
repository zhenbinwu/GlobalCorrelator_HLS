# move the configuration in here for ctp7, copy the mp7 project which just serializes the inputs

# open the project, don't forget to reset
open_project -reset "Layer2_test_v64"
#set_top test_PFHT
set_top test_JetClu
add_files layer2/simple_jetcluster_hw.cpp -cflags "-DTESTCTP7 -Ilayer2/ -I./"
add_files -tb layer2/simple_layer2algo_hw.cpp -cflags "-DTESTCTP7 -Ilayer2/"
add_files -tb layer2/ParallelFindMax.cpp -cflags "-DTESTCTP7 -Ilayer2/"
## Test main
add_files -tb layer2/simple_layer2algo_test.cpp  -cflags "-DTESTCTP7"
#add_files -tb layer2/simple_layer2algo_ref.cpp -cflags "-DTESTCTP7 -I./"
## Testing jets
add_files -tb layer2/simple_jetcluster_hw.cpp -cflags "-DTESTCTP7 -Ilayer2/ -I./"
add_files -tb layer2/simple_jetcluster_ref.cpp -cflags "-DTESTCTP7 -Ilayer2/"
##
add_files -tb layer2/pattern_reader.cpp  -cflags "-DTESTCTP7 -I./"
add_files -tb utils/pattern_serializer.cpp -cflags "-DTESTCTP7  -std=c++0x -Iutils/"
add_files -tb utils/DiscretePFInputs_IO.h -cflags "-DTESTCTP7 -I./"
add_files -tb DiscretePFInputs.h    -cflags "-DTESTCTP7 -std=c++0x"

# input data
add_files -tb layer2/mp7_output_patterns.txt

# reset the solution
open_solution -reset "solutionL2"
set_part {xc7vx690tffg1927-2}
create_clock -period 5 -name default

#config_interface -trim_dangling_port
# do stuff
csim_design
csynth_design
#cosim_design -trace_level all
#export_design -format ip_catalog -vendor "cern-cms" -version ${l1pfIPVersion} -description "${l1pfTopFunc}"

# exit Vivado HLS
exit


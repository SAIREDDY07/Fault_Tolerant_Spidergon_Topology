# CMake generated Testfile for 
# Source directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/at_1_phase
# Build directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/at_1_phase
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/tlm/at_1_phase/at_1_phase "/usr/bin/cmake" "-DTEST_EXE=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/at_1_phase/at_1_phase" "-DTEST_DIR=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/at_1_phase" "-DTEST_INPUT=" "-DTEST_GOLDEN=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/at_1_phase/results/expected.log" "-DTEST_FILTER=" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/tlm/at_1_phase/at_1_phase PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")

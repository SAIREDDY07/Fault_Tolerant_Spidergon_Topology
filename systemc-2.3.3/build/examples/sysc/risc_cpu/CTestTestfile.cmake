# CMake generated Testfile for 
# Source directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/risc_cpu
# Build directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/risc_cpu
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/sysc/risc_cpu/risc_cpu "/usr/bin/cmake" "-DTEST_EXE=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/risc_cpu/risc_cpu" "-DTEST_DIR=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/risc_cpu" "-DTEST_INPUT=" "-DTEST_GOLDEN=" "-DTEST_FILTER=" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/sysc/risc_cpu/risc_cpu PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
# CMake generated Testfile for 
# Source directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.1/dpipe
# Build directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.1/dpipe
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/sysc/2.1/dpipe/dpipe "/usr/bin/cmake" "-DTEST_EXE=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.1/dpipe/dpipe" "-DTEST_DIR=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/sysc/2.1/dpipe" "-DTEST_INPUT=" "-DTEST_GOLDEN=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/sysc/2.1/dpipe/golden.log" "-DTEST_FILTER=" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/sysc/2.1/dpipe/dpipe PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
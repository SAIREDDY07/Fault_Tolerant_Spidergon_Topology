# CMake generated Testfile for 
# Source directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/lt_dmi
# Build directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_dmi
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/tlm/lt_dmi/lt_dmi "/usr/bin/cmake" "-DTEST_EXE=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_dmi/lt_dmi" "-DTEST_DIR=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_dmi" "-DTEST_INPUT=" "-DTEST_GOLDEN=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/lt_dmi/results/expected.log" "-DTEST_FILTER=" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/tlm/lt_dmi/lt_dmi PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")

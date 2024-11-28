# CMake generated Testfile for 
# Source directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/lt_mixed_endian
# Build directory: /home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_mixed_endian
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(examples/tlm/lt_mixed_endian/lt_mixed_endian "/usr/bin/cmake" "-DTEST_EXE=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_mixed_endian/lt_mixed_endian" "-DTEST_DIR=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/build/examples/tlm/lt_mixed_endian" "-DTEST_INPUT=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/lt_mixed_endian/results/input.txt" "-DTEST_GOLDEN=/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/examples/tlm/lt_mixed_endian/results/expected.log" "-DTEST_FILTER=::" "-DDIFF_COMMAND=/usr/bin/diff" "-DDIFF_OPTIONS=-u" "-P" "/home/sai/Desktop/#Nirgam_Spidergon_final/systemc-2.3.3/cmake/run_test.cmake")
set_tests_properties(examples/tlm/lt_mixed_endian/lt_mixed_endian PROPERTIES  FAIL_REGULAR_EXPRESSION "^[*][*][*]ERROR")
